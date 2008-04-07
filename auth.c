/** Fichier auth.c
 **
 ** Routines de gestion des mots de passe
 **
 ** Copyright (c) 1991-1995 by Ollivier ROBERT
 ** A distribuer selon les regles de la GNU GPL General Public Licence
 ** Voir le fichier COPYING fourni.
 **
 **/

#include "config.h"     /* GNU configure */

                        /* fichier de configuration */
#include "conf.h"

static int  pam_pwcheck (struct passwd *calife, char * user_to_be, \
                         char * user_pass, int l_size);
static int  local_pwcheck (struct passwd *calife, char * user_to_be, \
                           char * user_pass, int l_size);
static void get_user_passwd(char * user_pass, int l_size);

/** authenticate_user
 **
 ** Demande et verifie le mot de passe pour l'utilisateur name
 ** Si name == root on ne fait rien.
 **
 ** Parametres :    name        char *  nom de l'utilisateur
 **                 user_to_be  char *  qui va-t-on devenir
 **                 this_time   char *  quand ?
 **                 tty         char *  sur quel tty
 **
 ** Retourne :      neant
 **
 ** Algo:
 **   si WITH_PAM est définit
 **     on essaie auth_pam()
 **     si ça plante, on continue avec un warning
 **     sinon retour ok
 **   fin
 **   vérification mdp
 ** 
 **/

void    
authenticate_user (char * who, char * user_to_be, char * when, char * tty)
{
    int             i = 0, rval, md5_here = 0;
    size_t          l_size = 0;
    
    struct  passwd  * calife;

    char    got_pass = AUTH_ERR;
    char    * pt_pass, * pt_enc, 
            * user_pass = NULL, * enc_pass = NULL, salt [10];

    /*
     * returns long usually
     */
#ifdef HAVE_WORKING_SYSCONF
    l_size = (size_t) sysconf (_SC_LINE_MAX);
#else
    l_size = MAX_STRING;
#endif /* HAVE_WORKING_SYSCONF */    

    user_pass = (char *) xalloc (l_size);
    enc_pass = (char *) xalloc (l_size);

     /*
      * become root again
      */
    GET_ROOT;
    calife = getpwnam (who);       /* null or locked password */
    RELEASE_ROOT;

    if (calife == NULL)
        die (1, "Bad pw data at line %d", __LINE__);

#ifdef WITH_PAM
    got_pass = pam_pwcheck (calife, user_to_be, user_pass, l_size);
#endif

    if (got_pass != AUTH_OK)
        got_pass = local_pwcheck (calife, user_to_be, user_pass, l_size);

    MESSAGE_1 ("Auth process returned %d\n", got_pass);

    if (got_pass == AUTH_NULL)
    {
        syslog (LOG_AUTH | LOG_ERR, "NULL CALIFE %s to %s on %s", who, \
                user_to_be, tty);
        closelog ();
        die (10, "Sorry.\n");
    }

    if (got_pass == AUTH_ERR)
    {
        syslog (LOG_AUTH | LOG_ERR, "BAD CALIFE %s to %s on %s", who, user_to_be, tty);
        closelog ();
        die (9, "Sorry.\n");
    }

    if (got_pass == AUTH_BADP)
    {
        syslog (LOG_AUTH|LOG_ERR, "GARBLED PASSWORD %s to unknown %s on%s",\
                who, user_to_be, tty);
        die (8, "Bad password string.\n");
    }

    free (user_pass);
    free (enc_pass);
}

/** local_pwcheck
 **
 ** Private method to check user password against local database
 **
 ** Parameters:   calife     struct passwd * requesting user
 **               user_to_be char *          target user
 **               user_pass  chat *          user password
 **               l_size     int             maximum size for password
 **
 ** Returns:      got_pass   Whether we got authenticated or not
 **/

