#ifndef s_message_class_h
#define s_message_class_h

#define MSG_NOBLOCK  (1 << 0)                 /* no blocking */
#define MSG_NOCOLOUR (1 << 1)                 /* no colours */
#define MSG_NOWRAP   (1 << 2)                 /* don't wrap this message */

#define MSG_CONV     (1 << 3)                              /* conversation */
#define MSG_EDIT     ((1 << 4) | MSG_NOBLOCK | MSG_NOCOLOUR | MSG_NOWRAP)
                                                           /* editor */
#define MSG_ENV      (1 << 5)                              /* general stuff */
#define MSG_ERROR    ((1 << 6) | MSG_NOBLOCK)              /* error messages */
#define MSG_HELP     ((1 << 7) | MSG_NOBLOCK)              /* help messages */
#define MSG_ROOMDESC ((1 << 8) | MSG_NOBLOCK)              /* room descs */
#define MSG_PROMPT   ((1 << 9) | MSG_NOBLOCK | MSG_NOWRAP) /* prompts */
#define MSG_SYSTEM   ((1 << 10) | MSG_NOBLOCK)             /* sys messages */

/* client specific messages */
#define MSG_LOGIN    (1 << 11)                      /* prot. neg. for login */
#define MSG_MSG_BOX  (1 << 12)                      /* display msg box */
#define MSG_QUERY    (1 << 13)                      /* response to client */


/* client command types */
#define CMD_LOGIN    (1 << 0)                       /* login */
#define CMD_USER     (1 << 1)                       /* normal user cmds */
#define CMD_QUERY    (1 << 2)                       /* client query */

#endif /* s_message_class_h */
