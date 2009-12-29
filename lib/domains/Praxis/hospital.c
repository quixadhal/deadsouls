#include <lib.h>
#include <daemons.h>

inherit LIB_ROOM;

#define COST    money/bonus
#define MAX_DONATION 600

mapping blood;

void init() {
    ::init();
    add_action("new_body", "renew");
    add_action("clean_poison", "clean");
    add_action("regenerate", "regenerate");
    add_action("read", "read");
    add_action("donate", "donate");
    add_action("transfuse", "transfuse");
}

void create() {
    ::create();
    SetProperties( (["light" :2 , "indoors" :1 , "castle" : 1 ]) );
    SetShort( "the hospital of Praxis");
    SetLong(
            "You are in the hospital of Praxis. It basically consists "
            "of one large single room, with several crude beds placed in rows. "
            "The hospital doesn't look to be to clean, you might want to think "
            "twice before engaging their services. A list of all the services "
            "that can be performed is posted on the far wall. The exit back to the "
            "main road is east.");
    SetItems(
            (["list" : "You can read all the services by typing <read list>.",
             "hospital" : "The clerics here specialize in regenerating "
             "lost limbs.",
             "clerics" : "They are mending the wounds of patients.",
             "cleric" : "He is mending a patient's wounds."]));
    SetExits( 
            (["east" : "/domains/Praxis/n_centre2"]) );
    blood = ([ "who": ([]), "hp":200, "mp":200 ]);
    SetProperty("no teleport", 1);
}

int new_body(string str) {
    object *inv;
    int i;

    if( this_player()->query_level() != 1) {
        notify_fail("The clerics only perform this service for the inexperienced.\n");
        return 0;
    }
    inv = all_inventory(this_player());
    for(i=0; i<sizeof(inv); i++) {
        inv[i]->unequip();
    }
    write("A cleric comes over to you and mutters a small prayer.");
    write("You again have all the limbs you were born with!");
    say("A cleric mutters a small prayer for the novice "+this_player()->query_cap_name()+".", this_player());
    this_player()->new_body();
    return 1;
}

int read(string str) {
    if(!str) {
        notify_fail("Read what?\n");
        return 0;
    }
    if(str != "list") {
        notify_fail("That is not here for reading.\n");
        return 0;
    }
    message("info", "Welcome to the Cleric's Hospital of Praxis!",this_player());
    message("Ninfo",
            "The clerics perform the following services:\n"
            "------------------------------------------------------------------\n"
            "<renew body>: This is a charity service the clerics perform for\n"
            "	novice adventurers who have lost limbs while adventuring.\n"
            "	All limbs are replaced.\n"
            "<regenerate [limb]>: This service is for the experienced adventurer\n"
            "	who has lost limbs.  The limb is replaced and acts like new.\n"
            "	Tithe schedule for regeneration:\n"
            "            from "+currency_value(320, "gold")+" gold for minor limbs (non-clerics)\n"
            "            to "+currency_value(800, "gold")+" gold for major limbs (non-clerics)\n"
            "            "+currency_value(240, "gold")+" to "+currency_value(600, "gold")+" gold for clerics\n"
            "<clean poison>: Helps remove some of the poison from your body.\n"
            "\ttithe: "+currency_value(50, "gold")+" gold\n"
            "<donate # (hp or mp) of blood>: Donates some of your blood in\n"
            "\texchange for gold.\n"
            "<transfuse # (hp or mp)>: Transfuse some blood int hp or mp into your body\n"
            "\ttithe: amount times "+currency_value(3, "gold")+" gold.\n"
            "Currently: "+blood["hp"]+" hp blood and "+blood["mp"]+" mp blood free.\n"
            "------------------------------------------------------------------\n"
            "Half off all regenerations with the severed limb!\n"
            "Your tithe is used only toward good causes.\n", this_player());
    return 1;
}

int clean_poison(string str) {
    object tp;

    if(!str) return 0;
    if(str != "poison") return 0;
    tp = this_player();
    if(tp->query_poisoning()<1) {
        notify_fail("A cleric whispers to you: But you are not poisoned!\n");
        return 0;
    }
    if(tp->query_money("gold") < currency_value(50, "gold")) {
        notify_fail("You do not have enough gold for the tithe.\n");
        return 0;
    }
    tp->AddCurrency("gold", -currency_value(50, "gold"));
    tp->add_poisoning(-10);
    write("A cleric casts a spell of healing upon you.");
    say("A cleric casts a spell of healing on "+tp->query_cap_name()+".", tp);
    return 1;
}

