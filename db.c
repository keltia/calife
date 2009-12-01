/** Fichier db.c
 **
 ** Routines de gestion des mots de passe et des fichiers
 ** de configuration pour calife.
 **
 ** Copyright (c) 1991-2010 by Ollivier ROBERT
 ** A distribuer selon les regles de la GNU GPL General Public Licence
 ** Voir le fichier COPYING fourni.
 **
 **/

#ifndef lint
static const char * rcsid = "@(#) $Id: db.c,v 5a1f3f02b317 2009/12/01 17:00:37 roberto $";
#endif

#include "config.h"     /* GNU configure */

                        /* fichier de configuration */
#include "conf.h"

static FILE * fp = NULL;    /* Moved from calife.c */

/** On cherche et ouvre la database locale.
 **
 ** Dans tous les cas, le fichier log est ouvert (il est local). Si syslogd(8)
 ** est actif on ouvre le "fichier" pour syslog(3). Le fichier de log devient
 ** maintenant dependant de NO_SYSLOG: s'il n'y a pas de syslog d'utilisable
 ** alors on ouvre le fichier de log.
 **
 ** Parametres :    neant
 **
 ** Retourne :      int     s'il y a un probleme renvoie 1 sinon 0.
 **/

int    
open_databases (void)
{
    int err;

    MESSAGE ("Opening databases...\n");
    /*
     * become root again
     */
    GET_ROOT;

    /*
     * open protected database
     */
    fp = fopen (AUTH_CONFIG_FILE, "r");
    if (fp == NULL)
    {
        syslog (LOG_AUTH | LOG_ERR, "No database in `%s', launching su...\n", AUTH_CONFIG_FILE);
        MESSAGE_1 ("No database in `%s'...\n", AUTH_CONFIG_FILE);                              
        return 1;
    }
    else 
    {
        /*
         * XXX Is this still necessary?
         */    
        err = fchmod (fileno (fp), 0400);
        if (err)
            die(6, "Unable to change '%s' permissions.", AUTH_CONFIG_FILE);
            /*NOTREACHED*/
    }
    /*
     * stay non root for a time
     */
    RELEASE_ROOT;
    /*
     * Success
     */
    return 0;
}

/** Verifie si l'utilisateur de nom name a le droit d'utiliser
 ** calife.
 **
 ** Line format is :
 **
 ** login[:][shell][:user1,user2,user3,...]
 ** @group[:][shell][:user1,user2,...]
 **
 ** Parametres :    name        char *  nom de l'utilisateur
 **                 user_to_be  char *  futur nom d'utilisateur
 **
 ** Retourne :      int     1 si l'utilisateur est autorise 0 sinon.
 **/

