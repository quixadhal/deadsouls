#ifndef s_logs_h
#define s_logs_h

#include <dirs.h>

/* Network stuff */
#define LOG_HTTP	  DIR_NETWORK_LOGS "/http"
#define LOG_IMC2	  DIR_INTERMUD_LOGS "/imc2"
#define LOG_I3   	  DIR_SECURE_INTERMUD_LOGS "/i3"
#define LOG_I3ERR   	  DIR_SECURE_INTERMUD_LOGS "/i3err"
#define LOG_INET          DIR_NETWORK_LOGS "/inet"
#define LOG_ROUTER        DIR_INTERMUD_LOGS "/router"
#define LOG_RSS           DIR_NETWORK_LOGS "/rss"

/* Other stuff */
#define LOG_BANISH        DIR_SECURE_LOGS "/banish"
#define LOG_BUGS          DIR_LOGS "/bugs"
#define LOG_EVENTS        DIR_LOGS "/events"
#define LOG_LIVEUPGRADE   DIR_SECURE_LOGS "/liveupgrade"
#define LOG_QUESTS        DIR_SECURE_LOGS "/quests"

#endif /* s_logs_h */
