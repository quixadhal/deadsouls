#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("ride");
    SetRules("STR", "into STR" );
    SetErrorMessage("Ride in which direction?");
    SetHelp("Syntax: ride <DIRECTION>\n"
            "        ride into <PLACE>\n\n"
            "Moves your mount towards the direction you specify, or "
            "into the place "
            "you specify.  The command \"ride into\" is synonymous with the "
            "\"enter\" command.\n"
            "See also: mount, dismount");
}

mixed can_ride_str(string str) {
    object mount = environment(this_player());
    object where;

    if(mount) where = environment(mount); 
    if(mount && !mount->GetMount()) return "You are not mounted.";

    if( !mount ) return "You are nowhere.";
    if( !where ) return "Your mount is nowhere.";
    if( mount->GetStaminaPoints() <3 )
        return "Your mount is too tired to go anywhere right now.";
    if(!stringp(hobbled(mount))) return "Your mount is incapacitated.";
    if(str) switch(str){
        case "n" : str = "north"; break;
        case "s" : str = "south"; break;
        case "e" : str = "east"; break;
        case "w" : str = "west"; break;
        case "u" : str = "up"; break;
        case "d" : str = "down"; break;
        case "ne" : str = "northeast"; break;
        case "nw" : str = "northwest"; break;
        case "se" : str = "southeast"; break;
        case "sw" : str = "southwest"; break;
    }
    return where->CanGo(mount, str);
}

mixed can_ride_into_str(string str) {
    object mount = environment(this_player());
    object where;

    if(mount) where = environment(mount); 
    if(mount && !mount->GetMount()) return "You are not mounted.";

    if( !mount ) return "You are nowhere.";
    if( !where ) return "Your mount is nowhere.";
    if( mount->GetStaminaPoints() <3 )
        return "Your mount is too tired to go anywhere right now.";
    if(!stringp(hobbled(mount))) return "Your mount is incapacitated.";
    return where->CanEnter(mount, str);
}

mixed do_ride_str(string str) {
    object mount = environment(this_player());
    if(str) switch(str){
        case "n" : str = "north"; break;
        case "s" : str = "south"; break;
        case "e" : str = "east"; break;
        case "w" : str = "west"; break;
        case "u" : str = "up"; break;
        case "d" : str = "down"; break;
        case "ne" : str = "northeast"; break;
        case "nw" : str = "northwest"; break;
        case "se" : str = "southeast"; break;
        case "sw" : str = "southwest"; break;
    }
    return mount->eventRide(str);
}

mixed do_ride_into_str(string str) {
    object mount = environment(this_player());
    return mount->eventRide("into" + str);
}
