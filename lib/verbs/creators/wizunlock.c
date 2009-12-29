#include <lib.h>
#include <daemons.h>
#include <damage_types.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("wizunlock");
    SetRules("OBJ");
    SetErrorMessage("wizunlock what?");
    SetHelp("Syntax: wizunlock <object>\n\n"
            "Magically unlock a lockable thing without needing a key.\n"
            "See also: wizunlock");
}

    mixed can_wizunlock_obj(string str) { 
        if(!creatorp(this_player()))
            return "This command is only available to creators.";
        else return 1;
    }

mixed do_wizunlock_obj(object ob){
    string name;

    if(living(ob)) {
        write("You can only wizunlock non-living things.");
        return 1;
    }
    else name = ob->GetShort();
    if(ob->GetDoor()) ob = load_object(ob->GetDoor());
    write("You wizunlock "+name+".");
    say(this_player()->GetName()+" makes an open-hand gesture at"+
            " "+name+".",({ob}));
    ob->SetLocked(0);
    return 1;
}
