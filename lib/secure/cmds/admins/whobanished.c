//      /bin/adm/_whobanished.c
//      from the Dead Souls Mudlib
//      lists all names banished from usage
//      originally written by Shadowwolf@Dead Souls 
//      rewritten for the new Dead Souls security system by Descartes of Borg
//      930809

#include <lib.h>
#include <daemons.h>

int cmd(string str) {
    string *noms;

    if(!archp(previous_object())) return 0;
    noms = (string *)BANISH_D->query_banished();
    if(str) {
        if(member_array(lower_case(str), noms) == -1) 
          write(str+" is not currently a banished name.\n");
        else write(str+" is a banished name.\n");
    }
    else {
        write("These names are currently banished:\n");
        this_player()->more(explode(format_page(noms, 4), "\n"));
    }
    return 1;
}

void help() {
    write("Syntax: <whobanished ([name])>\n\n"
      "Without an argument, this command lists all names currently\n"
      "listed as being banished.  Otherwise it will confirm whether\n"
      "the name is banished or not.\n"
    );
}
