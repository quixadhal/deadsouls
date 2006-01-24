#include <lib.h>
#include <damage_types.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object thing;
    int mhp;
    if( !str || str == "" ) return "Syntax: <zap OBJECT>";
    if(!present(str, this_player()) &&
      !present(str, environment(this_player())))
	return notify_fail("You don't see that.\n");
    thing = present( str, this_player());
    if(!thing) thing = present(str, environment(this_player()));
    if(!thing) return notify_fail("Uh oh, error.\n");
    if(!living(thing)) return "You can only zap living things.";
    if(archp(thing) && !archp(this_player())){
	write("Your zap backfires!");
	say(this_player()->GetName()+"'s zap backfires!");
	return this_player()->eventDestruct();
    }
    mhp = thing->GetMaxHealthPoints();
    if(!mhp) mhp = 99999;
    write("You zap "+thing->GetName()+".");
    say(this_player()->GetName()+" raises a hand and %^RED%^ZAPS%^RESET%^"+
      " "+thing->GetName()+"!",({thing})); 
    tell_object(thing,this_player()->GetName()+" raises a hand and %^RED%^ZAPS%^RESET%^ you!");
    //return thing->AddHP(-mhp - 100);
    thing->eventReceiveDamage(this_player(),BLUNT,(mhp * 5),0,({"torso"}));
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <zap LIVING>\n\n"
      "Deals devastating damage to the creature specified.\n\n");
}
