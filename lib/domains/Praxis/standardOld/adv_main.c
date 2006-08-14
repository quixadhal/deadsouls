#include <config.h>

inherit "/std/room";

int get_exp(int lev);
int train_player(object tp, string which, int amount);
int get_stat_cost(int x, int lev);

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_no_clean(1);
    set_short( "Adventurer's Hall");
    set_long( 
      "This is the Praxis adventurer's hall.\n"
      "Adventurers meet here to discuss their adventures. "
      "Boc La Road is north of here.  South is a small passage "
      "illuminated by a shimmering %^BLUE%^blue%^RESET%^ light.");
    set_exits( 
      (["north" : "/domains/Praxis/e_boc_la2",
	"east" : "/domains/Praxis/lpmud_room",
	"south" : "/domains/Praxis/adv_inner",
	"down" : "/domains/Praxis/ombud_hall"]) );
    set_items(
      (["hall" : "A place for a rowdy time!",
	"road" : "A small dirt path going through Praxis.",
	"passage" : "A %^BLUE%^blue%^RESET%^ light illuminates it.",
	"light" : "It is probably a force field."]) );

    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board", "bulletin board", "reality board" }) );
    ob->set_board_id("main_board");
    ob->set_short("The Nightmare Reality Board");
    // ob->set_short( "The Nightmare Reality Board");
    ob->set_long( "An old cork board where the adventurers "
      "who pass through Praxis post information on the "
      "reality they have discovered.\n");
    ob->set_max_posts(75);

    ob->move(this_object());
    set_property("no steal", 1);
    set_property("no attack", 1);
}

int go_south() {
    if(!creatorp(this_player())) {
	write("%^BLUE%^%^BOLD%^You cannot penetrate the force of the blue light.");
	say(this_player()->query_cap_name()+" tries to get through the "
	  "passage, but fails.", this_player());
	return 0;
    }
    return 1;
}

int get_exp(int lev) {
    int val;

    switch(lev) {
    case 0: val = 0; break;
    case 1: val = 1014; break;
    case 2: val = 2028; break;
    case 3: val = 3056; break;
    case 4: val = 4800; break;
    case 5: val = 6200; break;
    case 6: val = 9100; break;
    case 7: val = 12500; break;
    case 8: val = 15000; break;
    case 9: val = 20000; break;
    case 10: val = 28000; break;
    case 11: val = 40000; break;
    case 12: val = 55000; break;
    case 13: val = 72000; break;
    case 14: val = 104000; break;
    case 15: val = 150000; break;
    default: val = (lev-12)*(lev-15)*28000 + 150000;
    }
    return val;
}

int train_player(object tp, string which, int amount) {
    int exp, amt;

    if(amount < 1) {
	notify_fail("You cannot train that amount.\n");
	return 0;
    }
    exp = (int)tp->query_exp();
    if(exp-amount < get_exp((int)tp->query_level())) {
	notify_fail("You do not have the experience to train that much.\n");
	return 0;
    }
    if((int)tp->query_max_skill(which) <= (int)tp->query_skill(which)) {
	notify_fail("You can train no more in that skill.\n");
	return 0;
    }
    amt = ( (int)tp->query_level() * 4 ) + 20;
    if(amt <= (int)tp->query_skill(which) )
    {
	notify_fail("You must advance your level to train more in that skill"+
	  ".\n");
	return 0;
    }
    tp->add_skill_points(which, amount/4);
    tp->add_exp(-amount);
    tell_object(tp, "You train in the skill of "+which+".");
    return 1;
}

int get_stat_cost(int x, int lev) {
    if(x==1) return ((lev/10)*(lev/10)*10000) + (lev/3)*1000;
    else return (lev/7)*(lev/7)*(lev*900);
}

