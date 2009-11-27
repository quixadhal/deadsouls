#include <lib.h>
#include <position.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    int pos;  
    object ob;
    string ret;

    if(str) ob = get_object(str);
    if(!ob || !living(ob)){
        if(str){
            write("No such living thing found.");
            return 1;
        }
        ob = this_player();
        ret = "You are ";
    }
    else {
        ret = ob->GetShort()+" is ";
    }

    pos = ob->GetPosition();

    switch(pos){
        case POSITION_STANDING : ret += "standing.";break;
        case POSITION_SITTING : ret += "sitting.";break;
        case POSITION_LYING : ret += "prone.";break;
        case POSITION_FLYING : ret += "flying.";break;
        case POSITION_KNEELING : ret += "kneeling.";break;
        case POSITION_FLOATING : ret += "floating.";break;
        case POSITION_SWIMMING : ret += "swimming.";break;
        default : ret += "here.";
    }

    write(ret);
    return 1;
}

string GetHelp() {
    return ("Syntax: position [living thing]\n\n"
            "Reports the posture of the person named, or if no argument is "
            "provided, your own physical posture.");
}