int regenerate(string limb) {
    int money, bonus;
    mapping limb_info;
    object tp;
    string *there, *missing;

    tp = this_player();
    if(present(limb, this_player())) bonus = 2;
    else bonus = 1;
    there = tp->query_limbs();
    missing = this_player()->query_severed_limbs() +
        RACES_D->query_limbs(this_player()->query_race());
    /*
       checking with the race_d is allowing compatibility with old
       versions of the mudlib
     */
    if(!missing) {
        notify_fail("You aren't missing any limbs!\n");
        return 0;
    }
    if(member_array(limb, missing) == -1) {
        notify_fail("You are not missing that limb!\n");
        return 0;
    }
    if(member_array(limb, there) != -1) {
        notify_fail("You already have that one back!\n");
        return 0;
    }
    limb_info= RACES_D->query_limb_info(limb,tp->query_race());
    if(!limb_info) {
        notify_fail("That limb cannot be replaced!\n");
        return 0;
    }
    if(limb_info["attach"] != "0") {
        if(member_array(limb_info["attach"], there) == -1) {
            notify_fail("You would need a "+limb_info["attach"]+" for that!\n");
            return 0;
        }
    }
    if(strsrch(limb, "hand") != -1 || strsrch(limb, "foot") != -1 || 
            strsrch(limb, "hoof") != -1) {
        money = (this_player()->query_class() == "cleric" ? 
                currency_value(240, "gold") : currency_value(320, "gold"));
    }
    else money = (this_player()->query_class() == "cleric" ? 
            currency_value(600, "gold") : currency_value(800, "gold"));
    if(tp->query_money("gold") < COST) {
        notify_fail("The cleric tells you:  You do not have enough gold.\n");
        return 0;
    }
    tp->AddLimb(limb, limb_info["ref"], tp->query_max_hp()/limb_info["max"], 0, 0);
    if(member_array(limb, RACES_D->query_wielding_limbs(tp->query_race())) != -1) 
        tp->add_wielding_limb(limb);
    this_player()->AddCurrency("gold", -COST);
    say(sprintf("%s asks the clerics for some help with %s missing %s.",
                this_player()->query_cap_name(), possessive(this_player()), limb));
    write("The clerics restore your "+limb+"!");
    return 1;
}

int donate(string str) {
    string what;
    int amount, tmp;

    if(!str) {
        notify_fail("Donate what?\n");
        return 0;
    }
    if(sscanf(str, "%d %s of blood", amount, what) !=2) {
        notify_fail("Correct syntax: <donate [#] [hp | mp] of blood>\n");
        return 0;
    }
    if(what != "mp" && what != "hp") {
        notify_fail("Donate what?\n");
        return 0;
    }
    if(blood[this_player()->query_name()]+amount > MAX_DONATION) {
        write("You will have to wait before giving that much blood.");
        return 1;
    }
    tmp = call_other(this_player(), "query_"+what);
    if(tmp < amount + 5) {
        notify_fail("You must have at least 5 more than you plan to give!\n");
        return 0;
    }
    call_other(this_player(), "add_"+what, -amount);
    this_player()->AddCurrency("gold", currency_value(amount/5, "gold"));
    blood[what] += amount;
    blood["who"][this_player()->query_name()] += amount;
    write("You donate some blood for "+(currency_value(amount/5, "gold"))+" gold coins.");
    say(this_player()->query_cap_name()+" donates some blood for some gold.", this_player());
    call_out("reduce_donation", 900, ({ this_player(), amount }));
    return 1;
}

int transfuse(string str) {
    string what;
    int amount;

    if(!str) {
        notify_fail("Transfuse what?\n");
        return 0;
    }
    if(sscanf(str, "%d %s", amount, what) != 2) {
        notify_fail("Correct syntax: <transfuse [#] [mp | hp]>\n");
        return 0;
    }
    if(what != "hp" && what != "mp") {
        notify_fail("You cannot do that!\n");
        return 0;
    }
    if(amount < 1) {
        notify_fail("Are you crazy?!?\n");
        return 0;
    }
    if(blood[what] < amount) {
        notify_fail("We do not have that much blood in right now.\n");
        return 0;
    }
    if(this_player()->query_money("gold") < currency_value(amount*3, "gold")) {
        notify_fail("You do not have enough gold for the tithe.\n");
        return 0;
    }
    this_player()->AddCurrency("gold", -currency_value(amount*3, "gold"));
    call_other(this_player(), "add_"+what, amount);
    write("You receive a blood transfusion");
    say(this_player()->query_cap_name()+" receives a blood transfusion.", this_player());
    return 1;
}

void reduce_donation(mixed *tmp) {
    blood["who"][tmp[0]] -= tmp[1];
    if(blood["who"][tmp[0]] < 1) map_delete(blood, tmp[0]);
}




