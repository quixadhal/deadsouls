#include <lib.h>
#include <daemons.h>
#include <damage_types.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("pulsecheck");
    SetRules("LVS");
    SetErrorMessage("pulsecheck what?");
    SetHelp("Syntax: pulsecheck <CREATURE>\n\n"
            "Check a creature's heartbeat.\n"
            "See also: zap");
}

mixed can_pulsecheck_liv(string str) { 
    if(!creatorp(this_player())) return "This command is only available to builders and creators.";
    else return 1;
}

mixed do_pulsecheck_liv(object ob){
    string name;
    int mhp;

    if(!living(ob)) {
        write("You can only pulsecheck living things.");
        return 1;

    }
    write(ob->GetName()+"'s heart_beat is "+query_heart_beat(ob)+".");
    write(ob->GetName()+"'s HeartRate is "+ob->GetHeartRate()+".\n");
    return 1;
}

mixed do_pulsecheck_lvs(object *obs) {
    foreach(object ob in obs){
        if(!(ob == this_player())) do_pulsecheck_liv(ob);
    }
    return 1;
}
