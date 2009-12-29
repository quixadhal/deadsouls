//      /bin/dev/_banish.c
//      from the Dead Souls Mudlib
//      prevents a name being used by a new player
//      created by Descartes of Borg ???

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    int substr = 0;

    if(!str || str == ""){
        write("Syntax: banish <string>");
        return 1;
    }

    if(grepp(str,"-s ")){
        str = replace_string(str,"-s ","");
        substr = 1;
    }
    if(!user_exists(str = lower_case(str))) {
        if(!substr){
            BANISH_D->banish_name(str);
            write(capitalize(str)+" is now banished.\n");
        }
        else {
            BANISH_D->set_illegal_substring(str);
            write("The substring \""+str+"\" is now illegal in a name.");
        }
    }
    else {
        write("A player by that name already exists.\n");
        this_player()->eventPrint(FINGER_D->GetFinger(str));
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: banish [-s] <name | word>\n\n"
            "Protects a name from being used by a new player.\n"
            "For example, if you have created a monster named Cassandra, in order\n"
            "to avoid problems with player complaining \"I typed 'kill\n"
            "cassandra' meaning to kill the evil enchantress, but Cassandra\n"
            "walked in and I accidentally killed her.\"  It is also to be\n"
            "used to keep people from using offensive words as names.\n"
            "The -s option makes the argument a substring to be made illegal, "
            "so that:\n"
            "banish -s top\n"
            "would make it impossible to create a character named Carrottop."
           );
}
