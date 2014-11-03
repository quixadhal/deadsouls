#include <lib.h>
#include <daemons.h>
#include <damage_types.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("wizlock");
    SetRules("OBJ");
    SetErrorMessage("wizlock what?");
    SetHelp("Syntax: wizlock <object>\n\n"
            "Magically lock a lockable thing without needing a key.\n"
            "See also: wizunlock");
}

mixed can_wizlock_obj(string str) { 
    if(!creatorp(this_player())) 
        return "This command is only available to creators.";
    else return 1;
}

mixed do_wizlock_obj(object ob){
    string name;
    if(living(ob)) {
        write("You can only wizlock non-living things.");
        return 1;
    }
    else name = ob->GetShort();
    if(ob->GetDoor()) ob = load_object(ob->GetDoor());
    if(!ob->GetClosed()){
        write("You can't wizlock it because it is not closed.");
        return 1;
    } 
    write("You wizlock "+name+".");
    say(this_player()->GetName()+" makes a closed-fist gesture at"+
            " "+name+".",({ob}));
    ob->SetLocked(1);
    return 1;
}
