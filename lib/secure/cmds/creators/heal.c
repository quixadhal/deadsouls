#include <lib.h>
#include ROOMS_H

inherit LIB_DAEMON;

mixed cmd(string args) {
    string tp, cmd = args;
    object who;
    mapping stumps;
    string *limb_arr = ({});
    object *germs;
    int i, mhp, msp, mmp, self;

    if(this_player()) tp = this_player()->GetKeyName();
    if( !cmd || cmd == "" ) {
        return "Syntax: <heal LIVING>";
    }

    if(member_array(args,({ "me","myself", (tp || "") })) != -1 ) self = 1;
    else self = 0;

    if( !self && !who = find_living(args) ) {
        write("That being is unavailable."); 
        return 1;
    }
    if((who && who->GetGhost()) ) {
        write("You can't heal the dead.");
        return 1;
    }
    if(!self){
        previous_object()->eventPrint("You heal " + who->GetName() + ".");
        who->eventPrint(previous_object()->GetCapName() + " heals you.");
        say(this_player()->GetCapName() + " heals "+ who->GetName() + ".",who);
    }
    else {
        who = this_player();
        previous_object()->eventPrint("You heal thyself.");
        say(this_player()->GetCapName() + " heals "+
                objective(this_player())+"self.",who);
    }
    mhp = who->GetMaxHealthPoints();
    msp = who->GetMaxStaminaPoints();
    mmp = who->GetMaxMagicPoints();

    if(who->GetLead()){
        int shots = who->GetLead("gunshot_wounds");
        if(shots) who->AddLead("gunshot_wounds", -shots);
        shots = who->GetLead("rifleshot_wounds");
        if(shots) who->AddLead("rifleshot_wounds", -shots);
    }

    while(sizeof(who->GetMissingLimbs())){
        limb_arr = who->GetMissingLimbs();
        foreach(string limb in limb_arr) {
            who->RestoreLimb(limb);
        }
    }

    if(who->GetPoison() > 0) who->AddPoison(0 - who->GetPoison());

    germs = filter(deep_inventory(who), (: $1 && $1->isGerm() :) );

    if(sizeof(germs)) germs->eventMove(ROOM_FURNACE);
    germs->eventDestruct();

    limb_arr = who->GetLimbs();
    foreach(string limb in limb_arr) {
        who->HealLimb(limb);
    }

    who->AddHP(mhp);
    who->AddStaminaPoints(msp);
    who->AddMagicPoints(mmp);

    return 1;
}

string GetHelp(){
    return ("Syntax: heal <LIVING>\n\n"
            "Allows you to restore a living being's health, stamina, "
            "magic, and severed limbs. Wow!");
}
