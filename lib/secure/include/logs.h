#ifndef s_logs_h
#define s_logs_h

#include <dirs.h>
#include CONFIG_H

#if ENABLE_INSTANCES
#define UNIQ_EXT "." + __PORT__
#else
#define UNIQ_EXT ""
#endif

/* Network stuff */
#define LOG_HTTP          DIR_NETWORK_LOGS "/http" UNIQ_EXT
#define LOG_IMC2          DIR_INTERMUD_LOGS "/imc2" UNIQ_EXT
#define LOG_I3            DIR_SECURE_INTERMUD_LOGS "/i3" UNIQ_EXT
#define LOG_I3ERR         DIR_SECURE_INTERMUD_LOGS "/i3err" UNIQ_EXT
#define LOG_INET          DIR_NETWORK_LOGS "/inet" UNIQ_EXT
#define LOG_ROUTER        DIR_INTERMUD_LOGS "/router" UNIQ_EXT
#define LOG_RSS           DIR_NETWORK_LOGS "/rss" UNIQ_EXT

/* Other stuff */
#define LOG_BANISH        DIR_LOGS "/banish" UNIQ_EXT
#define LOG_BUGS          DIR_LOGS "/bugs" UNIQ_EXT
#define LOG_EVENTS        DIR_LOGS "/events" UNIQ_EXT
#define LOG_LIVEUPGRADE   DIR_SECURE_LOGS "/liveupgrade" UNIQ_EXT
#define LOG_QUESTS        DIR_SECURE_LOGS "/quests" UNIQ_EXT
#define LOG_RESET         DIR_LOGS "/reset" UNIQ_EXT
#define LOG_ICP           DIR_SECURE_LOGS "/icp" UNIQ_EXT

#endif /* s_logs_h */
