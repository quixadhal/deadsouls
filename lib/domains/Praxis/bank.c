//	/d/standard/bank.c
//	from the Nightmare mudlib
//	banking room
//	created by Descartes of Borg 28 february 1993

#include <lib.h>
#include <daemons.h>
//#include <money.h>
//#include <bank.h>

#define BANK_ID "praxis"

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("Bugga", "Bugga");
    add_action("read", "read");
}

void create() {
    ::create();
    SetProperty("magic hold", 10);
    SetProperty("no castle", 1);
    SetProperty("light", 2);
    SetProperty("night light", 2);
    SetShort( "the bank of praxis");
    SetLong(
            "Welcome to the Bank of Praxis!\n"
            "The Bank of Praxis is a lovely looking building. Red carpeting "
            "covers the worn floor, and to the north there is a polished wooden "
            "counter. In the back of the bank there is a vault where all "
            "the town's deposits are kept. A sign by the teller details "
            "all commands. The exit to the bank is back south.");
    SetItems(
            (["bank" : "You are in its huge lobby. There is a counter in "
             "front of you\nand and exit behind you.",
             "citizens" : "They are wandering about aimlessly.",
             "account" : "You're a damn loon.",
             "sign" : "Reading it will give you a list of commands.",
             "teller" : "The teller looks at you impatiently.",
             "counter" : "A teller waits behind it for you to do something.",
             "exit" : "It leads out into the alley.",
             "vault" : (: this_object(), "look_at_vault" :) ]) );
    SetExits( 
            (["south":"/domains/Praxis/alley1"]) );
}

void reset() {
    object mon, weapon, key;

    ::reset();
    if(!present("guard")) {
        mon = new(LIB_NPC);
        mon->SetKeyName("guard");
        mon->SetId( ({ "guard", "bank guard", "big ogre" }) );
        mon->SetRace( "ogre");
        mon->SetGender("male");
        mon->SetShort( "Bank guard");
        mon->SetLong( "A big, ugly ogre hired to guard the "
                "newly open bank.\n");
        mon->SetLevel(14);
        mon->SetRace("human");
        mon->SetHealthPoints(500 + random(100));
        mon->SetClass("fighter");
        mon->SetSpellChance(10);
        mon->SetSpells( ({ "parry", "Bugga" }) );
        mon->SetSkills("defense", 70);
        mon->SetSkills("blade", 90);
        mon->set_emotes(9, 
                ({ "Guard says: Goddamn thief!",
                 "Guard grunts.",
                 "Guard says: No way you're getting past me!",
                 "Guard says: You disgust me."}), 1);
        mon->set_emotes(3,
                ({ "Guard munches on a rat pie.", 
                 "Guard says: I hate rogues." }), 0);
        mon->SetWielding_limbs( ({ "right hand", "left hand" }) );
        mon->move(this_object());
        weapon = new(LIB_ITEM);
        weapon->SetKeyName("broadsword");
        weapon->SetId( ({ "broadsword", "sword" }) );
        weapon->SetShort( "Broadsword");
        weapon->SetLong( "A huge broadsword.");
        weapon->SetClass(13);
        weapon->SetType("blade");
        weapon->SetMass(700);
        weapon->SetValue(91);
        weapon->move(mon);
        mon->eventForce("wield broadsword in right hand");
        key = new(LIB_ITEM);
        key->SetKeyName("key");
        key->SetId( ({ "bank key", "key", "bronze key" }) );
        key->SetShort( "Bronze key");
        key->SetLong( "An unremarkable bronze key.");
        key->SetMass(29);
        key->SetValue(35);
        key->move(mon);
    }
    if(query_reset_number() != 1) {
        object tmpob = present("guard");
        if(tmpob){
            tmpob->eventForce("close vault");
            tmpob->eventForce("lock vault with key");
        }
    }
}

int do_drunkard() {
    if(present("guard") && !this_player()->query_invis()) {
        present("guard")->eventForce("kill "+this_player()->query_name());
        this_player()->add_follower(present("guard"));
        write("The guard foils you before you can slip the key in!");
        say(this_player()->query_cap_name()+" is foiled trying to break "
                "into the vault!");
        return 0;
    }
    if(this_player()->query_intox()) {
        write("You are fumble around drunkenly with the bank key.");
        say(this_player()->query_cap_name()+" fiddles around drunkenly "
                "with the bank key.");
        return 0;
    }
    return 1;
}

int read(string str) {
    if(str != "sign") return notify_fail("Read what?\n");
    message("info",
            "You may do any of the following at Praxis Merchant's Bank:\n"
            "<open account>\nWill open an account for you.\n\n"
            "<close account>\nCloses your account.\n\n"
            "<balance>\nGives you account balance information.\n\n"
            "<deposit [#] [type]>\nDeposits # of currency of type.\n\n"
            "<withdraw [#] [type]>\nWithdraws # of currency of type.\n\n"
            "<exchange [#] of [type1] for [type2]>\nExchanges currencies.  There is a 10% charge.\n\n",
            this_player()
           );
    return 1;
}

int Bugga(string str) {
    string limb;
    object ob;
    int amount;

    if(this_player()->is_player()) return 0;
    ob = (object)this_player()->query_current_attacker();
    if(!ob) return 1;
    limb = ob->return_limb();
    if(ob->query_class() == "rogue") amount = 24 + random(30);
    else amount = 20 + random(20);
    ob->do_damage(limb, amount);
    tell_object(ob, "The guard bashes your "+limb+" with his "
            "left fist!");
    tell_room(this_object(), "The guard bashes "+ob->query_cap_name()+"'s "+
            limb+" with his left fist!", ({ ob }));
    return 1;
}

