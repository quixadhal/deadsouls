#include <lib.h>
#include <modules.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string arg;
    object staff;
    int room;

    if(!(PLAYERS_D->CheckBuilder(this_player()))){
        write("This command is for builders and creators.");
        return 1;
    }

    if(!str || str == "") {
        write("You'll need to be more specific. Try 'help create'");
        return 1;
    }

    staff = present("tanstaafl",this_player());
    if(!staff) {
        write("You must be holding the creator staff in order to use this command.");
        write("If you don't know where you put it, get another one from the chest ");
        write("in your workroom.");
        return 1;
    }

    if(sscanf(str,"room%s",arg) == 1 ||
            sscanf(str,"enter%s",arg) == 1 ||
            sscanf(str,"exit%s",arg) == 1 ) room = 1;

    if(room) staff->make(arg);
    else staff->gmake(str);

    return 1;
}

string GetHelp() {
    return ("Syntax:\n"
            "        create room <DIRECTION> <FILE>\n"
            "        create room none <FILE>\n"
            "        create npc <FILE>\n"
            "        create barkeep <FILE>\n"
            "        create vendor <FILE>\n"
            "        create trainer <FILE>\n"
            "        create armor <FILE>\n"
            "        create worn_storage <FILE>\n"
            "        create weapon <FILE>\n"
            "        create item <FILE>\n"
            "        create container <FILE>\n"
            "        create bed <FILE>\n"
            "        create chair <FILE>\n"
            "        create table <FILE>\n"
            "        create book <FILE>\n"
            "        create door <DIRECTION> <FILE>\n"
            "\nThis command makes a generic copy of the type of thing "
            "you specify. In the case of a room, the room you are in is "
            "copied into the direction you specify. In the case of other "
            "objects, a generic object appears in the room you are in. "
            "After that object materializes, you can make changes to it "
            "with the \"modify\" command. These changes are saved "
            "automatically.\n\n"
            "See also: modify, copy, delete, add, initfix");
}
