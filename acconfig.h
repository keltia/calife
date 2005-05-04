/* acconfig.h
 *
 * Contains calife's specific defines
 *
 * $Id: //depot/security/calife/main/acconfig.h#4 $
 */

/* Define if you have the <string.h> header file.  */
#undef HAVE_STRING_H

/* Define if you have the <stdarg.h> header file.  */
#undef HAVE_STDARG_H

/* Define if you have the <sys/param.h> header file.  */
#undef HAVE_SYS_PARAM_H

/* Define if you have the <libgen.h> header file.  */
#undef HAVE_LIBGEN_H

/* Define if you have the <syslog.h> header file.  */
#undef HAVE_SYSLOG_H

/* Define if you have the crypt library (-lcrypt).  */
#undef HAVE_LIBCRYPT

/* Define if you want POSIX signals */
#undef HAVE_POSIX_SIGNALS

/* Define if you're under SCO */
#undef SCO

/* Define it if you have SCO's /etc/shadow file */
#undef HAVE_SCO_ETC_SHADOW

/* Define it if you have an /etc/shadow file */
#undef HAVE_ETC_SHADOW

/* Define if you don't want to type a password at all */
#undef RELAXED

/* Define if your syslog.h is in sys/ */
#undef NEED_SYS_SYSLOG_H

/* Define if you're using OSF C2 security package */
#undef HAVE_OSF1_C2_SECURITY

/* Define if you have setrlimit/getrlimit */
#undef HAVE_RLIMIT

/* Define if no syslog-type functions exist */
#undef NO_SYSLOG

/* Define if _SC_LINE_MAX is defined for sysconf */
#undef HAVE_WORKING_SYSCONF

/* Define it to su' path */
#undef SU_CMD

/* Define it if you want debugging output */
#undef DEBUG

/* Define it if you have a setuid-aware shell */
#undef NO_SETUID_SHELL

/* Define it if you have <sys/wait.h> but a non-POSIX one like NeXTStep */
#undef HAVE_NON_POSIX_WAIT_H

/* Define it if don't want to use getspnam(3) and friends-- Linux */
#undef UNUSED_SHADOW

/* Define if you want a global /etc/calife.out script */
#undef WANT_GLOBAL_RC

/* Define if we want to use PAM authentication */
#undef WITH_PAM
