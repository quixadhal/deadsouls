#include <lib.h>
inherit LIB_BARKEEP;

create() {
    ::create();
    SetKeyName("waitress");
    SetId( ({ "waitress", "restaurant waitress" }) );
    SetShort("A restaurant waitress");
    SetAggressive( 0);
    SetLevel(10);
    SetLong("She is ready to take your order.\n");
    SetGender("female");
    SetMorality(164);
    SetRace( "nymph");
    SetHealthPoints(270);
    AddCurrency("copper", random(200));
    SetSkill("melee",90);
    SetLocalCurrency("gold");
    //SetMenu(
    //	({
    //	"lobster", "worms", "fish", "fries"
    //	}),
    //	({ "food", "food", "food", "food" }),
    //	({ 25, 15, 8, 3 })
    //   );
    //  set_my_mess(
    //	({
    //	"What a wonderful way of not cooking lobsters!\n",
    //	"Feel them wiggle in your throat?\n",
    //	"Tasty indeed!\n",
    //	"You enjoy them one by one to the end.\n"
    //	})
    //   );
    //  set_your_mess(
    //	({
    //	"enjoys some uncooked lobster.\n",
    //	"slides down a plate of worms.\n",
    //	"chomps on some Praxian fish.\n",
    //	"eats some fries one by one.\n"
    // 	})
    //  );
    //   SetMenu_short(
    //	({
    //	"A lobster",
    //	"A plate of sea worms",
    //	"A nice fish",
    //	"Fries"
    //	})
    //  );
    //   SetMenu_long(
    //	({
    //	"It is uncooked in a Praxia sort of way.\n",
    //	"They are still alive!\n",
    //	"Mmmmmmm good.\n",
    //	"Not many there, but something just the same.\n"
    // 	})
    //   );
}
