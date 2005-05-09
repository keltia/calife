/** Fichier calife.c
 **
 ** SU ameliore dans ce sens qu'il consulte une database protegee dans
 ** ADMIN_DB/auth) afin de savoir si l'utilisateur est authorise a devenir root
 ** ou un autre utilisateur. Si oui, alors on lance un shell analogue a celui
 ** de l'utilisateur, sinon on lance un SU et l'utilisateur peut s'il a le
 ** password de root devenir calife a la place du calife.
 **
 ** Dans tous les cas, un fichier log est maintenu a jour des differents
 ** usages du programme soit par syslog(3) soit directement.
 **
 ** Le programme utilise maintenant les facilites du demon syslogd(8) pour
 ** afficher sur la console le succes ou l'echec du lancement d'un `...'.
 **
 ** Si le programme est compile en mode paranoiaque (le defaut maintenant),
 ** il demande le mot de passe de l'utilisateur qui lance ... de facon a
 ** assurer une securite maximale.
 **
 ** Le programme doit etre suid root pour fonctionner.
 **
 ** Compilation:    via le makefile
 **
 ** Copyright (c) 1991-2002 by Ollivier ROBERT
 ** A distribuer selon les regles de la GNU GPL General Public Licence
 ** Voir le fichier COPYING fourni.
 **
 ** 1.0 01/08/91    version pour Antenne 2, lance tcsh par defaut.
 ** 1.1 04/04/92    version pour Marne-L-V, utilise le shell de login du user
 **                 en question.
 ** 1.2 27/05/92    permet de ... vers quelqu'un autre comme su(1)
 ** 1.3 16/06/92    utilise syslog(3) en plus du log
 ** 1.4 17/06/92    log le debut et la fin de chaque session via un fork et
 **                 un wait.
 ** 1.5 18/06/92    ne change les droits effectifs (uid,gid) que pour le fils
 **                 facon a ce que ... soit tjs au meme user.
 ** 1.6 19/06/92    * correction bug stupide : oubli de verifier si le
 **                   user_to_be existe ... C'etait dans la 1.2 :-(
 **                 * on permet a root de ... sur un user, comme su(1).
 ** 1.7 29/06/92    * Rajout mode paranoiaque : rentree du mot de passe de
 **                   l'utilisateur pour authentification maximale.
 **                   Compilation avec un -DPARANOID
 **                   Seul root n'en a pas besoin.
 **                 * Si le mode paranoiaque est selectionne, l'entree dans
 **                   syslog sera calife+ et non pas calife tout court.
 **                 * Si le user en question n'a pas de mot de passe,
 **                   l'usage de calife est interdit pour des raisons
 **                   evidentes :-)
 **                 * Refonte interne, creation des deux fonctions
 **                   OpenDatabases & VerifyPassword
 **                 * Support pour changer le shell via le fichier
 **                   d'autorisation, ne marche pas encore, a voir...
 ** 1.8 07/06/92    * Mode paranoiaque par defaut : -DRELAXED pour l'inhiber
 **                 * Le changement de shell marche (sauf pour root a Marne ?
 **                   Permission denied).
 **                 * Mode debug (-DDEBUG) : plus verbeux, affiche les
 **                   variables, etc.
 **                 * Mode sans syslog (-DNO_SYSLOG) pour les machines avec
 **                   un vieux syslog (SunOS 3.2) ou pas du tout. (SVR3 et
 **                   autres derives).
 ** 1.9 08/06/92    * rajout de la liste des login sur lesquels un utilisateur
 **                   a le droit d'executer calife.
 **                 * Creation de la fonction VerifyAuthorizations qui va
 **                   effectuer la lecture de la base d'auth.
 **                 * Changement d'algo. pour la lecture de la base d'auth :
 **                   plusieurs espaces/tabs, commentaires.
 **                 * Support de SCO et SunOS 3.2 directement (non teste).
 **                 * Plus de mode CUSTOM_SHELL, ca devient en fait le
 **                   defaut, le code de gestion de liste de users n'etant
 **                   pas compatible si le mode n'est pas CUSTOM_SHELL. (et
 **                   ca ne vaut pas le coup de les reecrire :-))
 **                 * nettoyage du code, ajout de commentaires.
 **                 * Le probleme de l'execl de tcsh resiste encore, je ne
 **                   sais pas d'ou ca vient...
 **                 * Control de l'exit status du fils pour le pb ci-dessus.
 ** 2.0 13/09/92    * Abandon des listes d'utilisateurs, le programme revient
 **                   a sa vocation originelle, c-a-d devenir root. De toute
 **                   maniere, ca simplifie le code et root peut devenir qui il
 **                   veut.
 **                 * Le probleme de tcsh vient du fait qu'il a l'impression
 **                   d'etre setuid. Un patch violent vient de sortir...
 **                   #define NO_UGLY_QUIRK pour l'enlever :-)
 **                 * Calife devient network-aware en utilisant
 **                   les Yellow Pages (aka NIS) si elles sont presentes.
 **                   la map utilisee a pour nom `calife_auth'. Le format est
 **                   le meme que pour le fichier d'authorisations.
 **                 * ATTENTION : le format de la base change pour utiliser les
 **                   YP comme les maps standards : le separateur est `:'
 **                   login_name:[custom_shell]
 **                 * Ajout de string2passwd qui transforme une ligne de map
 **                   passwd en une structure passwd.
 **                 * Fix pour AIX 3.x qui ne semble pas avoir de macro
 **                   WCOREDUMP pour wait(3)
 ** 2.1 23/03/93    * Changement des messages affiches de maniere a mimer ceux
 **                   de su(1).
 **                 * Changement du format des messages a syslog(3)
 **                 * Modification du Makefile pour trouver su(1)
 **                   Ajout variable SU_CMD
 **                 * Utilisation de setprocmask a la place de signal pour
 **                   les systems Posix
 **                 * Portage Linux a l'aide de Rene Cougnenc : en fait juste
 **                   une autre entree dans conf.h
 **                 * Modification a l'appel de /bin/sh, on ajoute un exec.
 ** 2.2 03/09/93    * Refonte interne du code, simplification, changement
 **                   des noms des fonctions pour etre consistent, renvoie de
 **                   valeurs au lieu d'utiliser la variable globale allowed
 **                   qui disparait.
 **                 * Ajout de exec_shell pour simplification.
 **                 * Correction petit bug si YP.
 ** 2.3 04/09/93    * Separation en trois fichiers sources : calife.c, db.c et
 **                   util.c.
 **                 * Reecriture du Makefile
 **                 * Modification de conf.h pour les variables globales
 ** 				* Changement dans les logs : utilise syslog(3) _ou_ un
 ** 			      fichier de log annexe, pas les deux.
 ** 2.4 07/01/94    * FreeBSD a la fonction initgroups(2)
 **                 * FreeBSD 1.1 n'a pas besoin du hack violent pour tcsh
 **                 * correction bug dans lequel wanted_user est ecrasee
 ** 				  par l'appel a getpwname(calife).
 ** 				* FreeBSD 1.1 dispose des YP !!
 ** 				* Changement de _POSIX_SOURCE en POSIX_SIGNALS
 ** 2.5 22/04/94    * Incorporation de modifs pour SVR4 et sa p... de gestion
 ** 				  des shadow passwords.
 ** 				* Securisation du programme en empechant les cores avec
 **                   rlimit et la gestion des uid/euid/ssuid. Nouvelle option
 **                   HAVE_RLIMIT.
 **                 * Modification de la gestion de wait(2) : ajout du #define
 **                   HAVE_INT_WAIT pour les systèmes ayant ``int status'' et
 **                   non pas ``union wait status''.
 ** 2.6 25/09/94    * Ajout de l'execution d'un fichier .calife.out a la fin
 **                   de la session s'il existe. Cette modification s'inscrit
 **                   dans la gestion du serveur FrMug.
 **                 * Changement de NO_UGLY_QUIRK en NOSETUID_SHELL (inverse)
 **                 * Remplacement des seteuid(2) en setresuid(2) sur HP-UX.
 **                 * Fin du support YP.
 **                 * .calife.out devient global (plus intéressant). Le nom
 **                   est spécifié dans le Makefile, le symbole étant
 **                   CALIFE_OUT (par défaut /etc/calife.out)
 ** 2.7 23/04/95    * Rajout des listes d'utilisateurs -- pour un client
 **                   mais peut être pratique.
 **                   Le format est le suivant :
 **                   login[:shell][:user1,user2,...,usern]
 **                 * Elimination d'allocation statiques -> xalloc
 **                 * Ajout des macros pour les messages de DEBUG, rend le
 **                   code plus lisible.
 **                 * Fixes divers (mem. leak, etc.) potentiellement ennuyeux.
 **                 * Support de la version de crypt(3) utilisant MD5 sur
 **                   FreeBSD 2.x.
 **                 * Fixe un bug dans la recopie de user_to_be avec un '\0'
 **                   à la fin. Signalé par Marc Baudoin.
 **                 * utilisation de l'ancienne valeur de RLIMIT_CORE juste
 **                   avant l'exec du shell.
 **                 * Ajout d'un hack pour contourner un bug de SSH sur SunOS.
 **                   Si getlogin(3) renvoie NULL, on regarde dans le fichier
 **                   utmp.
 **                 * Ajout des appels à die() aux points statégiques.
 **                 * Réorganisation des commentaires, déplacement de code.
 ** 2.8 15/10/95    * Passage à GNU autoconf avec changement de divers
 **                   symboles.
 **                 * Support de "-" comme premier paramètre pour faire relire
 **                   les fichiers d'init (.login,.profile)
 **                 * Meilleur contrôle sur le fichier out_rc.
 **                 * Correction bug si argv[1] == '-'.
 **                 * Correction bug d'allocation mémoire, trouvé par
 **                   Thomas Quinot.
 **                 * Calife exige un tty associé à stdin, trouvé par
 **                   Laurent Wacrenier.
 **                 * Compilation sur Hurd + simplification des strcpy.
 **                   Thomas Quinot.
 **                 * Utilise utmp aussi sous Linux et amélioration du
 **                   traitement.
 **                 * Utilise un code analogue à su(8) pour pallier aux
 **                   déficiences de getlogin(3).
 **                 * Contourne un bug de getpass(3) sur Linux/glibc.
 **                 * /etc/calife.out devient optionnel.
 **                 * il est possible de spécifier un group dans calife.auth
 **                   via @group:...
 **                 * syslog(3) est maintenant requis.
 ** 3.0 04/05/05    * PAMification de calife, PAM est désormais *obligatoire*
 **                   les gens ne l'ayant pas sont invités à rester à 2.8.
 **                 * Le mode RELAXED sans mot de passe disparait.
 **/

