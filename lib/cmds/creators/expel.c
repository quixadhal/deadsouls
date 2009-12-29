#include <lib.h>
#include ROOMS_H

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob, *obs;

    if( !args || args == "" ) return "Expel whom?";
    ob = present(args,environment(this_player()));
    if(args != "all" && (!ob || !living(ob))){
        return "Expel only works for living things in your environment.";
    }
    if(archp(ob) && !archp(this_player())){
        write("You can't expel an admin.");
        tell_player(ob, this_player()->GetName()+" just tried to expel you.");
        return 1;
    }

    if(args == "all"){
        if(archp(this_player())) {
            obs = filter(get_livings(environment(this_player())), (: $1 != this_player() :) );
        }
        else {
            obs = filter(get_livings(environment(this_player())), 
                    (: $1 != this_player() && !archp($1) :) );
        }
    }

    else obs = ({ ob });

    foreach(object nuisance in obs){
        nuisance->eventWimpy(1);
    }

    foreach(object nuisance in obs){
        if(environment(nuisance) == environment(this_player())){
            if(creatorp(nuisance)) {
                nuisance->eventMoveLiving(homedir(nuisance)+"/workroom");
            }
            else nuisance->eventMoveLiving(ROOM_START);
        }
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: expel <living> \n\n"
            "Forces the specified living thing to leave your environment.\n"
            "See also: return, goto, move, trans");
}
