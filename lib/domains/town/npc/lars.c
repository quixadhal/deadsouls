#include <lib.h>
#include <position.h>

inherit LIB_BARKEEP;

mixed NoKill(object attacker);

static void create() {
    barkeep::create();
    SetKeyName("lars");
    SetId( ({ "lars", "owner", "barkeep", "bartender", "keeper" }) );
    SetShort("Lars, the keeper of the pub");
    SetLevel(1);
    SetLong("Lars is the owner of the local pub, an old and famous "
            "drinking establishment in the heart of town.  He is an "
            "unassuming fellow, and quite jovial.");
    SetMenuItems(([
                ({ "espresso", "imported espresso" }) : "/domains/town/meals/espresso",
                ({ "sandwich", "ham sandwich" }) : "/domains/town/meals/ham_sand",
                ({ "spring water", "water" }) : "/domains/town/meals/water",
                ({ "first class ale", "ale" }) : "/domains/town/meals/ale",
                ({ "regular coffee", "coffee" }) : "/domains/town/meals/coffee",
                ({ "firebreather" }) : "/domains/town/meals/fire",
                ({ "special of the house", "special" }) : "/domains/town/meals/special",
                ]));
    SetGender("male");
    SetMorality(40);
    SetRace("human");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetAttackable( (: NoKill :) );
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}
void init(){
    ::init();
}

mixed NoKill(object attacker){
    if(attacker->GetTown() == "Town"){
        return "Lars is like your favorite uncle. You find yourself unable to attack him.";
    }
    else {
        tell_object(attacker,"Lars casually deflects your attack and boots you out the door.");
        say("Lars casually deflects an attack from "+attacker->GetName()+" and "
                "boots "+objective(attacker)+" out the door.");
        tell_room("/domains/town/room/road",attacker->GetName()+" comes flying out of the pub and "
                "lands on "+possessive(attacker)+" butt on the road.");
        attacker->eventMove("/domains/town/room/road");
        attacker->eventDescribeEnvironment();
        attacker->SetPosition(POSITION_SITTING);
        return "";
    }
}
