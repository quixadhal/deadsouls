// By Magnafix 10-14-96
// required addition of list_new_posts() in bboard daemon
#include <daemons.h>
#include <lib.h>
#include <daemons.h>

inherit LIB_COMMAND;

int cmd(string str){
    string *boards;
    int x;

    boards = ({ "immortal_board" });
    if(archp(this_player())) boards += ({ "admin_board" });

    for(x = 0; x < sizeof(boards); x++)
        write(BBOARD_D->list_new_posts(boards[x], 1));

    return 1;
}

string GetHelp() {
    return "Syntax: boards\n"
        "This command tells you which boards have unread messages "
        "on them.";
}
