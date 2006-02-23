/*    /daemon/services.c
 *    from the Dead Souls LPC Library
 *    handles intermud services
 *    created by Descartes of Borg 950523
 *    Version: @(#) services.c 1.3@(#)
 *    Last modified: 96/12/14
 */

#ifndef __PACKAGE_SOCKETS__
#error You should not try to load /daemon/services.c without the sockets package
#else

#include <lib.h>
#include <save.h>
#include <network.h>

inherit LIB_DAEMON;

mapping GetServices();

#include "/daemon/services/channel.c"
#include "/daemon/services/emoteto.c"
#include "/daemon/services/error.c"
#include "/daemon/services/finger.c"
#include "/daemon/services/locate.c"
#include "/daemon/services/tell.c"
#include "/daemon/services/who.c"
#include "/daemon/services/auth.c"

static void create() {
    SetSaveFile(SAVE_SERVICES);
    daemon::create();
    SetNoClean(1);
#ifdef SERVICE_UCACHE
    call_out((: eventCompactUcache :), 3600);
#endif
}

int eventDestruct() {
    eventSave();
    return daemon::eventDestruct();
}

mapping GetServices() {
    return ([
#ifdef SERVICE_AUTH
      "auth" : 1,
#endif
#ifdef SERVICE_CHANNEL
      "channel" : 1,
#endif
#ifdef SERVICE_EMOTETO
      "emoteto" : 1,
#endif
#ifdef SERVICE_FILE
      "file" : 1,
#endif
#ifdef SERVICE_FINGER
      "finger" : 1,
#endif
#ifdef SERVICE_LOCATE
      "locate" : 1,
#endif
#ifdef SERVICE_MAIL
      "mail" : 1,
#endif
#ifdef SERVICE_NEWS
      "news" : 1,
#endif
#ifdef SERVICE_TELL
      "tell" : 1,
#endif
#ifdef SERVICE_UCACHE
      "ucache" : 1,
#endif
#ifdef SERVICE_WHO
      "who" : 1,
#endif
#ifdef PORT_FTP
      "ftp" : PORT_FTP,
#endif
#ifdef PORT_HTTP
      "http" : PORT_HTTP,
#endif
#ifdef PORT_NNTP
      "nntp" : PORT_NNTP,
#endif
#ifdef PORT_RCP
      "rcp" : PORT_RCP,
#endif
#ifdef PORT_SMTP
      "smtp" : PORT_SMTP,
#endif
    ]);
}

#endif /* __PACKAGE_SOCKETS__ */
