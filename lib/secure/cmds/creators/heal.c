#include <lib.h>

inherit LIB_DAEMON;

int eventRegenerate(object who){
    int i;
    string *stumps;
    stumps=who->GetMissingLimbs();
    if(sizeof(stumps)) {
	for(i=0;i<sizeof(stumps);i++){
	    who->RestoreLimb(stumps[i]);
	}
    }
    return 1;
}

mixed cmd(string args) {
    string cmd = args;
    object who;
    mapping stumps;
    string *limb_arr = ({});
    int i, mhp, msp, mmp, self;

    if( !cmd || cmd == "" ) {
	return "Syntax: <heal LIVING>";
    }

    if(member_array(args,({"me","myself"})) != -1 ) self = 1;
    else self = 0;

    if( !self && !who = find_living(args) ) {
	write("That being is unavailable."); 
	return 1;
    }
    if(!self){
	previous_object()->eventPrint("You heal " + who->GetName() + ".");
	who->eventPrint(previous_object()->GetCapName() + " heals you.");
	say(this_player()->GetCapName() + " heals "+ who->GetName() + ".");
    }
    else {
	who = this_player();
	previous_object()->eventPrint("You heal thyself.");
	say(this_player()->GetCapName() + " heals "+
	  objective(this_player())+"self.");
    }
    mhp = who->GetMaxHealthPoints();
    msp = who->GetMaxStaminaPoints();
    mmp = who->GetMaxMagicPoints();
    who->AddHP(mhp);
    who->AddStaminaPoints(msp);
    who->AddMagicPoints(mmp);
    if(who->GetPoison() > 0) who->AddPoison(0 - who->GetPoison());
    eventRegenerate(who);
    limb_arr = who->GetLimbs();
    foreach(string limb in limb_arr) {
	who->HealLimb(limb);
    }
    return 1;
}

string GetHelp(string topic) {
    return ("Syntax: <heal LIVING>\n\n"
      "Allows you to restore a living being's health, stamina, "
      "magic, and severed limbs."); 
}
