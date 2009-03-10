#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <dirs.h>

#define CD                    "/cmds/dev/cd"
#define CMD_CALL              (DIR_SECURE_CREATOR_CMDS "/call")
#define CMD_DECRE             (DIR_SECURE_ADMIN_CMDS "/decre")
#define CMD_ENCRE             (DIR_SECURE_ADMIN_CMDS "/encre")
#define CMD_END               (DIR_SECURE_ADMIN_CMDS "/end")
#define CMD_IMC_SERVER_D      (DIR_SECURE_ADMIN_CMDS "/imc2d")
#define CMD_LIVEUPGRADE       (DIR_SECURE_ADMIN_CMDS "/liveupgrade")
#define CMD_LS                (DIR_CREATOR_CMDS "/ls")
#define CMD_SAY               (DIR_PLAYER_CMDS "/say")
#define CMD_SETREBOOT         (DIR_SECURE_ADMIN_CMDS "/setreboot")
#define CMD_SUICIDE           (DIR_SECURE_PLAYER_CMDS "/suicide")
#define CMD_TELL              (DIR_SECURE_PLAYER_CMDS "/tell")
#define CMD_UPDATE            (DIR_SECURE_CREATOR_CMDS "/update")
#define CMD_ROUTER            (DIR_SECURE_ADMIN_CMDS "/router")
#define SU                    (DIR_SECURE_CREATOR_CMDS "/su")
#define CMD_SU                (DIR_SECURE_CREATOR_CMDS "/su")

#endif /* __COMMANDS_H */
