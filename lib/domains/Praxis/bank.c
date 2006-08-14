//	/d/standard/bank.c
//	from the Nightmare mudlib
//	banking room
//	created by Descartes of Borg 28 february 1993

#include <lib.h>
#include <daemons.h>
//#include <money.h>
//#include <bank.h>

#define BANK_ID "praxis"

//inherit LIB_ROOM;
inherit LIB_ROOM;

void init() {
    ::init();
    add_action("Bugga", "Bugga");
    add_action("read", "read");
    add_action("open", "open");
    add_action("close", "close");
    add_action("deposit", "deposit");
    add_action("withdraw", "withdraw");
    add_action("balance", "balance");
    add_action("exchange", "exchange");
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
    SetDoor("vault", "/domains/Praxis/bank_vault", "north", "bank key");
    set_func("vault", "unlock", "do_drunkard");
    present("guard")->eventForce("close vault");
    present("guard")->eventForce("lock vault with key");
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
	present("guard")->eventForce("close vault");
	present("guard")->eventForce("lock vault with key");
    }
}

void look_at_vault() {
    write("A huge, secure door that protects the bank's money.");
    if(GetOpen("vault")) 
	write("But someone has managed to get it open!");
    else write("It is sealed shut right now.");
}

int do_drunkard() {
    if(present("guard") && !this_player()->query_invis()) {
	present("guard")->eventForce("kill "+(string)this_player()->query_name());
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

int exchange(string str) {
    string from, to;
    float val;
    int amount;

    if(!str) {
	notify_fail("Correct syntax: <exchange # type for type>\n"+
	  "ex: <exchange 100 gold for copper>\n");
	return 0;
    }
    if(sscanf(str, "%d %s for %s", amount, from, to) !=3) {
	notify_fail("Correct syntax: <exchange # type for type\nex: "
	  "<exchange 100 gold for copper>\n");
	return 0;
    }
    if(amount < 1) {
	notify_fail("That would be a nifty trick indeed!\n");
	return 0;
    }
    if((int)this_player()->query_money(from) < amount) {
	notify_fail("You do not have that much of that currency.\n");
	return 0;
    }
    if(member_array(from, HARD_CURRENCIES) == -1)
	return notify_fail("The bank does not support "+from+".\n");
    val = amount/currency_rate(from);
    if(member_array(to, HARD_CURRENCIES) == -1)
	return notify_fail("The bank does not support "+to+".\n");
    this_player()->AddCurrency(from, -(to_int(amount*0.01)+amount));
    amount = to_int(val * currency_rate(to));
    this_player()->AddCurrency(to, amount);
    message("my_action", "You exchange your "+from+" coins for "+amount+" "+to,
      this_player());
    message("other_action", (string)this_player()->query_cap_name()+
      " exchanges some money.", this_object(), ({ this_player() }));
    return 1;
}

int close(string str) {
    int i;
    mapping money;

    if(!str || str != "account") return 0;
    money = (mapping)BANK_D->query_balance(
      (string)this_player()->query_name(), BANK_ID
    );
    if(!money || money == ([])) {
	write("The teller says: You have no account here to close!");
	say(this_player()->query_cap_name()+" tries to close a "
	  "non-existent account!");
	return 1;
    }
    for(i=0; i<sizeof(HARD_CURRENCIES); i++) {
	if(money[HARD_CURRENCIES[i]]) 
	    this_player()->AddCurrency(HARD_CURRENCIES[i], money[HARD_CURRENCIES[i]]);
    }
    write("The teller hands you all your money.");
    say(sprintf("%s collects all %s money from the teller.",
	(string)this_player()->query_cap_name(), possessive(this_player())));
    BANK_D->close_account((string)this_player()->query_name(), BANK_ID);
    return 1;
}

int open(string str) {
    int x;

    if(!str || str != "account") return 0;
    x = (int)BANK_D->open_account((string)this_player()->query_name(), BANK_ID);
    if(x != TRANSACTION_OK) {
	write("You already have an account here!");
	return 1;
    }
    write("You open up an account at the Praxis Merchant's Bank.");
    say(this_player()->query_cap_name()+" opens an account.");
    return 1;
}

int balance(string str) {
    mapping money;
    int i;

    money = (mapping)BANK_D->query_balance(
      (string)this_player()->query_name(), BANK_ID
    );
    if(!money || money == ([])) {
	write("The teller says: You have no account here!");
	return 1;
    }
    say(this_player()->query_cap_name()+" asks for account information.");
    /* why does this not work?
	write("Last transaction: ");
	if(!money["time"]) write("Opening your account.\n");
	else {
	    if(!money["transaction"]) write("Balance (");
	    else if(money["transaction"] < 0) write("Withdrawal (");
	    else write("Deposit: (");
	    write(ctime(money["time"])+")");
	}
    */
    for(i=0; i<sizeof(HARD_CURRENCIES); i++) {
	write(capitalize(HARD_CURRENCIES[i])+": "+
	  money[HARD_CURRENCIES[i]]+" coins\n");
    }
    return 1;
}

int deposit(string str) {
    string type;
    int x, amount;

    if(!str) {
	notify_fail("Correct syntax: <deposit [amount] [type]>\n");
	return 0;
    }
    if(sscanf(str, "%d %s", amount, type) != 2) {
	notify_fail("Correct syntax: <deposit [amount] [type]>\n");
	return 0;
    }
    if((int)this_player()->query_money(type) < amount) {
	notify_fail("The teller says: You do not have that much money!\n");
	return 0;
    }
    x = (int)BANK_D->deposit(
      (string)this_player()->query_name(), BANK_ID, amount, type
    );
    if(x != TRANSACTION_OK) {
	switch(x) {
	case NO_ACCOUNT:
	    notify_fail("The teller says: You have no account here!\n");
	    break;
	case BAD_MONEY:
	    notify_fail("The teller says: That is not a real money type!\n");
	    break;
	default: notify_fail("The teller says: You can't do that!\n");
	}
	return 0;
    }
    this_player()->AddCurrency(type, -amount);
    write("The teller takes your "+amount+" "+type+" coins.");
    say(this_player()->query_cap_name()+" deposits some money.");
    return 1;
}

int withdraw(string str) {
    string type;
    int x, amount;

    if(!str) {
	notify_fail("Correct syntax: <deposit [amount] [type]>\n");
	return 0;
    }
    if(sscanf(str, "%d %s", amount, type) != 2) {
	notify_fail("Correct syntax: <deposit [amount] [type]>\n");
	return 0;
    }
    if(amount < 0) {
	notify_fail("The teller says: That would be a neat trick!\n");
	return 0;
    }
    x = (int)BANK_D->withdraw(
      (string)this_player()->query_name(), BANK_ID, amount, type
    );
    if(x != TRANSACTION_OK) {
	switch(x) {
	case NO_ACCOUNT:
	    notify_fail("The teller says: You have no account here!\n");
	    break;
	case BAD_MONEY:
	    notify_fail("The teller says: That is not a real money type!\n");
	    break;
	default: notify_fail("The teller says: You can't do that!\n");
	}
	return 0;
    }
    this_player()->AddCurrency(type, amount);
    write("The teller gives you your "+amount+" "+type+" coins.");
    say(this_player()->query_cap_name()+" withdraws some money.");
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

int pick_lock(string str) {
    if(present("guard") && this_player())
	present("guard")->eventForce("kill "+(string)this_player()->query_name());
    this_player()->add_follower(present("guard"));
    return ::pick_lock(str);
}

int Bugga(string str) {
    string limb;
    object ob;
    int amount;

    if(this_player()->is_player()) return 0;
    ob = (object)this_player()->query_current_attacker();
    if(!ob) return 1;
    limb = (string)ob->return_limb();
    if((string)ob->query_class() == "rogue") amount = 24 + random(30);
    else amount = 20 + random(20);
    ob->do_damage(limb, amount);
    tell_object(ob, "The guard bashes your "+limb+" with his "
      "left fist!");
    tell_room(this_object(), "The guard bashes "+ob->query_cap_name()+"'s "+
      limb+" with his left fist!", ({ ob }));
    return 1;
}

