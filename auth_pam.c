/* auth_pam.c
 *
 * Shamelessly slightly rewritten pamize.h from FreeBSD
 */
/* $FreeBSD: src/libexec/lukemftpd/pamize.h,v 1.1 2003/02/02 21:06:10 obrien Exp $ */

#ifndef lint
static const char * rcsid = "@(#) $Id: auth_pam.c,v 88c4f70961ac 2008/08/19 15:34:11 roberto $";
#endif

#include <config.h>

#include "conf.h"

#ifdef WITH_PAM
/*
 * the following code is stolen from imap-uw PAM authentication module and
 * login.c
 */

static int
auth_conv(int num_msg, const struct pam_message **msg,
      struct pam_response **resp, void *appdata)
{
    int i;
    cred_t *cred = (cred_t *) appdata;
    struct pam_response *reply;

    reply = calloc(num_msg, sizeof *reply);
    if (reply == NULL)
        return PAM_BUF_ERR;

    for (i = 0; i < num_msg; i++) {
        switch (msg[i]->msg_style) {
        case PAM_PROMPT_ECHO_ON:    /* assume want user name */
            reply[i].resp_retcode = PAM_SUCCESS;
            reply[i].resp = COPY_STRING(cred->uname);
            /* PAM frees resp. */
            break;
        case PAM_PROMPT_ECHO_OFF:   /* assume want password */
            reply[i].resp_retcode = PAM_SUCCESS;
            reply[i].resp = COPY_STRING(cred->pass);
            /* PAM frees resp. */
            break;
        case PAM_TEXT_INFO:
        case PAM_ERROR_MSG:
            reply[i].resp_retcode = PAM_SUCCESS;
            reply[i].resp = NULL;
            break;
        default:            /* unknown message style */
            free(reply);
            return PAM_CONV_ERR;
        }
    }

    *resp = reply;
    return PAM_SUCCESS;
}

/*
 * Attempt to authenticate the user using PAM.  Returns 0 if the user is
 * authenticated, or 1 if not authenticated.  If some sort of PAM system
 * error occurs (e.g., the "/etc/pam.conf" file is missing) then this
 * function returns -1.  This can be used as an indication that we should
 * fall back to a different authentication mechanism.
 */
int
auth_pam(struct passwd **ppw, const char *pass)
{
    const char *tmpl_user;
    const void *item;
    int rval;
    int e;
    cred_t auth_cred = { (*ppw)->pw_name, pass };
    struct pam_conv conv = { &auth_conv, &auth_cred };

    e = pam_start("calife", (*ppw)->pw_name, &conv, &pamh);
    if (e != PAM_SUCCESS) {
        syslog(LOG_AUTH | LOG_ERR, "pam_start: %s", pam_strerror(pamh, e));
        return -1;
    }
    MESSAGE_1("pam_start succeeded for %s\n", (*ppw)->pw_name);

    /* pam_authenticate may require root privs to obtain shadow             
       password data */                                                     
    GET_ROOT;                                                               
    e = pam_authenticate(pamh, 0);                                          
    RELEASE_ROOT;                                                           
    switch (e) {
    case PAM_SUCCESS:
        /*
         * With PAM we support the concept of a "template"
         * user.  The user enters a login name which is
         * authenticated by PAM, usually via a remote service
         * such as RADIUS or TACACS+.  If authentication
         * succeeds, a different but related "template" name
         * is used for setting the credentials, shell, and
         * home directory.  The name the user enters need only
         * exist on the remote authentication server, but the
         * template name must be present in the local password
         * database.
         *
         * This is supported by two various mechanisms in the
         * individual modules.  However, from the application's
         * point of view, the template user is always passed
         * back as a changed value of the PAM_USER item.
         */
        if ((e = pam_get_item(pamh, PAM_USER, &item)) == PAM_SUCCESS)
        {
            tmpl_user = (const char *) item;
            if (strcmp((*ppw)->pw_name, tmpl_user) != 0)
                *ppw = getpwnam(tmpl_user);
        } else
            syslog(LOG_AUTH | LOG_ERR, "Couldn't get PAM_USER: %s",
                                       pam_strerror(pamh, e));
        rval = 0;
        break;

    case PAM_AUTH_ERR:
    case PAM_USER_UNKNOWN:
    case PAM_MAXTRIES:
        syslog(LOG_AUTH | LOG_ERR, "auth_pam: %s", pam_strerror(pamh, e));
        rval = 1;
        break;

    default:
        syslog(LOG_AUTH | LOG_ERR, "pam_authenticate: %s", pam_strerror(pamh,\
                                                                        e));
        rval = -1;
        break;
    }

/* XXX this does not work on solaris10 */
#ifndef solaris
    if (rval == 0) {
        e = pam_acct_mgmt(pamh, 0);
        if (e == PAM_NEW_AUTHTOK_REQD) {
            e = pam_chauthtok(pamh, PAM_CHANGE_EXPIRED_AUTHTOK);
            if (e != PAM_SUCCESS) {
                syslog(LOG_AUTH | LOG_ERR, "pam_chauthtok: %s",
                    pam_strerror(pamh, e));
                rval = 1;
            }
        } else if (e != PAM_SUCCESS) {
            rval = 1;
        }
    }
#endif /* solaris */

    MESSAGE_1 ("auth_pam returns %d\n", rval);
    return rval;
}

#endif /* WITH_PAM */
