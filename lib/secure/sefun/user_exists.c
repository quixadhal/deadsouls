//      /adm/simul_efun/user_exists.c
//      from the Dead Souls Mudlib
//      returns true if the player exists
//      created by Descartes of Borg 27 july 1993

int user_exists(string str) { return (int)master()->player_exists(str); }
