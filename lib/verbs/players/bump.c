/*  /cmds/verbs/player/bump.c
 *  Allows players to possibly bump a target into another room
 *  created by Blitz@Dead Souls
*/
 
#pragma save_binary

#include <lib.h>
#include <damage_types.h>
#include "include/bump.h"
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("bump");
    SetRules("LIV");
    SetErrorMessage("Whom would you like to bump?");
    SetHelp( "Syntax: bump LIV\n\n"        
         "This command allows you bump the living object named "
         "into an adjacent room.  It is quite possible that "
         "bumping may cause you bodily injury.");
}
 
mixed can_bump_liv() {
    object env = environment(this_player());
    
    if( !env ) return "You are nowhere!";
    if( env->GetProperty("no bump") )
      return "Mystical forces prevent your pushy actions.";
    return 1;
}

mixed do_bump_liv(object ob) {
  int Strength, TargetStrength;
  string *Exits;
  string NewLocation, OldLocation;
  mixed var;
  object env;
 
  if( !ob ) return 0;
  if( !(env = environment(ob)) ) return 0;
  if( (int)ob->GetProperty("no bump") ) {
    message("my_action", "You try to bump "+
      (string)ob->GetCapName()+".",
      this_player() );
    message("other_action", (string)this_player()->GetCapName()+" tries "
      "to bump "+(string)ob->GetCapName()+".",
      environment(ob), ({ ob, this_player() }) );
    message("other_action", (string)this_player()->GetCapName()+" tries "
      "to bump you.",
      ob);
    if( !playerp(ob) )
      ob->eventForce("growl "+(string)this_player()->GetKeyName());
    return 1;
  }
  if( var = ob->eventBump(this_player()) ) return var;
  this_player()->AddStaminaPoints( -(random(15) + 3) );
  if( ( (int)ob->GetStatLevel("agility") / 2 ) >
      ( (int)this_player()->GetStatLevel("strength")) ) {
    message("my_action", "You deftly sidestep "+
      possessive_noun((string)this_player()->GetCapName())+" attempt "
      "to bump you.",
    ob);
    message("other_action", (string)ob->GetCapName()+" deftly sidesteps "
      "your attempt to bump "+objective(ob)+".",
      this_player() );
    message("other_action", (string)ob->GetCapName()+" deftly sidesteps "+
      possessive_noun((string)this_player()->GetCapName())+" attempt "
      "to bump "+objective(ob)+".",
      env, ({ this_player(), ob }));
    return 1;
  }
  Strength = (int)this_player()->GetStatLevel("strength") +
             random((int)this_player()->GetStatLevel("speed") / 2);
  TargetStrength = (int)ob->GetStatLevel("strength") +
                   random( (int)ob->GetStatLevel("agility") / 2 );
  if( (Strength - TargetStrength) < -10 ) {
    this_player()->eventReceiveDamage(ob, BLUNT, random(5) + 1);
    message("other_action", (string)ob->GetCapName()+" shoves you "
      "to the ground!",
      this_player() );
    message("my_action", (string)this_player()->GetCapName()+" is shoved "
      "to the ground while trying to bump you.",
      ob);
    message("other_action", (string)this_player()->GetCapName()+" is shoved "
      "to the ground while trying to bump "+(string)ob->GetCapName()+".",
      env, ({ ob, this_player() }) );
    return 1;
  }
  else if( (Strength - TargetStrength) < (5 + random(20)) ) {
    this_player()->eventReceiveDamage(ob, BLUNT, random(3) + 1);
    message("my_action", "You fail to bump "+(string)ob->GetCapName()+" out "
      "of the way.", this_player() );
    message("other_action", (string)this_player()->GetCapName()+" fails in "
      "an attempt to bump you.",
      ob);
    message("other_action", (string)this_player()->GetCapName()+" fails in "
      "an attempt to bump "+(string)ob->GetCapName()+".",
      environment(ob), ({ ob, this_player() }) );
    return 1;
  } else {
    this_player()->AddStatPoints("strength", random(5));
    Exits = (string *)env->GetExits();
    Exits = filter(Exits,
            (: !(object)$(env)->GetDoor($1) ||
               !(object)$(env)->GetDoor($1)->GetClosed() :) );
    if( !sizeof(Exits) ) {
      message("system", "There is nowhere for "+(string)ob->GetCapName()
        +" to go!", this_player() );
      return 1;
    }
    NewLocation = Exits[ random(sizeof(Exits)) ];
    NewLocation = environment(ob)->GetExit(NewLocation);
    OldLocation = base_name(environment(ob));
    message("my_action", "You shove "+(string)ob->GetCapName()+" out "
      "of the way!",
      this_player() );
    message("other_action", "You are shoved out of the way by "+
      (string)this_player()->GetCapName()+"!",
      ob);
    message("other_action", (string)this_player()->GetCapName()+" shoves "+
      (string)ob->GetCapName()+" out of the way!",
      environment(ob), ({ ob, this_player() }) );
    if( !(int)ob->eventMove(NewLocation) ) {
      message("other_action", (string)ob->GetCapName()+" is bounced "
        "back into the room.", environment(ob), ob);
      message("my_action", "You are bounced back to your original "
        "location.",
        ob);
    } else {
      ob->eventDescribeEnvironment(0);
      if( !userp(ob) )
        call_out((: MoveBack :), 12 + random(6), ob, OldLocation);
    }
    if( (int)ob->GetHealthPoints() > 5 )
      ob->eventReceiveDamage(this_player(), BLUNT, random(5));
    return 1;
  }
  return 0;
}
 
void MoveBack(object ob, string where) {
  if( !ob ) return;
  ob->eventMoveLiving(where, "$N leaves angrily", "$N enters angrily");
  ob->eventForce("growl");
}
 
