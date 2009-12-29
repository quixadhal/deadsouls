#include <lib.h>

inherit STD_ROOM;

int get_exp(int lev);
int train_player(object tp, string which, int amount);
int get_stat_cost(int x, int lev);

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetNoClean(1);
    SetShort( "the adventurer's hall");
    SetLong( 
            "This is the adventurer's hall of the town of Praxis. It "
            "is a large wooden building which looks worn from all the use "
            "over the years. A large bulletin board stands in the center of the "
            "room, filled with notes from the many adventurers of this reality. "
            "In front of the building Boc La Road runs north of here. To the "
            "south is a small passage illuminated by a shimmering "
            "%^BLUE%^blue%^RESET%^ light.");
    SetExits( 
            (["north" : "/domains/Praxis/e_boc_la2",
             "east" : "/domains/Praxis/lpmud_room",
             "south" : "/domains/Praxis/adv_inner",
             "down" : "/domains/Praxis/ombud_hall"]) );
    SetItems(
            (["hall" : "A place for a rowdy time!",
             "road" : "A small dirt path going through Praxis.",
             "passage" : "A %^BLUE%^blue%^RESET%^ light illuminates it.",
             "light" : "It is probably a force field."]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "bulletin board", "reality board" }) );
    ob->set_board_id("main_board");
    ob->SetShort("The Nightmare Reality Board");
    // ob->SetShort( "The Nightmare Reality Board");
    ob->SetLong( "An old cork board where the adventurers "
            "who pass through Praxis post information on the "
            "reality they have discovered.\n");
    ob->set_max_posts(75);

    ob->move(this_object());
    SetProperty("no steal", 1);
    SetProperty("no attack", 1);
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
    exp = tp->query_exp();
    if(exp-amount < get_exp(tp->query_level())) {
        notify_fail("You do not have the experience to train that much.\n");
        return 0;
    }
    if(tp->query_max_skill(which) <= tp->query_skill(which)) {
        notify_fail("You can train no more in that skill.\n");
        return 0;
    }
    amt = ( tp->query_level() * 4 ) + 20;
    if(amt <= tp->query_skill(which) )
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

void init(){
    ::init();
}
