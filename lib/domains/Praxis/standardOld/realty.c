/*    /domains/Praxis/realty.c
 *    from Nightmare IV
 *    the place to buy stuff
 *    created by Descartes of Borg 940702
 */

#include <std.h>
#include <objects.h>

#define DEED_COST 250000
#define ORDER_COST 17000

inherit ROOM;

void create() {
    room::create();
    set_properties( ([ "light":2, "indoors":1 ]) );
    set_short("Praxis Realty");
    set_long(
      "Welcome to Praxis Realty!\n"
      "People come here to buy deeds to build estates, as well as work "
      "orders for making changes to those estates.  The office is built "
      "up in all kinds of dreary colours, with a desk at the far end of the "
      "office being the spot where all business is done."
    );
    set_items( ([ "colours": "Mostly puce.", "desk": (: "at_desk" :),
	"office":"A nice little space with a desk in it." ]) );
    set_exits( ([ "north" : "/"+__DIR__+"unnamed1" ]) );
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
    ob = new(MONSTER);
    ob->set_name("atmos");
    ob->set_properties( (["no paralyze":1, "no steal":1]));
    ob->set_id( ({ "atmos", "atmos the patron of high mortals", "patron",
	"realtor" }) );
    ob->set_short("Atmos, the Patron of High Mortals");
    ob->set_long("Atmos is a geek.");
    ob->set_level(66);
    ob->set_race("artrell");
    ob->set_class("mage");
    ob->add_money("gold", currency_value(100, "gold"));
    ob->set_stats("constitution", 500);
    ob->set_stats("dexterity",500);
    ob->set_stats("strength", 500);
    ob->set_skill("melee", 390);
    ob->set_skill("defense", 500);
    ob->set_skill("stealth", 300);
    ob->set_skill("magic attack", 500);
    ob->set_hp(1000000);
    ob->set_mp(5000000);
    ob->set_spell_chance(20);
    ob->set_spells( ({ "freeze", "fireball" }) );
    ob->set_alignment(100);
    ob->add_limb("head","FATAL",800000,0,15);
    ob->add_limb("torso","FATAL",800000,0,15);
    ob->add_limb("first arm","",600000,0,15);
    ob->add_limb("second arm","",600000,0,15);
    ob->add_limb("third arm","",600000,0,15);
    ob->add_limb("fourth arm","",600000,0,15);
    ob->add_limb("first hand","",400000,0,15);
    ob->add_limb("second hand",400000,0,15);
    ob->add_limb("third hand","",400000,0,15);
    ob->add_limb("fourth hand","",400000,0,15);
    ob->add_limb("left leg","",600000,0,15);
    ob->add_limb("right leg","",600000,0,15);
    ob->add_limb("left foot","",400000,0,15);
    ob->add_limb("right foot","",400000,0,15);
    ob->move(this_object());
    arm = new(ARMOUR);
    arm->set_name("plate of atmos");
    arm->set_id( ({ "plate", "plate of atmos" }) );
    arm->set_adjectives( ({ "heavy", "the" }) );
    arm->set_short("the plate of Atmos");
    arm->set_long("A beautifully crafted, fake-silver plate armour once "
      "owned by Atmos the Geek.");
    arm->set_ac(10);
    arm->set_destroy(1);
    arm->set_value(1000);
    arm->set_mass(1500);
    arm->set_type("armour");
    arm->set_limbs( ({ "torso", "first arm", "second arm", "third arm",
	"fourth arm" }) );
    arm->set_illuminate(20);
    arm->move(ob);
    ob->force_me("wear plate");
}

static int cmd_buy(string str) {
    object ob;

    if(!present("atmos", this_object())) 
	return notify_fail("Atmos is missing!\n");
    if(str == "deed") {
	if((int)this_player()->query_money("gold") <
	  currency_value(DEED_COST, "gold")) {
	    message("my_action", "You do not have enough gold.",this_player());
	    return 1;
	}
	message("my_action", "You purchase a deed.", this_player());
	message("other_action", (string)this_player()->query_cap_name()+
	  " purchases a deed.", this_object(), ({ this_player() }));
	this_player()->add_money("gold", -currency_value(DEED_COST, "gold"));
	ob = new(OB_DEED);
	if((int)ob->move(this_player())) {
	    message("my_action", "You drop your deed.", this_player());
	    message("other_action", (string)this_player()->query_cap_name()+
	      " drops "+possessive(this_player())+" deed.", this_object(),
	      ({ this_player() }));
	    ob->move(this_object());
	}
	return 1;
    }
    else if(str == "order" || str == "work order" ) {
	if((int)this_player()->query_money("gold") <
	  currency_value(ORDER_COST, "gold")) {
	    ob->force_me("speak You are too low on gold!");
	    return 1;
	}
	message("my_action", "You purchase a work order.", this_player());
	message("other_action", (string)this_player()->query_cap_name()+
	  " purchases a work order.", this_object(), ({ this_player() }));
	this_player()->add_money("gold", -currency_value(ORDER_COST, "gold"));
	ob = new(OB_ORDER);
	if((int)ob->move(this_player())) {
	    message("my_action", "You drop the work order!", this_player());
	    message("other_action", (string)this_player()->query_cap_name()+
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
