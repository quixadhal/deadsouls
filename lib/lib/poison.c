/*  /lib/poison.c
 *  from the Dead Souls  Library
 *  Poisoning functions to be inherited *with* other objects that
 *  are meant to poison things.
 *  created by Blitz@Dead Souls 951120
 */

#include "include/poison.h"

private int PoisonUses, PoisonStrength;

static void create(){
    PoisonStrength = 0;
    PoisonUses = 0;
    this_object()->AddSave( ({ "PoisonUses" }) );
}

mixed indirect_poison_obj_with_obj(){
    if( environment() != this_player() ) return 0;
    if( PoisonStrength < 1 ) return "The poison is all used up.";
    return 1;
}

mixed eventSpreadPoison(object who, object target){
    if( !who || !target || !PoisonUses ) return 0;
    PoisonUses--;
    if( random(50) > who->GetStatLevel("coordination") ){
        this_player()->eventPrint("You fumble around with the poison, "
                "spilling it on yourself.");
        environment(this_player())->eventPrint(who->GetName() +
                " fumbles around with some "
                "poison, spilling it on " +
                reflexive(who) + ".",
                this_player());
        who->AddPoison(random(10));
        return 1;
    }
    target->eventPoison(who, this_object(), PoisonStrength);
    return 1;
}

int SetPoisonStrength(int x){ return (PoisonStrength = x); }

mixed GetPoisonStrength(){ return PoisonStrength; }

int SetPoisonUses(int x){ return (PoisonUses = x); }

int GetPoisonUses(){ return PoisonUses; }