static 
int local_pwcheck (struct passwd * calife, char * user_to_be, \
                   char * user_pass, int l_size)
{
    char * pt_pass, * who;
    int   i, rval, got_pass = AUTH_ERR, md5_here = 0;

#if defined (HAVE_SHADOW_H) && defined (HAVE_GETSPNAM) && !defined(UNUSED_SHADOW)
    struct  spwd  * scalife;
#endif

    who = calife->pw_name;
    MESSAGE_1 ("Testing w/o PAM with got_pass = %d\n", got_pass);
#if defined (HAVE_SHADOW_H) && defined (HAVE_GETSPNAM) && !defined(UNUSED_SHADOW)

    GET_ROOT;
    scalife = getspnam (who);       /* null or locked password */
    RELEASE_ROOT;

    /*
     * Goal is to manipulate only "calife", not both so if "scalife" is
     * valid, copy "scalife" interesting data into "calife"
     */
    if (scalife)
    {
        calife->pw_passwd =
            (char *) xalloc (strlen (scalife->sp_pwdp) + 1);
        strcpy (calife->pw_passwd, scalife->sp_pwdp);
    }
#endif /* HAVE_SHADOW_H */

    MESSAGE ("Not using PAM.\n");
    if ((*(calife->pw_passwd)) == '\0' || (*(calife->pw_passwd)) == '*')
        return (AUTH_NULL);


    if (getuid () != 0)
    {
        char    * pt_pass, * pt_enc, 
                * user_pass, * enc_pass, salt [10];

        user_pass = (char *) xalloc (l_size);
        enc_pass = (char *) xalloc (l_size);

        /*
         * cope with MD5 based crypt(3)
         */
        if (!strncmp (calife->pw_passwd, "$1$", 3)) /* MD5 */
        {
            char * pp = (char *) xalloc (strlen (calife->pw_passwd) + 1);
            char * md5_salt;
            char * md5_pass;

            md5_here = 1;
            strcpy (pp, calife->pw_passwd + 3);
            md5_salt = strtok (pp, "$");
            md5_pass = strtok (NULL, "$");
        
            if (md5_pass == NULL || 
                md5_salt == NULL ||
                (strlen (md5_salt) > 8))   /* garbled password */
  return (AUTH_BADP);
            MESSAGE_1 ("MD5 password found, salt=%s\n", md5_salt);
            strcpy (salt, md5_salt);            
            free (pp);
        }
        else
        {
            strncpy (salt, calife->pw_passwd, 2);
            salt [2] = '\0';
        }

        for ( i = 0; i < MAX_ATTEMPTS; i ++ )
        {
            get_user_passwd (user_pass, l_size);
            /*
             * At this point, either we have a password or
             * it has died already
             */
            pt_enc = (char *) crypt (user_pass, calife->pw_passwd);
            /*
             * Wipe out the cleartext password
             */
            memset (user_pass, '\0', l_size);

            /*
             * Move from the static buffer intoa safe location of our own
             */
            memset (enc_pass, '\0', l_size);
            strcpy (enc_pass, pt_enc);
            /*
             * assumes standard crypt(3)
             */
            if (!strcmp (enc_pass, calife->pw_passwd))
            {
                got_pass = AUTH_OK;
                break;
            }
        } /* for */
    } /* end if for getuid() */
    return (got_pass);
}

/** pam_pwcheck
 **
 ** Private method to check user password against PAM backend.
 **
 ** Parameters:   calife     struct passwd * target user
 **               user_to_be char *          target user
 **               user_pass  chat *          user password
 **               l_size     int             maximum size for password
 **
 ** Returns:      whether the password was obtained through PAM or not 
 **               (including failure in PAM process)
 **/

static 
int pam_pwcheck (struct passwd *calife, char * user_to_be, char * user_pass,\
                 int l_size)
{
    char * pt_pass, * who;
    int   i, rval, got_pass = AUTH_ERR;

#ifdef WITH_PAM
    who = calife->pw_name;
    if (getuid () != 0)
    {
        MESSAGE ("Trying PAM\n");
        for ( i = 0; i < MAX_ATTEMPTS; i++ )
        {
            get_user_passwd (user_pass, l_size);
            /*

             * At this point, either we have a password or
             * it has died already
             */
            MESSAGE ("Testing auth with PAM.\n");

            rval = auth_pam (&calife, user_pass);

            MESSAGE_1 ("PAM auth_pam returned %d\n", rval);
            if (rval)
            {
                syslog (LOG_AUTH | LOG_ERR, "PAM failed with code %d for %s", rval, who);
                /*
                 * Check return value:
                 * - 0:   auth succeeded
                 * - >0:  auth failed.
                 */ 
                /* Fallback to previous methods? */
            }
            else
            {
                syslog (LOG_AUTH | LOG_INFO, "PAM auth succeeded for %s", who);
                got_pass = AUTH_OK;
                break;
            }
        } /* end for */
    }
    else
        got_pass = AUTH_OK;
#endif
    return got_pass;
}

/** get_user_passwd
 **
 ** When not using PAM, get the user's password with getpass/getpassphrase
 **
 ** Parameters:    user_pass    char * password
 **                l_size       int    maximum size
 **
 ** Returns:       nothing
 **/

static
void get_user_passwd(char * user_pass, int l_size)
{
    char * pt_pass;

    /* XXX Solaris 10
     * Solaris 5.10 (and maybe before) truncate getpass() entry
     * to 8 bytes, use getpassphrase(3) instead
     *
     * Use getpassphrase(3) if available
     */
#ifdef HAVE_GETPASSPHRASE
        pt_pass = (char *) getpassphrase ("Password:");
#else
        pt_pass = (char *) getpass ("Password:");
#endif /* HAVE_GETPASSPHRASE */
    /* 
     * XXX don't assume getpass(3) will check the buffer
     * length. Linux glibc apparently lacks such checking and
     * will happily segfault if the previous entered password
     * was too big.
     * cf. <URL:http://www.securityfocus.com/archive/1/355510>
     */
    if (pt_pass == NULL)
        die(1, "Corrupted or too long password");
    memset (user_pass, '\0', l_size);
    /*
     * Be a bit more careful there
     */
    strncpy (user_pass, pt_pass, l_size);
    user_pass[l_size - 1] = '\0';
}

/*

  get_user_data
    calife
    scalife
  get_user_password
    (select getpass or getpassphrase)
  check_passwd
    if_pam
      auth_pam
    else
      (select encryption method based on sig $nn$salt$passwd)
    end
 */
