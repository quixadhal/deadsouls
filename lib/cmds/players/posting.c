//  Command _posting.c
//  Written by Hanse@Dead Souls 6/21/93

#include <lib.h>
inherit LIB_DAEMON;

int cmd(string str) {
    object *list;
    int i;

    if(str) return 0;
    list=users();
    write("\nList of users in various editors:\n");
    for(i=0;i<sizeof(list);i++)
        if(!list[i]->GetInvis() && list[i]->GetKeyName())
            if(present("mailer", list[i]))
                printf("%s - In mailer.\n",
                        arrange_string( list[i]->GetName(), 20));
            else if(in_edit(list[i]) || in_input(list[i]))
                printf("%s - Editing.\n",
                        arrange_string( list[i]->GetName(), 20));
    return 1;
}

string GetHelp() {
    return "Syntax: posting\n\n"
        "Displays whether a user is in editor, posting on a "
        "bulletin board, or in the mailer.";
}
