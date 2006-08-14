#include <lib.h>

inherit LIB_BARKEEP;

create() {
    ::create();
    SetProperty("no bump", 1);
    SetKeyName("lars");
    SetId( ({ "lars", "barkeep" }) );
    SetShort("Lars, the proprietor of the Lars Pub");
    SetAggressive( 0);
    SetLevel(12);
    SetLong("He tends bar and he listens.\n");
    SetGender("male");
    SetMorality(122);
    SetRace( "human");
    SetHealthPoints(300);
    AddCurrency("gold", random(200));
    SetRace("human");
    SetLocalCurrency("gold");
    //SetMenu(
    //	({
    //	"firebreather", "special", "ale"
    //	}),
    //	({ "alcoholic", "alcoholic", "alcoholic" }),
    //	({ 25, 15, 3 })
    //    );
    //    set_my_mess(
    //	({
    //	"The firebreather burns your throat!\n",
    //	"Wow! Lars sure knew how to make drinks when he invented that!\n",
    //	"You down some ale.\n"
    //	})
    //    );
    //    set_your_mess(
    //	({
    //	"screams from the burning of a drink of a firebreather.\n",
    //	"drinks a special of the house.\n",
    //	"drinks some ale.\n"
    //    	})
    //    );
    //    SetMenu_short(
    //	({
    //	"A firebreather",
    //	"A special of the house",
    //	"A pale ale"
    //	})
    //    );
    //    SetMenu_long(
    //	({
    //	"A firebreather from the Lars Pub.\n",
    //	"A classic special of the house from the Lars Pub.\n",
    //	"A pale ale brewed in the farming fields outside Praxis.\n"
    //   	})
    //    );
}
