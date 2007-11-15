

#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include <rooms.h>
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("resurrect");
    SetRules("OBJ", "here");
    SetErrorMessage("resurrect what?");
    SetHelp("Syntax: <resurrect OBJ>\n\n"
      "Bring back to life a dead player or creator.\n"
      "\nSee also: zap");
}

mixed can_resurrect_obj(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}


mixed do_resurrect_obj(object ob) {
    if(!interactive(ob)) {
        write("You may only resurrect dead players.");
        return 1;
    }

    if(!ob->GetGhost()) {
        write("You can't resurrect the living.");
        return 1;
    }

    if(environment(ob) != environment(this_player())) {
        write(capitalize(ob->GetKeyName())+" isn't here.");
        return 1;
    }

    tell_player(this_player(),"You wave your hand, and with a flash "+
      "of light, "+ob->GetCapName()+" comes back to life!");
    tell_player(ob,capitalize(this_player()->GetKeyName())+" waves "+
      possessive(this_player())+
      " hand, and with a flash of light, you come back from the dead!");
    tell_room(environment(this_player()),this_player()->GetCapName()+" waves "+
      possessive(this_player())+
      " hand, and with a flash of light, "+ob->GetCapName()+" comes back to life!",
      ({ob, this_player()}) );
    ob->eventRevive();
    ob->eventMoveLiving(ROOM_START);
    return 1;
}

