/*  /lib/donate.c
 *  a donation box inheritable
 *  created by BillGates@Dead Souls 961023
 */

#include "include/donate.h"
#include <lib.h>

inherit LIB_ITEM;

private int TotalAssets;
private string LocalCurrency, Owner;

static void create() {
   item::create();
   this_object()->AddSave( ({ "TotalAssets" }) );
   TotalAssets = 0;
   SetPreventGet(1);
   SetMass(99999);
}

static void init() {
   item::init();
   if( (string)this_player()->ClassMember((string)GetOwner()) ||
       (string)this_player()->GetGuild() == (string)GetOwner() ) {
	  add_action( (: eventDonate :) , "donate" );
   }
}

string GetLong(string str) {
   string ret;
   ret = item::GetLong();
   ret += "\nIt currently contains "+TotalAssets+" " +
	(string)GetLocalCurrency() + ".";
   return ret;
}

string SetLocalCurrency(string currency) {
   return (LocalCurrency = currency);
}

string GetLocalCurrency() {return LocalCurrency;}

string SetOwner(string owner) {return (Owner = owner);}

string GetOwner() {return Owner;}   

int AddAssets(int amount) {
   TotalAssets += amount;
   return 1;
}

int GetAssets() {return TotalAssets;}   

int eventDonate(int amount, string type) {
   object tp = this_player();   

   if(!amount) {
     tp->eventPrint("Donate how much?\n");
     return 0;
   }   
   if(amount < 0) {
     tp->eventPrint("Nice try!\n");
     return 0;
   }
   if(type != GetLocalCurrency()) {
     tp->eventPrint("The " + pluralize(GetOwner()) +
	 " only deal in " + GetLocalCurrency() +
     	 ".\n");
     return 0;
   }
   if((int)tp->GetCurrency(type) < amount) {
     tp->eventPrint("You don't have that much " + type + "!\n");
     return 0;
   }
   tp->AddCurrency(type, -amount);
   AddAssets(amount);
   tp->eventPrint("The " + pluralize(GetOwner()) + " are greatful "
	"for your donation.");
   return 1;
}

