/*    /domains/Praxis/realty.c
 *    from Nightmare IV
 *    the place to buy stuff
 *    created by Descartes of Borg 940702
 */

#include <lib.h>
#include <objects.h>

#define DEED_COST 250000
#define ORDER_COST 17000

inherit LIB_ROOM;

void create() {
    room::create();
    SetProperties( ([ "light":2, "indoors":1 ]) );
    SetShort("Praxis Realty");
    SetLong(
            "Welcome to Praxis Realty!\n"
            "People come here to buy deeds to build estates, as well as work "
            "orders for making changes to those estates.  The office is built "
            "up in all kinds of dreary colours, with a desk at the far end of the "
            "office being the spot where all business is done."
           );
    SetItems( ([ "colours": "Mostly puce.", "desk": (: "at_desk" :),
                "office":"A nice little space with a desk in it." ]) );
    SetExits( ([ "north" : "/"+__DIR__+"unnamed1" ]) );
}

void init() {
    room::init();
    add_action("read_list", "read");
    add_action("cmd_buy", "buy");
}

void reset() {
    object ob, arm;

    room::reset();
    if(present("atmos")) return;
    ob = new(LIB_NPC);
    ob->SetKeyName("atmos");
    ob->SetProperties( (["no paralyze":1, "no steal":1]));
    ob->SetId( ({ "atmos", "atmos the patron of high mortals", "patron",
                "realtor" }) );
    ob->SetShort("Atmos, the Patron of High Mortals");
    ob->SetLong("Atmos is a geek.");
    ob->SetLevel(66);
    ob->SetRace("artrell");
    ob->SetClass("mage");
    ob->AddCurrency("gold", currency_value(100, "gold"));
    ob->SetStat("constitution", 500);
    ob->SetStat("dexterity",500);
    ob->SetStat("strength", 500);
    ob->SetSkill("melee", 390);
    ob->SetSkill("defense", 500);
    ob->SetSkill("stealth", 300);
    ob->SetSkill("magic attack", 500);
    ob->SetHealthPoints(1000000);
    ob->SetMagicPoints(5000000);
    ob->SetSpellChance(20);
    ob->SetSpells( ({ "freeze", "fireball" }) );
    ob->SetMorality(100);
    ob->AddLimb("head","FATAL",800000,0,15);
    ob->AddLimb("torso","FATAL",800000,0,15);
    ob->AddLimb("first arm","",600000,0,15);
    ob->AddLimb("second arm","",600000,0,15);
    ob->AddLimb("third arm","",600000,0,15);
    ob->AddLimb("fourth arm","",600000,0,15);
    ob->AddLimb("first hand","",400000,0,15);
    ob->AddLimb("second hand",400000,0,15);
    ob->AddLimb("third hand","",400000,0,15);
    ob->AddLimb("fourth hand","",400000,0,15);
    ob->AddLimb("left leg","",600000,0,15);
    ob->AddLimb("right leg","",600000,0,15);
    ob->AddLimb("left foot","",400000,0,15);
    ob->AddLimb("right foot","",400000,0,15);
    ob->move(this_object());
    arm = new(LIB_ARMOR);
    arm->SetKeyName("plate of atmos");
    arm->SetId( ({ "plate", "plate of atmos" }) );
    arm->SetAdjectives( ({ "heavy", "the" }) );
    arm->SetShort("the plate of Atmos");
    arm->SetLong("A beautifully crafted, fake-silver plate armour once "
            "owned by Atmos the Geek.");
    arm->SetAC(10);
    arm->true(1);
    arm->SetValue(1000);
    arm->SetMass(1500);
    arm->SetType("armour");
    arm->SetRestrictLimbs( ({ "torso", "first arm", "second arm", "third arm",
                "fourth arm" }) );
    arm->set_illuminate(20);
    arm->move(ob);
    ob->eventForce("wear plate");
}

static int cmd_buy(string str) {
    object ob;

    if(!present("atmos", this_object())) 
        return notify_fail("Atmos is missing!\n");
    if(str == "deed") {
        if(this_player()->query_money("gold") <
                currency_value(DEED_COST, "gold")) {
            message("my_action", "You do not have enough gold.",this_player());
            return 1;
        }
        message("my_action", "You purchase a deed.", this_player());
        message("other_action", this_player()->query_cap_name()+
                " purchases a deed.", this_object(), ({ this_player() }));
        this_player()->AddCurrency("gold", -currency_value(DEED_COST, "gold"));
        ob = new(OB_DEED);
        if(ob->move(this_player())) {
            message("my_action", "You drop your deed.", this_player());
            message("other_action", this_player()->query_cap_name()+
                    " drops "+possessive(this_player())+" deed.", this_object(),
                    ({ this_player() }));
            ob->move(this_object());
        }
        return 1;
    }
    else if(str == "order" || str == "work order" ) {
        if(this_player()->query_money("gold") <
                currency_value(ORDER_COST, "gold")) {
            ob->eventForce("speak You are too low on gold!");
            return 1;
        }
        message("my_action", "You purchase a work order.", this_player());
        message("other_action", this_player()->query_cap_name()+
                " purchases a work order.", this_object(), ({ this_player() }));
        this_player()->AddCurrency("gold", -currency_value(ORDER_COST, "gold"));
        ob = new(OB_ORDER);
        if(ob->move(this_player())) {
            message("my_action", "You drop the work order!", this_player());
            message("other_action", this_player()->query_cap_name()+
                    " drops "+possessive(this_player())+" work order.",
                    this_object(), ({ this_player() }));
            ob->move(this_object());
        }
        return 1;
    }
    else return 0;
}

string at_desk(string str) {
    string tmp;

    tmp = "A list of real estate options is posted on the desk.  ";
    if(present("atmos")) tmp += "Atmos is seated behind it waiting.";
    else tmp += "No one seems to be around.";
    return tmp;
}
static int read_list(string str) {
    string tmp;

    if(str != "list") return 0;
    tmp = "Welcome to Praxis Realty!\n"
        "You may purchase any of the following items:\n"
        "    deed- an estate deed for creating an estate:  "+
        currency_value(DEED_COST, "gold")+" gold.\n"
        "    order- a work order for adding rooms:         "+
        currency_value(ORDER_COST, "gold")+" gold.\n";
    message("info", tmp, this_player());
    return 1;
}