#define MAIN_MODULE

#include "config.h"     /* généré par configure */

                        /* fichier de configuration */
#include "conf.h"

#ifndef lint
static char * rcsid = "$Id: //depot/security/calife/main/calife.c#45 $";
#endif /* lint */

FILE    * fp = NULL, * logfile = NULL;  /* fichier d'auth. et log */
int     custom_shell= 0;            /* modification du shell ? */
char    * shell;                    /* nom du shell */
uid_t   ssid;   					/* saved uid -- POSIX */
int     want_login = 0;             /* like 'su -' or not */
char    * _group;                   /* if auth. through group */

#ifdef WITH_PAM
pam_handle_t	*pamh = NULL;
#endif /* WITH_PAM */

#ifdef HAVE_RLIMIT
static struct rlimit rlp, orlp;
#endif

#ifdef STDC_HEADERS
int 
main (int argc, char * argv [])
#else /* !STDC_HEADERS */
int 
main(argc, argv)
    int    argc;
    char * argv [];
#endif
{
    int             allowed = 0;        /* l'utilisateur est-il valide ? */
    char            * name, * user_to_be, * tty, this_time [30];
    char            * login;
#ifdef WANT_GLOBAL_RC
    char            * out_rc;
#endif
    char            * uargv;
    uid_t           uid;
    time_t          t;
    struct passwd * test_user, *pwd;
#ifdef HAVE_POSIX_SIGNALS
    sigset_t        old_set, sig_set;
#endif
#ifdef WITH_PAM
    int             e;
#endif

    name = NULL;
    tty = NULL;
    ssid = geteuid ();  /* get saved uid for swapping -- security reasons */

    RELEASE_ROOT;

#ifndef DEBUG 
    if (ssid != 0)                /* ... not setuid, exit */
        exit (11);
#endif

                        /* displays compilation options */
#ifdef DEBUG
#ifdef STDC_HEADERS
    fprintf (stderr, "Calife : How to become someone else legally\n"
                     "Distribute in respect of the GNU General Public Licence\n"
                     "Copyright (c) 1991, 2002 by Ollivier ROBERT\n"
                     "%s\n", rcsid);
#else /* !STDC_HEADERS */
    fprintf (stderr, "Calife : How to become someone else legally\n"
    fprintf (stderr, "Distribute in respect of the GNU General Public Licence\n");
    fprintf (stderr, "Copyright (c) 1991, 2002 by Ollivier ROBERT\n");
    fprintf (stderr, "%s\n", rcsid);
#endif /* STDC_HEADERS */
    fprintf (stderr, "Options : ");
    fprintf (stderr, "custom_shell, ");
#ifdef NO_SETUID_SHELL 
    fprintf (stderr, "no_setuid_shell, ");
#endif /* NO_SETUID_SHELL */
#ifdef RELAXED
    fprintf (stderr, "relaxed_mode, ");
#endif /* RELAXED */
#ifdef WANT_GLOBAL_RC
    fprintf(stderr, "global_rc, ");
#endif /* WANT_GLOBAL_RC */
#ifdef WITH_PAM
    fprintf (stderr, "with_pam,\n");
#endif /* WITH_PAM */
    fprintf (stderr, "su_like, ");
    fprintf (stderr, "debug\n");
    fflush (stderr);
#endif /* DEBUG */

#ifdef HAVE_RLIMIT
    /*
     * save the old limit values
     */
    getrlimit (RLIMIT_CORE, &orlp);
    rlp.rlim_max = 0;   		/* no core file */
    rlp.rlim_cur = 0;
    setrlimit (RLIMIT_CORE, &rlp);
#endif /* HAVE_RLIMIT */    
#ifndef HAVE_POSIX_SIGNALS
                                /* no signals !! */
    (void) signal (SIGQUIT, SIG_IGN);
    (void) signal (SIGINT, SIG_IGN);
    (void) signal (SIGHUP, SIG_IGN);
    (void) signal (SIGABRT, SIG_IGN);
    (void) signal (SIGTERM, SIG_IGN);
    (void) signal (SIGCHLD, SIG_IGN);
#ifdef SIGTTOU
    (void) signal (SIGTTOU, SIG_IGN);
#endif /* SIGTTOU */
#else /* HAVE_POSIX_SIGNALS */
    sigemptyset (&sig_set);
    sigaddset (&sig_set, SIGQUIT);
    sigaddset (&sig_set, SIGINT);
    sigaddset (&sig_set, SIGHUP);
    sigaddset (&sig_set, SIGABRT);
    sigaddset (&sig_set, SIGTERM);
    sigaddset (&sig_set, SIGCHLD);
    sigaddset (&sig_set, SIGTTOU);
    sigprocmask (SIG_BLOCK, &sig_set, &old_set);
#endif

    /*
     * We insist on having a controlling terminal
     */
    if (!isatty (0))
    {
        die (1, "No tty associated with stdin, bailing out");
    }

    user_to_be = (char *) xalloc (MAXLOGNAME + 1);

    /*
     * default is root 
     */
    (void) strcpy (user_to_be, ROOT_LOGIN);
    /*
     * check arguments
     */
    uargv = argv[1];             /* points on username */
    if (argc == 3 && !strcmp (uargv, "-"))
    {
        want_login = 1;
        argc--;
        uargv = argv[2];         /* skip first arg */
    }
    if (argc >= 3)
        die (1, "Usage : %s [-] [user_name]\n", argv [0]);
    
    if (argc == 2)          /* un argument = user_to_be */
    {
        int n_len;

        /* 
         * check whether the parameter is a "-" or not
         */
        if (!strcmp (uargv, "-"))
            want_login = 1;
        else
        {                
            /*
             * verify length 
             */
            if ((n_len = strlen (uargv)) > MAXLOGNAME)
                die (1, "Name too long `%s'", uargv);
            /*
             * avoid misuse and special characters
             */
            if (strpbrk (uargv, " ,\t:+&#%$^()!@~*?<>=|\\/\"\n[]{}"))
                die (1, "Illegal characters in username `%s'", uargv);
            
            strncpy (user_to_be, uargv, n_len);
            user_to_be [n_len] = '\0';
        }
    }
    /*
     * get tty name, for the log file 
     */
    {
        char * p = ttyname (0);
        if (p == NULL)
            die (1, "No tty associated with stdin, bailing out");
        tty = (char *) xalloc (strlen (p) - 4);
        strcpy (tty, p + 5);        /* strip /dev/ */
    }

    /*
     * Get login name the standard way
     *
     * Code extracted from usr.bin/su/su.c
     * heavily modified.
     */
    uid = getuid();
    MESSAGE_1("Real uid = %ld\n", uid);
    login = getlogin();
    if (login == NULL)
    {
        pwd = getpwuid(uid);
        MESSAGE_2 ("getpwuid uid = %ld login = %s\n", uid, pwd->pw_name);
    }
    else
    {
        pwd = getpwnam(login);
        MESSAGE_1 ("getlogin = %s\n", login);
    }
    if (pwd == NULL)
        die (1, "who are you?");
    login = strdup (pwd->pw_name);

    MESSAGE_2 ("uid = %ld name = %s\n", uid, login);

    if (login == NULL || *login == '\0')
    {
    /*
     * Under SunOS, ssh sometimes makes getlogin(3) think the utmp
     * entry is invalid thus getlogin(3) returns NULL.
     * We'll look in utmp ourselves.
     */
#if defined(SUNOS4) || defined(__linux__)
        int         utmp_fd, get_out = 0, err = 0;
        struct utmp utmp;

#ifdef DEBUG
        fprintf (stderr, "getlogin failed, reading utmp\n");
        fflush (stderr);
#endif /* DEBUG */        
        utmp_fd = open (_PATH_UTMP, O_RDONLY);
        if (utmp_fd == -1)
            get_out = 1;
        else
        {
            /*
             * read each entry
             */
            do
            {                
                err = read (utmp_fd, &utmp, sizeof utmp);
                if (err == -1 || err == 0)
                {
                    get_out = 1;
                    break;
                }
            } while (strcmp (tty, utmp.ut_line));
        }
        close (utmp_fd);    
        /*
         * XXX
         * assuming 8 chars is unwise but there is no other way under SunOS
         * XXX
         */        
        {
#ifdef UT_NAMESIZE
            int max_size = UT_NAMESIZE;
#else /* !UT_NAMESIZE */
            int max_size = 8;
#endif /* UT_NAMESIZE */
            int l_name = 0;
            
            name = (char *) xalloc ((max_size + 1) * sizeof (char)); 
            strncpy (name, utmp.ut_name, sizeof utmp.ut_name);
            while (name [l_name] && l_name != max_size)
                l_name++;
            name [l_name] = '\0';
        }        
        if (get_out)
        {
#endif /* !SUNOS4  && !__linux__ */
            die (1, "You don't exist, get out.");
#if defined (SUNOS4) || defined (__linux__)
        }    
#endif /* SUNOS4 && __linux__ */
    }
    else
    {   
        /*
         * don't assume length 
         */
        int l_len = strlen (login);
        
        name = (char *) xalloc (l_len + 1);
        strncpy (name, login, l_len);
        name [l_len] = '\0';
    }    
    /*
     * open syslod log file
     */
#ifndef RELAXED
#ifdef WITH_PAM
    openlog ("calife/pam", LOG_PID | LOG_CONS, LOG_AUTH);
#else
    openlog ("calife+", LOG_PID | LOG_CONS, LOG_AUTH);
#endif /* WITH_PAM */
#else /* RELAXED */
    openlog ("calife", LOG_PID | LOG_CONS, LOG_AUTH);
#endif /* RELAXED */
    /*
     * does the user we want to become exist or not ? 
     */
    test_user = getpwnam (user_to_be);
    if (test_user == NULL)
    {
        syslog (LOG_AUTH | LOG_ERR, "BAD CALIFE %s to unknown %s on %s", name, user_to_be, tty);
        die (8, "Unknown user %s.\n", user_to_be);
        /* NOT REACHED */
    }
    /*
     * test if user already root 
     */
    if (uid == 0)
    {
        if (!strcmp (user_to_be, ROOT_LOGIN))
            die (0, "Already root.");
        allowed = 1;
    }
    /*
     * open database and log file
     */
    if (open_databases ())  /* successful opening ? */
        allowed = 0;
    else                    /* verify rights, even for root just for shell */
        allowed = verify_auth_info (name, user_to_be);

    /*
     * record the time
     */
    time (&t);
    strcpy (this_time, ctime (&t));
    this_time [24] = '\0';  /* strip \n */

    if (allowed)                    /* hello, master-to-be */
    {
#if defined(HAVE_SYS_WAIT_H)
        int             status;
#else /* !HAVE_SYS_WAIT_H */
        union wait      status;
#endif /* HAVE_SYS_WAIT_H */
        struct passwd * calife, * wanted_user, * p_calife;

        calife = (struct passwd *) xalloc (sizeof (struct passwd));
        wanted_user = (struct passwd *) xalloc (sizeof (struct passwd));

        verify_password (name, user_to_be, this_time, tty);
        switch (fork ())
        {
            case -1:               
                /* 
                 * fork error... 
                 */
                die (5, "\nFork failure ... exiting ...");
                /* NOT REACHED */
            case 0:                
                /*
                 * child process, will become shell
                 */
                test_user = getpwnam (user_to_be);
                memcpy (wanted_user, test_user, sizeof (struct passwd));
#ifdef DEBUG
                fprintf (stderr, "uid,gid: %lu,%lu --> %u,%u\n", getuid(), getgid(), wanted_user->pw_uid, wanted_user->pw_gid);
#endif             
#ifdef HAVE_POSIX_SIGNALS
                sigprocmask (SIG_SETMASK, &old_set, NULL);
#endif
                MESSAGE ("Looking for shell after changing rights...\n");

                p_calife = getpwnam (name); /* return ptr is static */
                if (p_calife == NULL)
                    die (1, "Bad pw data errno = %d", errno);

                memcpy (calife, p_calife, sizeof (struct passwd));
                if (logfile)
                    fclose (logfile);
                {
                    unsigned int e1, e2;

                    GET_ROOT;
                    
                    e2 = setgid (wanted_user->pw_gid);
#ifdef HAVE_INITGROUPS					/* don't know about linux */
                    initgroups (user_to_be, wanted_user->pw_gid);
#endif /* HAVE_INITGROUPS */
                    e1 = setuid (wanted_user->pw_uid);
#ifdef DEBUG                    
                    fprintf (stderr, " setr{u,g}id user=%s uid=%d gid=%d e1=%u e2=%u\n", user_to_be, wanted_user->pw_uid, wanted_user->pw_gid, e1, e2);
#endif /* DEBUG */                        
                }
                /*
                 * cleanup
                 */
                free (wanted_user);
                
                if (!custom_shell)
                {
                    exec_shell (calife->pw_shell);
                }
                else
                {
                    exec_shell (shell);
                }
                /* NOT REACHED well should not :-) */
                fprintf (stderr, "shell = >>%s<<\n", "/bin/sh");
                execl ("/bin/sh", "sh", 0);
                die (2, "\nAARRRGGGHHH, exec (shell) failed... errno = %d",
                     errno);
            default:                    
            {
                int fd;

                /*
                 * father, will wait for the child
                 */
                /*
                 * cleanup
                 */
                free (calife);
                free (wanted_user);

#ifdef WITH_PAM
                	if (pamh) 
                	{
                		if ((e = pam_open_session(pamh, 0)) != PAM_SUCCESS) 
                		{
                			syslog (LOG_ERR, "pam_open_session: %s",
                			    pam_strerror (pamh, e));
                		}
                		else if ((e = pam_setcred(pamh, PAM_ESTABLISH_CRED)) !=
                		    PAM_SUCCESS) 
                		{
                			syslog (LOG_ERR, "pam_setcred: %s",
                			    pam_strerror (pamh, e));
                		}
                	}
#endif /* WITH_PAM */

                if (allowed == 2)
                    syslog (LOG_AUTH | LOG_NOTICE, "%s@%s to %s on %s - BEGIN", 
                            name, _group, user_to_be, tty);
                else
                    syslog (LOG_AUTH | LOG_NOTICE, "%s to %s on %s - BEGIN", name, user_to_be, tty);
                wait (&status);

                RELEASE_ROOT;

#ifdef WANT_GLOBAL_RC
                out_rc = (char *) xalloc (strlen (CALIFE_OUT_FILE) + 1);
                strcpy (out_rc, CALIFE_OUT_FILE);

                MESSAGE_1 ("Looking for %s.\n", out_rc);

                fd = open (out_rc, O_RDONLY);
                if (fd != EOF)
                {
                    int err;
                    struct stat sb1, sb2;

                    /*
                     * Get some info
                     */
                    err = fstat (fd, &sb1);
                    if (err)
                    {
                        syslog (LOG_AUTH | LOG_NOTICE, "fstat: %s unreadable/executable", out_rc);
                        goto done;
                    }
                    /*
                     * See if symlink
                     */
                    err = lstat (out_rc, &sb2);
                    if (err)
                    {
                        syslog (LOG_AUTH | LOG_NOTICE, "lstat: %s unreadable/executable", out_rc);
                        goto done;
                    }
                    /*
                     * Verify no-one tried to change it on the fly
                     */
                    if (sb1.st_dev != sb2.st_dev ||
                        sb1.st_ino != sb2.st_ino)
                    {
                        syslog (LOG_AUTH | LOG_NOTICE, "lstat: %s IS A SYMLINK !!", out_rc);
                        goto done;
                    }
                    /*
                     * Verify that it is not rwxrwxrwx
                     */
                    if ((sb1.st_mode & S_IRWXO) == S_IRWXO)
                    {
                        syslog (LOG_AUTH | LOG_NOTICE, "fstat: %s IS WORLD WRITABLE !!", out_rc);
                        goto done;
                    }
                    /*
                     * See if at least "something/something/r-x"
                     */
                    if (((sb1.st_mode & S_IXOTH) == S_IXOTH) &&
                        ((sb1.st_mode & S_IXOTH) == S_IXOTH))
                    {
#if defined(HAVE_SYS_WAIT_H)
                        int         status1;
#else /* !HAVE_SYS_WAIT_H */
                        union wait  status1;
#endif /* HAVE_SYS_WAIT_H */
                        pid_t pid;

                        close (fd);
                        MESSAGE_1 ("%s found, executing it.\n", out_rc);
                        pid = fork();
                        if (pid == -1)
                        {
                            die (5, "\nFork failure. exiting ...");
                        }   
                        else if (pid)   /* father */
                        {
                            wait (&status1);
                        }
                        else            /* child */
                        {
                            /*
                             * Potential race condition: someone could have 
                             * changed out_rc. Could be fixed by using the
                             * /dev/fd/N trick but it is not universal...
                             * 
                             * We could dup fd into stdin and launch /bin/sh on
                             * that too...
                             *
                             * We're not root anymore so it is less important
                             */
                            execl ("/bin/sh", "sh", "-c", out_rc, NULL);
                            die (1, "Error reading %s", out_rc);
                        }
                    }
                    else
                        fprintf (stderr, "\a%s not executable/readable.\n", out_rc);
                }
                done:                    
#endif /* WANT_GLOBAL_RC */
                if (allowed == 2)
                    syslog (LOG_AUTH | LOG_NOTICE, "%s@%s to %s on %s - END.", 
                            name, _group, user_to_be, tty);
                else
                    syslog (LOG_AUTH | LOG_NOTICE, "%s to %s on %s - END.", name, user_to_be, tty);
                closelog ();
                /*
                 * cleanup
                 */
#ifdef WANT_GLOBAL_RC
                free (out_rc);
#endif /* WANT_GLOBAL_RC */

#ifdef WITH_PAM
              	if ((e = pam_setcred (pamh, PAM_DELETE_CRED)) != PAM_SUCCESS)
                		syslog (LOG_ERR, "pam_setcred: %s", pam_strerror(pamh, e));
              	if ((e = pam_close_session (pamh, 0)) != PAM_SUCCESS)
                		syslog(LOG_ERR, "pam_close_session: %s", pam_strerror(pamh, e));
              	if ((e = pam_end (pamh, e)) != PAM_SUCCESS)
                		syslog(LOG_ERR, "pam_end: %s", pam_strerror(pamh, e));
              	pamh = NULL;
#endif

#ifdef DEBUG
                if (WIFEXITED (status))
                {
                    if (WEXITSTATUS (status))
                        fprintf (stderr, "Child died with error level of %d, check please.\n", WEXITSTATUS (status));
                }
                else
                {
                    if (WIFSIGNALED (status))
                        fprintf (stderr, "Child was killed by signal %d\n", WTERMSIG (status));
#if !defined(AIX3) && !defined(HAVE_SYS_WAIT_H)
#ifndef _POSIX_SOURCE
                    if (WCOREDUMP (status))
                        fprintf (stderr, "And there is a core to look upon...");
#endif /* !_POSIX_SOURCE */
#endif /* !AIX3 */
                }
#endif /* DEBUG */
            }
            break;
        }
    }
    else
    {
    /*
     * su will ask for the password, if any
     */
        syslog (LOG_AUTH | LOG_ERR, "BAD CALIFE %s to %s on %s",
                name, user_to_be, tty);
        closelog ();
        fprintf (stderr, "Calife failed. Sorry, trying to run su.\n");
        fflush (stderr);
        /*
         * give the baby to su
         */
        execl (SU_CMD, "su", user_to_be, 0);
        die (3, "\nBy Zandru's Nineth Hell !!! Even su failed...");
        /* NOT REACHED */
    }
    return 0;
}

/** Trouve et execute le shell passe en parametre
 ** 
 ** Parametres :    shell   char *       shell a executer
 **
 ** Retourne :      n'est pas sense revenir...
 **
 ** Privileges :    aucun
 **/

#ifdef STDC_HEADERS
void 
exec_shell (char * shell_name)
#else /* !STDC_HEADERS */
void 
exec_shell (shell_name)
char * shell_name;
#endif /* STDC_HEADERS */
{
    int         fd, err;
    char        * shell_arg0;
    struct stat sb;   
     
    MESSAGE_2 ("standard_shell = |%100s| basename = |%100s|\n", 
                shell_name, basename (shell_name));
 
    fd = open (shell_name, O_RDONLY);
    if (fd == -1)
        die (3, "Shell not executable/not found\n");

    err = fstat (fd, &sb);
    if (err)
        die (3, "Can't fstat()\n");
    
    close (fd);
    /*
     * If we can execute it, do it
     */
    if (sb.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)) 
    {
#ifdef HAVE_RLIMIT
        setrlimit (RLIMIT_CORE, &orlp);     /* restore old limit */
#endif /* HAVE_RLIMIT */    
#ifdef NO_SETUID_SHELL
        char * cmdline = (char *) xalloc (MAX_STRING);
            
        snprintf (cmdline, "exec %100s", shell_name);

        if (!strcmp (basename (shell_name), "tcsh"))
        {
            execl ("/bin/sh", "sh", "-c", cmdline, 0);
            fprintf (stderr, "\nexecl (/bin/sh -c %s) failed... errno = %d\n", shell_name, errno);
            fflush (stderr);
        }
#else /* !NO_SETUID_SHELL */

        /*
         * Well-known convention
         */
        if (want_login)
        {
            char * pt;
            
            pt = basename (shell_name);
            shell_arg0 = (char *) xalloc (strlen (pt) + sizeof (char));
            strncpy (shell_arg0, pt, strlen (pt) + sizeof (char));            
            *shell_arg0 = '-';
        }
        else
            shell_arg0 = basename (shell_name);
        
#ifdef DEBUG
        MESSAGE_2 (" shell_name=%s shell_arg0=%s\n", shell_name, shell_arg0);
#endif /* DEBUG */            
        execl (shell_name, shell_arg0, 0);
        fprintf (stderr, "\nexecl (%s) failed... errno = %d\n", shell_name, errno);
        fflush (stderr);
#endif /* NO_SETUID_SHELL */
    }
#ifdef DEBUG
    else
    {
        MESSAGE_1 ("No access to this shell = %s\n", shell_name);
    }
#endif /* DEBUG */
}

