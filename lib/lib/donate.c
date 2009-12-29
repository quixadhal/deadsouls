/*  /lib/donate.c
 *  a donation box inheritable
 *  created by BillGates@Dead Souls 961023
 */

#include "include/donate.h"
#include <lib.h>

private int TotalAssets;
private string LocalCurrency, Owner;

static void create(){
    this_object()->AddSave( ({ "TotalAssets" }) );
    TotalAssets = 0;
}

static void init(){
    if( this_player()->ClassMember(GetOwner()) ||
            this_player()->GetClan() == GetOwner() ){
        add_action( (: eventDonate :) , "donate" );
    }
}

string GetLong(string str){
    string ret = "\nIt currently contains "+TotalAssets+" " +
        "coins and bills.";
    if(TotalAssets) return ret;
    else return "";
}

string SetLocalCurrency(string currency){
    return (LocalCurrency = currency);
}

string GetLocalCurrency(){return LocalCurrency;}

string SetOwner(string owner){return (Owner = owner);}

string GetOwner(){return Owner;}   

int AddAssets(int amount){
    TotalAssets += amount;
    return 1;
}

int GetAssets(){return TotalAssets;}   

int eventDonate(string amt, string type){
    int amount;
    object tp = this_player();   

    if(sscanf(amt,"%d", amount) < 1){
        write("That isn't a proper amount.");
        return 1;
    }

    if(!valid_currency(type)){
        write("That isn't a valid currency.");
        return 1;
    }

    if(!amount){
        tp->eventPrint("Donate how much?\n");
        return 0;
    }   
    if(amount < 0){
        tp->eventPrint("Nice try!\n");
        return 0;
    }
    if(tp->GetCurrency(type) < amount){
        tp->eventPrint("You don't have that much " + type + "!\n");
        return 0;
    }
    tp->AddCurrency(type, -amount);
    AddAssets(amount);
    tp->eventPrint(GetOwner() + " is grateful "
            "for your donation.");
    if(living(this_object())) this_object()->eventForce("say Thank you!");
    return 1;
}
