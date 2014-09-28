#ifndef s_config_h
#define s_config_h
#define DEFAULT_PROMPT           "> "
#define HOST_IP                  "127.0.0.1"
#define MUD_STATUS               "mudlib development"
#define ADMIN_EMAIL              "admin@here"
#define DEBUGGER                 "DEBUG_NAME"
#define LOCAL_NEWS_PREFIX        "nm"
#define MUD_IS_LOCKED            0
#define AUTO_WIZ                 0
#define RESTRICTED_INTERMUD      0
#define DISABLE_INTERMUD         0
#define DISABLE_IMC2             0
#define MINUTES_REBOOT_WARNING   20
#define TIME_TO_RESET            3600
#define RESET_ALL                0
#define GMT_OFFSET               -32000
#define LOCAL_TIME               0
#define EXTRA_TIME_OFFSET        0
#define MAX_LOG_SIZE             150000
#define MAX_NET_DEAD_TIME        1800
#define MIN_HISTORY_SIZE         50
#define MAX_HISTORY_SIZE         100
#define LOGON_TIMEOUT            360
#define IDLE_TIMEOUT             180000
#define LINK_WAIT_PERIOD         1800
#define MAX_PASSWORD_TRIES       3
#define MIN_USER_NAME_LENGTH     2
#define MAX_USER_NAME_LENGTH     15
#define MAX_USER_CAP_NAME_LENGTH 20
#define LOCKED_ACCESS_ALLOWED    ({ "SECURE", "ASSIST", "ELDER", "TEST" })
#define SUPPORTED_CLIENTS       ({ "amcp", "amcp/1.1" })
#define HOUR_LENGTH              1200
#define DAY_LENGTH               20
#define MAX_NEWBIE_LEVEL         0
#define AUTOSAVE_TIME            200
#define MORTAL_POSITIONS         ({ "player", "high mortal", "ambassador" })
#define DEFAULT_GRAVITY          1.0
#define DEFAULT_ITEM_MESSAGE     "You do not see that here."
#define RESET_INTERMUD           0
#define ENGLISH_ONLY             0
#define HUMANS_ONLY              0
#define SEVERABLE_LIMBS          1
#define CLASS_SELECTION          0
#define PINGING_MUDS             ({})
#define PING_INTERVAL            290
#define ENABLE_ENCUMBRANCE       0
#define GLOBAL_MONITOR           0
#define PLAYER_KILL              0
#define NPC_CATCH_TELL_DEBUG     0
#define SAME_IP_MAX              4
#define COMPAT_MODE              1
#define RETAIN_ON_QUIT           1
#define FAST_COMBAT              1
#define MAX_COMMANDS_PER_SECOND  10
#define DEFAULT_PARSING          0
#define CALL_OUT_LOGGING         0
#define EVENTS_LOGGING           0
#define DISABLE_REBOOTS          0
#define BARE_EXITS               1
#define COMMAND_MATCHING         1
#define OBJECT_MATCHING          1
#define LIVEUPGRADE_SERVER       "Dead Souls"
#define CARRIAGE_RETURN          "\r"
#define OLD_STYLE_PLURALS        1
#define NM_STYLE_EXITS           0
#define WEB_SOURCE_IP            "97.107.133.86"
#define WEB_SOURCE_NAME          "dead-souls.net"
#define DESTRUCT_LOGGING         0
#define ROUTER_TESTING           1
#define SEFUN_PLURALIZE          0
#define LOG_REMOTE_CHANS         0
#define LOG_LOCAL_CHANS          1
#define ENABLE_CGI               0
#define WWW_DIR_LIST             0
#define ENABLE_CREWEB            0
#define F_TERMINAL_COLOR         1
#define MAX_CALL_OUTS            750
#define REQUIRE_QUESTING         1
#define AUTO_ADVANCE             1
#define MAX_INVENTORY_SIZE       2048
#define MAX_ATTACKS_PER_HB       40
#define MEMUSE_SOFT_LIMIT        0
#define MEMUSE_HARD_LIMIT        0
#define MAX_USERS                0
#define MIN_PERF                 400
#define MINIMAP                  1
#define WIZMAP                   1
#define GRID                     1
#define ENABLE_INSTANCES         0
#define CED_DISABLED             1
#define PLAYER_INTERTELL_ALLOWED 0
#define GUEST_ALLOWED            1

#endif /* s_config_h */
