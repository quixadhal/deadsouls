/*    /lib/corpse.c
 *    from the Dead Souls LPC Library
 *    standard corpse object
 *    created by Descartes of Borg 960711
 *    Version: @(#) corpse.c 1.10@(#)
 *    Last Modified: 96/12/31
 */

#include <lib.h>
#include <medium.h>
#include <message_class.h>

inherit LIB_SURFACE;

int DecayLife, Count, CallOut, Fresh;
string Owner, Race;

int eventDecay() {
    int smell;
    if( !environment() ) {
        Destruct();
        return 0;
    }
    Fresh = 0;
    if(environment()->GetMedium() == MEDIUM_LAND) smell = 1;
    switch(Count) {
    case 10:
        if(smell){
            environment()->eventPrint(possessive_noun(Owner) + " corpse " +
              "is starting to stink.", MSG_ROOMDESC);
            SetId(GetId()..., "corpse", "remains","flesh","pile","pile of flesh");
            SetAdjectives(GetAdjectives()..., "stinky", "rotting");
            SetShort("the stinky remains of a rotting corpse");
            SetSmell("This corpse is beginning to stink up the entire area.");
        }
        break;
    case 20:
        if(smell){
            environment()->eventPrint("A rotting stench fills the entire "
              "area.", MSG_ROOMDESC);
            SetId(GetId()..., "flesh", "pile", "pile of flesh");
            SetShort("a pile of rotting flesh");
            SetSmell("Its smell is nearly unbearable.");
        }
        break;
    case 30:
        Destruct();
        return 0;
    }
    Count++;
    return Count;
}

static int Destruct() {
    return ::Destruct();
}

int SetDecayLife(int x) { return (DecayLife = x); }

int GetDecayLife() { return DecayLife; }

void SetCorpse(object who) {
    string tmpshort = (string)who->GetShort();
    if(sizeof(who->GetRiders())) tmpshort = (string)who->GetPlainShort();
    if( DecayLife < 100 ) {
        DecayLife = 500;
    }
    SetKeyName(who->GetKeyName());
    SetId(({ "body","corpse",who->GetId()... }) );
    Owner = who->GetCapName();
    Race = who->GetRace();
    Count = 1;
    Fresh = 1;
    SetShort("the corpse of "+tmpshort);
    SetLong("As you look closely at " + who->GetCapName() +
      ", you notice that " +  nominative(who) +
      " does not appear to be moving.");
}

int isFreshCorpse() {
    return Fresh;
}

string GetHealthShort() {
    return "%^RED%^" + GetShort();
}

string GetOwner() { return Owner; }

string GetRace() { return Race; }

string GetSaveString() { return 0; }

int CanReceive(object ob) { return 1; }


static void create() {
    ::create();
    SetId( ({ "corpse", "flesh", "remains" }) );
    SetAdjectives( ({"pile of", "rotting", "stinky"}) );
    Count = 0;
    CallOut = 0;
    DecayLife = 100;
    Owner = 0;
    Race = 0;
    SetNoCondition(1);
}

int direct_animate_obj() {
    return 1;
}

int direct_offer_obj() {
    return 1;
}

string GetItemCondition() { return "";}
