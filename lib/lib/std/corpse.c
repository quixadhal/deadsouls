/*    /lib/corpse.c
 *    from the Dead Souls LPC Library
 *    standard corpse object
 *    created by Descartes of Borg 960711
 *    Version: @(#) corpse.c 1.10@(#)
 *    Last Modified: 96/12/31
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_STORAGE;
inherit LIB_SMELL;

private int DecayLife, Count, CallOut, Fresh;
private string Owner, Race;

int eventDecay() {
    if( !environment() ) {
        Destruct();
        return 0;
    }
   Fresh = 0;
    switch(Count) {
        case 1:
	    environment()->eventPrint(possessive_noun(Owner) + " corpse " +
				      "is starting to stink.", MSG_ROOMDESC);
	    SetId(GetId()..., "corpse", "remains");
	    SetAdjectives(GetAdjectives()..., "stinky", "rotting");
            SetShort("the stinky remains of a rotting corpse");
	    SetSmell("This corpse is beginning to stink up the entire area.");
            break;
        case 2:
	    environment()->eventPrint("A rotting stench fills the entire "
				      "area.", MSG_ROOMDESC);
	    SetId(GetId()..., "flesh", "pile", "pile of flesh");
            SetShort("a pile of rotting flesh");
	    SetSmell("Its smell is nearly unbearable.");
            break;
        case 3:
            Destruct();
            return 0;
    }
    Count++;
    CallOut = call_out((: eventDecay :), DecayLife/3);
    return Count;
}

static int Destruct() {
    if( CallOut ) {
	remove_call_out(CallOut);
    }
    return storage::Destruct();
}

int SetDecayLife(int x) { return (DecayLife = x); }

int GetDecayLife() { return DecayLife; }

void SetCorpse(object who) {
    if( DecayLife < 100 ) {
	DecayLife = 100;
    }
    SetKeyName(who->GetKeyName());
    SetId(who->GetId()...);
    Owner = who->GetCapName();
    Race = who->GetRace();
    Count = 1;
    Fresh = 1;
    SetShort((string)who->GetShort());
    SetLong("As you look closely at " + who->GetCapName() +
	    ", you notice that " +  nominative(who) +
	    " does not appear to be moving.");
    CallOut = call_out((: eventDecay :), DecayLife/3);
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
    storage::create();
    SetId( ({ "corpse", "flesh", "remains" }) );
    SetAdjectives( ({"pile of", "rotting", "stinky"}) );
    Count = 0;
    CallOut = 0;
    DecayLife = 100;
    Owner = 0;
    Race = 0;
}

int direct_animate_obj() {
    return 1;
}

int direct_offer_obj() {
    return 1;
}