int 
verify_auth_info (char * name, char * user_to_be)
{
    int     allowed = 0;
    char    ** user_list = NULL;
    size_t  l_line = 0;
    int     nb_users = 0;
    char    * line, * ptr;
    char    * line_name, * line_shell;
    char    * group_name = NULL, * group = NULL, **p;
    struct group *gr_group;
    
    /*
     * let's search if user allowed or not
     */
#ifdef HAVE_WORKING_SYSCONF
    l_line = (size_t) sysconf (_SC_LINE_MAX);   /* returns long usually */
#else
    l_line = MAX_STRING;
#endif /* HAVE_WORKING_SYSCONF */    

    line = (char *) xalloc (l_line);
    while (!feof (fp))
    {
        int lastc;

        if (fgets (line, l_line, fp) == NULL)
            break;
        /*
         * remove trailing '\n' if present
         */
        lastc = strlen (line) - 1;
        if (line [lastc] == '\n')
            line [lastc] = '\0';
        /*
         * empty line or comment  or null login -- not allowed but ignored
         */
        if (!line || (*line) == '#' || (*line) == '\0')
            continue;
        MESSAGE_1 ("Line read = |%s|\n", line);
        line_name = line;
        /*
         * Look for a @ as first character (for groups)
         */
        if (*line == '@')
        {
            group = strdup(line);
            group_name = strtok(group, ":");
            group_name++;           /* skip '@' */
            /*
             *  Look into /etc/groups (or its equivalent with get)
             */
             gr_group = (struct group *) getgrnam(group_name);
             if (gr_group == NULL)
             {
                 die(1, "No such group %s", group_name);
                 allowed = 0;
                 goto end;
             }
             for ( p = &gr_group->gr_mem[0]; *p ; p++ )
             {
                 MESSAGE_3 ("matching %s to %s in %s\n", name, *p, group_name);  
                 if (strcmp(name, *p) == 0)
                 {
                     MESSAGE_2 ("User %s Allowed through group %s\n", name, group_name);
                     _group = strdup (group_name);
                     strcpy (_group, group_name);
                     allowed = 2;
                     break;
                 }
             }
        }
        /*
         * Look for first ':'
         */
        ptr = strchr (line, ':');
        if (ptr == NULL)
        {
            /*
             * No shell and no user list
             */
            custom_shell = 0;
            
            /*
             * Bypass if already allowed through group
             */
            if (group_name != NULL && allowed == 1)
                goto escape;
            if (strcmp (line_name, name))   /* not us */
                continue;
            goto escape;            
        }
        /*
         * I got a ':', put a '\0' instead.
         */
        *ptr++ = '\0';
        /*
         * Do we have anything behind ?
         */
        if (*ptr == '\0')
            custom_shell = 0;

        MESSAGE_2 ("Current line_name = |%s| / name = |%s|\n", line_name,name);

        /*
         * Bypass if already allowed through group
         */
        if (group_name != NULL && allowed != 0)
            goto end;
        if (strcmp (line_name, name))   /* not us */
            continue;

        line_shell = ptr;
        /*
         * Look for another ':', maybe just after the first one
         */
        ptr = strchr (line_shell, ':');
        if (ptr == line_shell)
            custom_shell = 0;            
        /*
         * If we got another one, put a '\0' instead
         */
        if (ptr)
            *ptr++ = '\0';
        /*
         * Analyze the shell candidate
         */
        if ((*(line_shell) == '*'))   /* LOCKED ACCOUNT */
        {
            allowed = 0;
            goto end;
        }
        /*
         * look for pathname
         */
        if ((*(line_shell) == '/'))
        {
            /*
             * Is this a real shell ?
             */
            if (access (line_shell, R_OK | X_OK))
            {
                MESSAGE_1 (" access of |%s| failed, no custom_shell\n",
                           line_shell);
                custom_shell = 0;
            }
            else
            {
                custom_shell = 1;
                shell = (char *) xalloc (strlen (line_shell) + 1);
                strncpy (shell, line_shell, strlen (line_shell));
                shell [strlen (line_shell)] = '\0';
                MESSAGE_1 (" current line_shell = |%s|\n", line_shell);
            }
        }
        else
        {
            custom_shell = 0;
        }
        MESSAGE_1 (" custom_shell = |%d|\n", custom_shell);

        /*
         * Analyze the rest of the line
         */
        if (ptr)
        {
            MESSAGE_1 ("ptr = %s\n", ptr);
            /*
             * we've got a user list
             */
            if (*ptr)
            {
                char  * p, ** q;
                
                MESSAGE_1 (" current user_list = |%s|\n", ptr);
                /*
                 * the list should be user1,user2,...,user_n
                 */
                p = ptr;     /* count users # in the list */
                while (p && *p)
                    if (*p++ == ',')
                        nb_users++;
                if (*--p != ',')    /* last char is not a ',' */
                    nb_users++;

                MESSAGE_1 ("  users # = |%d|\n", nb_users);

                /*
                 * two passes to avoid fixed size allocations
                 */ 
                user_list = (char **) xalloc((nb_users + 1) * sizeof(char *));
                /*
                 * put pointers in user_list
                 */
                p = ptr;     
                q = user_list;
                /*
                 * 1st user is immediate
                 */
                (*q++) = p;
                while (p && *p)
                {
                    if (*p == ',')    /* found a ',' */
                    {
                        *p = '\0';
                        if (*++p == ',')
                            continue;
                        (*q++) = p++;     /* next char is user name */
                    }            
                    else
                        p++;        /* only regular letters */
                }            
            }
        }        
        /*
         * user_list is complete, now compare
         */
escape:
        if (user_list)
        {
            int i;
            
            for ( i = 0; i < nb_users; i++ )
            {
                MESSAGE_2(" comparing |%s| to |%s|\n", 
                          user_to_be, user_list [i]);
                if (!strcmp (user_list [i], user_to_be))
                {
                    allowed = 1;
                    goto end;   /* beuh but too many loops */
                }
            }            
        }
        else
        {
            allowed = 1;
            break;            
        }
    }
end:
    free (line);
    if (group)
        free(group);
    if (user_list)
        free (user_list);
    fclose (fp);
    if (geteuid() == 0)
        allowed = 1;
    MESSAGE_1 ("Exit from verify_auth_info with allowed=%d\n", allowed);
    return allowed;    
}

