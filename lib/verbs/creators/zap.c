#include <lib.h>
#include <daemons.h>
#include <damage_types.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("zap");
    SetRules("LVS");
    SetErrorMessage("zap what?");
    SetHelp("Syntax: zap <CREATURE>\n\n"
            "Deal massive damage to a living thing.\n"
            "See also: dest, resurrect");
}

mixed can_zap_liv(string str) { 
    if(!creatorp(this_player()))
        return "This command is only available to creators.";
    else return 1;
}

mixed do_zap_liv(object ob){
    string name;
    int mhp;

    if(!living(ob)) {
        write("You can only zap living things.");
        return 1;

    }
    else name = ob->GetName();
    mhp = ob->GetMaxHealthPoints();
    if(!mhp) mhp = 99999;
    mhp *= 5;
    write("You zap "+name+".");
    say(this_player()->GetName()+" raises a hand and %^RED%^ZAPS%^RESET%^"+
            " "+name+"!",({ob}));
    tell_object(ob,this_player()->GetName()+" raises a hand and "+
            "%^RED%^ZAPS%^RESET%^ you!");
    ob->eventReceiveDamage(this_player(),DEATHRAY,mhp,0,({ob->GetTorso()}));
    return 1;
}

mixed do_zap_lvs(object *obs) {
    foreach(object ob in obs){
        if(!(ob == this_player())) do_zap_liv(ob);
    }
    return 1;
}
