/** conf.h
 **
 ** Fichier de configuration pour calife.c
 **
 ** cree pour simplifier calife.c
 **
 ** Copyright (c) 1991-2010 par O. ROBERT
 **
 ** @(#) $Id: conf.h,v b0fe97d87286 2014/03/20 11:17:33 roberto $
 **/

#ifndef CONF_H                  /* evite les includes multiples */
#define CONF_H

#define ROOT_LOGIN      "root"

#ifdef SUNOS4
#define _PATH_UTMP  "/etc/utmp" /* sigh */
#endif

                                /* for Gould NP1 */
#ifdef GOULD
#define BSD
#endif

#if (defined(BSD) && (BSD >= 199306))   /* for both NetBSD & FreeBSD */
#define ADMIN_LOG   "/var/log/calife"
#define HAVE_SYSCONF
#endif /* __386BSD__ */


#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H */

#if defined(HAVE_SYS_WAIT_H) || defined(HAVE_NON_POSIX_WAIT_H)
#include <sys/wait.h>
#endif /* HAVE_SYS_WAIT_H && HAVE_NON_POSIX_WAIT_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif /* HAVE_SYSLOG_H */

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif /* HAVE_SYS_TIME_H */

#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#if defined (SUNOS4) || defined (__linux__)
# include <sys/time.h>
# include <utmp.h>
#endif /* SunOS && __linux__ */

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif /* HAVE_SHADOW_H */

#include <errno.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */

#ifdef NEED_STRINGS_H
#include <strings.h>
#endif /* NEED_STRINGS_H */

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */

#ifdef HAVE_RLIMIT
#include <sys/resource.h>
#endif /* HAVE_RLIMIT */

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif /* HAVE_LIBGEN_H */

                                /* A verifier */
#ifdef BSD
#if !defined(__FreeBSD__) && !defined(HAVE_SYS_WAIT_H)
#define WEXITSTATUS(x)  ((x).w_retcode)
#define WCOREDUMP(x)    ((x).w_coredump)
#define WTERMSIG(x)     ((x).w_termsig)
#endif
#endif

/*
 * Number of attempts the code will try to verify the password
 */
#define MAX_ATTEMPTS    3

#ifdef WITH_PAM
/*
 * the following code is stolen from imap-uw PAM authentication module and
 * login.c
 */
#define COPY_STRING(s) (s ? strdup(s) : NULL)

struct cred_t {
	const char *uname;		/* user name */
	const char *pass;		  /* password */
};
typedef struct cred_t cred_t;

#ifdef HAVE_PAM_PAM_APPL_H
#include <pam/pam_appl.h>
#else
#include <security/pam_appl.h>
#endif
#endif /* WITH_PAM */

/*
 * Try to simplify the code
 */
#include <assert.h>
#ifdef HPUX
#  define GET_ROOT { \
                        int e = setresuid(-1,ssid,-1); \
                        assert(e == 0); \
                   }
#else
#  define GET_ROOT { \
                        int e = seteuid(ssid); \
                        assert(e == 0); \
                   }
#endif /* HPUX */

#ifdef HPUX
#  define RELEASE_ROOT { \
                            int e = setresuid(-1,getuid (),-1);
                            assert(e == 0); \
                       }
#else
#  define RELEASE_ROOT { \
                            int e = seteuid(getuid ()); \
                            assert(e == 0); \
                       }
#endif /* HPUX */

#define MAX_STRING  1024        /* "safe" value */

#ifndef MAXLOGNAME
#define MAXLOGNAME  16
#endif

#define ACCEPT_ALPHAL   "abcdefghijklmnopqrstuvwxyz"
#define ACCEPT_ALPHAU   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ACCEPT_NUM      "0123456789"
#define ACCEPT_PUNCT    "-_"

#define ACCEPT_CHARS    ACCEPT_ALPHAL \
                        ACCEPT_ALPHAU \
                        ACCEPT_NUM \
                        ACCEPT_PUNCT

#define AUTH_OK    0
#define AUTH_NULL  1
#define AUTH_ERR   2
#define AUTH_BADP  3

  int open_databases (void);
  void authenticate_user (char *, char *, char *, char *);
  int verify_auth_info (char *, char *);
  void exec_shell (char *);
# ifndef HAVE_BASENAME
  char * basename (char * file_name);
# endif /* HAVE_BASENAME */
  void * xalloc (size_t size);
  void die (int err, const char * fmt,...);
#ifdef WITH_PAM
  int  auth_pam(struct passwd **ppw, const char *pass);
#endif

#ifndef MAIN_MODULE
extern  int     custom_shell;   /* modification du shell ? */
extern  char    * shell;        /* nom du shell */
extern  uid_t   ssid;           /* POSIX saved uid */
extern  char    * _group;       /* Si user E group */
#ifdef WITH_PAM
extern  pam_handle_t	*pamh;
#endif /* WITH_PAM */
#endif /* !MAIN_MODULE */

extern  int errno;

#ifdef DEBUG
#define MESSAGE(x) fprintf (stderr, (x)); \
                   fflush (stderr)
#define MESSAGE_1(x,y) fprintf (stderr, (x), (y)); \
                       fflush (stderr)
#define MESSAGE_2(x,y,z) fprintf (stderr, (x), (y), (z)); \
                         fflush (stderr)
#define MESSAGE_3(x,y,z,t) fprintf (stderr, (x), (y), (z), (t)); \
                           fflush (stderr)
#define MESSAGE_4(x,y,z,t,u) fprintf (stderr, (x), (y), (z), (t), (u)); \
                             fflush (stderr)
#define MESSAGE_5(x,y,z,t,u,v) fprintf (stderr, (x), (y), (z), (t), \
                                                (u), (v)); \
                               fflush (stderr)
#else
#define MESSAGE(x)
#define MESSAGE_1(x,y)
#define MESSAGE_2(x,y,z)
#define MESSAGE_3(x,y,z,t)
#define MESSAGE_4(x,y,z,t,u)
#define MESSAGE_5(x,y,z,t,u,v)
#endif /* DEBUG */

#ifdef NO_SYSLOG
#define ADMIN_LOG   "/var/log/calife.log"
#endif /* NO_SYSLOG */

#endif /* CONF_H */
