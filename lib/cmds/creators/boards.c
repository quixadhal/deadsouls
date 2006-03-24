// By Magnafix 10-14-96
// required addition of list_new_posts() in bboard daemon
#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;


int cmd(string str){
    string *boards;
    int x;

    boards = ({ "immortal_board" });
    if(archp(this_player())) boards += ({ "admin_board" });

    for(x = 0; x < sizeof(boards); x++)
	write("/secure/daemon/bboard.c"->list_new_posts(boards[x]));

    return 1;
}



string GetHelp(string str) {
    return "Syntax: <bb>\n"
    "This command tells you which boards have unread messages "
    "on them.";
}
