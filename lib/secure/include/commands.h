#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <dirs.h>

#define CD                    "/cmds/dev/cd"
#define CMD_ADDCLASS          (DIR_SECURE_ADMIN_CMDS "/addclass")
#define CMD_ADDCLASSES        (DIR_SECURE_ADMIN_CMDS "/addclasses")
#define CMD_ADDRACE           (DIR_SECURE_ADMIN_CMDS "/addrace")
#define CMD_ADDRACES          (DIR_SECURE_ADMIN_CMDS "/addraces")
#define CMD_BK                (DIR_SECURE_CREATOR_CMDS "/bk")
#define CMD_CALL              (DIR_SECURE_CREATOR_CMDS "/call")
#define CMD_CED               (DIR_SECURE_CREATOR_CMDS "/ced")
#define CMD_CHANBAN           (DIR_SECURE_ADMIN_CMDS "/chanban")
#define CMD_CHANCREATE        (DIR_SECURE_ADMIN_CMDS "/chancreate")
#define CMD_CHANREMOVE        (DIR_SECURE_ADMIN_CMDS "/chanremove")
#define CMD_CHANUNBAN         (DIR_SECURE_ADMIN_CMDS "/chanunban")
#define CMD_DECRE             (DIR_SECURE_ADMIN_CMDS "/decre")
#define CMD_ENCRE             (DIR_SECURE_ADMIN_CMDS "/encre")
#define CMD_END               (DIR_SECURE_ADMIN_CMDS "/end")
#define CMD_ICP               (DIR_SECURE_ADMIN_CMDS "/icp")
#define CMD_IMC_SERVER_D      (DIR_SECURE_ADMIN_CMDS "/imc2d")
#define CMD_INSTCONFIG        (DIR_SECURE_ADMIN_CMDS "/instconfig")
#define CMD_LIVEUPGRADE       (DIR_SECURE_ADMIN_CMDS "/liveupgrade")
#define CMD_MUDCONFIG         (DIR_SECURE_ADMIN_CMDS "/mudconfig")
#define CMD_LS                (DIR_CREATOR_CMDS "/ls")
#define CMD_LSED              (DIR_SECURE_CREATOR_CMDS "/lsed")
#define CMD_REMOVECLASS       (DIR_SECURE_ADMIN_CMDS "/removeclass")
#define CMD_REMOVECLASSES     (DIR_SECURE_ADMIN_CMDS "/removeclasses")
#define CMD_REMOVERACE        (DIR_SECURE_ADMIN_CMDS "/removerace")
#define CMD_REMOVERACES       (DIR_SECURE_ADMIN_CMDS "/removeraces")
#define CMD_RID               (DIR_SECURE_ADMIN_CMDS "/rid")
#define CMD_SAY               (DIR_PLAYER_CMDS "/say")
#define CMD_SETREBOOT         (DIR_SECURE_ADMIN_CMDS "/setreboot")
#define CMD_SUICIDE           (DIR_SECURE_PLAYER_CMDS "/suicide")
#define CMD_TELL              (DIR_SECURE_PLAYER_CMDS "/tell")
#define CMD_UPDATE            (DIR_SECURE_CREATOR_CMDS "/update")
#define CMD_ROUTER            (DIR_SECURE_ADMIN_CMDS "/router")
#define SU                    (DIR_SECURE_CREATOR_CMDS "/su")
#define CMD_SU                (DIR_SECURE_CREATOR_CMDS "/su")

#endif /* __COMMANDS_H */
