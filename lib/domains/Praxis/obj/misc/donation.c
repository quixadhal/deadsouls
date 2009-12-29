inherit "/lib/std/item";
//#include <security.h>
#include <council.h>
#define LOG "/realms/nialson/data/mage_coffer_log"

int loaded, stored_amount;

void init() {
    ::init();
    add_action("donate", "donate");
    add_action("withdraw", "withdraw");
}

void create() {
    ::create();
    loaded=stored_amount=0;
    SetKeyName("donation box");
    SetId( ({ "donation box", "box" }) );
    SetShort("a donation box");
    SetRead("Property of Frobitz collection agency.");
    SetMass(0);
    SetValue(0);
    SetPreventGet("You aren't allowed to take that. No one is.");
    SetNoClean(1);
}

int restore_int(string str) {
    string tmp;
    string repl;
    int value;

    tmp=absolute_path("/realms/nialson/data", str);
    repl=read_file(tmp, 1, 1);
    sscanf(repl, "%d\n", value);
    return value;   
}

void save_int(string str, int value) {
    string tmp;

    tmp=absolute_path("/realms/nialson/data", str);
    rm(tmp);
    write_file(tmp, ""+value+"\n");
}

int donate(string str) {
    int amount;

    if(!str) {
        notify_fail("Donate what?\n");
        return 0;
    }
    if(sscanf(str, "%d gold", amount) !=1) {
        notify_fail("Correct syntax: <donate [#] gold>\n");
        return 0;
    }
    if(amount < 1) {
        notify_fail("That would be a nifty trick indeed!\n");
        return 0;
    }
    if(this_player()->query_money("gold") < amount) {
        notify_fail("You don't have that much gold!\n");
        return 0;
    }
    if (!loaded) {
        stored_amount = restore_int("mage_coffers");
        loaded=1;}
        this_player()->add_money("gold", -1*amount);
        write("You donate "+amount+" gold coins to the mages.");
        say(this_player()->query_cap_name()+" donates some gold.", this_player());
        stored_amount += amount;
        write_file(LOG,""+stored_amount+"\t"+
                this_player()->query_name()+" donates ."+amount+"\n");
        save_int("mage_coffers", stored_amount);
        return 1;
}

string GetLong(string junk) {
    if (!loaded) {
        stored_amount = restore_int("mage_coffers");
        loaded=1;}
        return
            "A medium sized, VERY heavy box that contains the funds of the mage class.\n"+
            "If you wish, you can <donate # gold> to increase those funds.\n"+
            "The coffers currently contain "+stored_amount+" gold.\n";
}

int withdraw(string str) {
    int amount;
#define TESTERS ({"nialson", "lassondra", "zaknaifen"})
    if((-1==member_array(this_player()->query_name(), TESTERS))&&
            (-1==member_array(this_player()->query_name(), MAGE_COUNCIL))) {
        notify_fail("Only council members may withdraw money.\n");
        return 0; }
        if(!str) {
            notify_fail("Withdraw what?\n");
            return 0; }
            if(sscanf(str, "%d gold", amount) !=1) {
                notify_fail("Correct syntax: <withdraw [#] gold>\n");
                return 0; }
                if (!amount) {
                    notify_fail("Stop wasting my time and yours.\n");
                    return 0; }
                    if (amount < 0) {
                        notify_fail("Negatives not allowed!\n");
                        return 0; }
                        if (!loaded) {
                            stored_amount = restore_int("mage_coffers");
                            loaded=1;}

                            write("You withdraw "+amount+" gold coins from the mages.");
                            say(this_player()->query_cap_name()+" withdraws some gold.", this_player());
                            stored_amount -= amount;
                            this_player()->add_money("gold", amount);
                            write_file(LOG,""+stored_amount+"\t"+
                                    this_player()->query_name()+" withdrew ."+amount+"\n");
                            save_int("mage_coffers", stored_amount);
                            return 1;
}

