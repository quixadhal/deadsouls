#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <dirs.h>

#define CMD_CALL              ("/secure/cmds/creator/call")
#define CMD_ENCRE             (DIR_SECURE_ADMIN_CMDS "/encre")
#define CMD_END               (DIR_ADMIN_CMDS+"/end")
#define CMD_DECRE             (DIR_SECURE_ADMIN_CMDS "/decre")
#define CMD_SAY			(DIR_PLAYER_CMDS "/say")
#define CMD_SETREBOOT         (DIR_SECURE_ADMIN_CMDS+"/setreboot")
#define CMD_SUICIDE           (DIR_MORTAL_CMDS+"/suicide")
#define CD "/cmds/dev/cd"
#define SU "/cmds/user/su"

#endif /* __COMMANDS_H */
