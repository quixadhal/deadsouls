#ifndef s_config_h
#define s_config_h

#define DEFAULT_PROMPT           "> "
#define MUD_STATUS               "mudlib development"
#define ADMIN_EMAIL              "USER@host"
#define LOCAL_NEWS_PREFIX        "nm"

#define MUD_IS_LOCKED            0
#define MINUTES_REBOOT_WARNING   20
#define TIME_TO_RESET            1000

#define GMT_OFFSET               -32000
#define MAX_LOG_SIZE             150000
#define MAX_NET_DEAD_TIME        1800
#define MIN_HISTORY_SIZE         10
#define MAX_HISTORY_SIZE         30

#define LOGON_TIMEOUT            360
#define LINK_WAIT_PERIOD         1800
#define MAX_PASSWORD_TRIES       3
#define MIN_USER_NAME_LENGTH     2
#define MAX_USER_NAME_LENGTH     15
#define MAX_USER_CAP_NAME_LENGTH 20
#define LOCKED_ACCESS_ALLOWED    ({ "SECURE", "ASSIST" })
#define SUPPORTED_CLIENTS       ({ "amcp", "amcp/1.1" })

#define HOUR_LENGTH              1200
#define DAY_LENGTH               20

#define MAX_NEWBIE_LEVEL         5
#define AUTOSAVE_TIME            450
#define MORTAL_POSITIONS         ({ "player", "high mortal", "ambassador" })

#define DEFAULT_GRAVITY          1.0
#define DEFAULT_ITEM_MESSAGE     "You do not see that here."

#endif /* s_config_h */
