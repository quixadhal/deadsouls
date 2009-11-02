#include <lib.h>

inherit LIB_ROOM;

object *begging;

void init() {
    ::init();
    add_action("confess", "confess");
    add_action("pray", "pray");
}

void create() {
    ::create();
    SetListen("default", "You hear the faint sounds of "
            "chanting from up the stairs.");
    SetListen("upstairs", "You hear the chanting of monks.");
    SetListen("chants", "We serve an old man in a dry season"
            "\nA lighthouse keeper in the desert sun\n"
            "Dreamers of sleepers and white treason\n"
            "We dream of rain and the history of the gun\n");
    begging = ({});
    SetShort( "Praxis monastary");
    SetLong(
            "You are in the ornate monastary of Praxis.\n"
            "In this huge open monastary run by Praxis monks, the wretched "
            "murders come to confess, and the dead come to pray for resurrection. "
            "A small stairway to the east leads to the monastary attic and "
            "basement. The Praxis Chapel where monks marry people is north. "
            "Boc La Road is outside the monastary to the south. ");
    SetExits( 
            (["south" : "/domains/Praxis/e_boc_la1",
             "east" : "/domains/Praxis/stairs",
             "north" : "/domains/Praxis/chapel"]) );
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetItems(
            (["monastery" : "The home of the Nightmare monks.", 
             "chamber" : "People come here to pray for resurrection "
             "when they die.", 
             "stairway" : "A small spiral set of stairs leading to "
             "the attic and cellar.", 
             "road" : "Boc La Road."]) );
    SetProperty("no attack", 1);
    SetProperty("no steal", 1);
    SetProperty("no castle", 1);
}

int pray() {
    if(!this_player()->query_ghost()) {
        notify_fail("The living do not need to pray for revival.\n");
        return 0;
    }
    this_player()->revive();
    this_player()->SetHealthPoints(10);
    this_player()->set_heart_beat(1);
    this_player()->set_heal_rate(2);
    return 1;
}

int confess(string str) {
    object *inv;
    int i, ok;
    string res;

    if(str != "murder") {
        notify_fail("Confess what?\n");
        return 0;
    }
    ok = 0;
    if(sscanf(this_player()->getenv("TITLE"), "%s murderer $N%*s", res)
            != 1) {
        notify_fail("You are no murderer.\n");
        return 0;
    }
    i = sizeof(inv = all_inventory(this_object()));
    while(i--) if(inv[i]->query_class() == "monk") ok = 1;
    if(!ok) {
        write("There is no one here to whom you may confess.");
        return 1;
    }
    message("my_action", "You beg the monks for forgiveness for "
            "your murder of a "+lower_case(res)+".", this_player());
    message("my_action", ("You pray that forgiveness does not mean "
                "death."), this_player());
    say(this_player()->query_cap_name()+" begs for "+
            this_player()->query_possessive()+" murder of a "+
            lower_case(str)+".");
    begging += ({ this_player() });
    call_out("forgive", 60, this_player());
    return 1;
}

object *query_forgiven() { return begging; }

void forgive(object ob) {
    string tmp;

    if(member_array(ob, begging) == -1) return;
    begging -= ({ ob });
    tmp = call_other("/domains/Praxis/"+ob->query_class()+"_hall",
            "get_new_title", ob);
    message("info", "You are now forgiven.", ob);
    ob->setenv("TITLE", tmp);
    ob->add_mp(-500);
    ob->add_hp(100- (ob->query_skill("faith")));
}


