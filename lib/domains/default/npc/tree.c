#include <lib.h>
inherit LIB_NPC;

static void create(){
    npc::create();
    SetKeyName("money tree");
    SetId(({"tree","money tree","arborus argentum"}));
    SetShort("a money tree");
    SetLong("This robust, 4-foot tall tree is the fabled arborus argentum, or money "+
            "tree. Its leaves are fresh, new dollar bills, and its seed-bearing "+
            "pods are full to bursting with coins of all kinds. It would be no "+
            "trouble at all to get <amount> <currency> from tree...looks like "+
            "you've hit the jackpot!");
    SetPacifist(1);
    SetCanBite(0);
    SetLevel(99);
    SetRace("tree");
    SetHealthPoints(99999);
    SetMaxHealthPoints(99999);
    SetPreventGet(1);
}

void init(){
    ::init();
    add_action("make_money","get");
}

int make_money(string str){
    int d1;
    string s1,s2;
    if(!creatorp(this_player()) && !member_group(this_player(),"TEST")) {
        write("Your hands seem to slide off the tree...as if somehow "
                "it knows this money shouldn't fall into your greedy hands.");
        return 1;
    }
    if(!str){ write("Get what?\n"); return 1; }
    if(sscanf(str,"%d %s from %s",d1,s1,s2) > 0){
        if(s1 !="electrum" && s1 != "platinum" &&
                s1 !="gold" && s1 !="cents" &&
                s1 !="dollars" && s1 !="copper" &&
                s1 !="silver"){
            write("That kind of money doesn't grow on trees!\n");
            return 1;
        }
        if(s2=="tree" || s2=="the tree" ||
                s2=="the money tree" || s2=="arborus argentum"){
            if(d1 > 1000000){
                write("That is too much money to get from the tree at once.\n");
                return 1;
            }
            this_player()->AddCurrency(s1, d1);
            write("You get "+d1+" "+s1+" from the money tree.");
            say(this_player()->GetCapName()+" gets money from the money tree.");
            return 1;
        }
        return 0;
    }
}
