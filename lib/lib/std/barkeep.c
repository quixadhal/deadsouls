/*    /lib/std/barkeep.c
 *    From the Dead Souls LPC Library
 *    A monster which sells food and drink
 *    Created by Descartes of Borg 950528
 *    Version: @(#) barkeep.c 1.2@(#)
 *    Last modified: 97/01/03
 */

#include <lib.h>
 
inherit LIB_SENTIENT;
inherit LIB_BUY;

private string  LocalCurrency = "gold";
private mapping MenuItems     = ([]);

string GetLocalCurrency();
mixed eventSell(object who, string args);

/* ******************* barkeep.c attributes *********************** */
int GetCost(string item) {
    float f = currency_rate(GetLocalCurrency());
    
    if( !MenuItems[item] ) {
	return 0;
    }
    if( f < 0.1 ) {
	f = 1.0;
    }
    return to_int(to_float(MenuItems[item]->GetValue()) * f);
}

string GetLocalCurrency() {
    return LocalCurrency;
}
 
string SetLocalCurrency(string str) {
    return (LocalCurrency = str);
}
 
mapping AddMenuItem(string item, string file) {
    MenuItems[item] = file;
    return MenuItems;
}
 
mapping GetMenuItems() {
    return MenuItems;
}
 
mapping RemoveMenuItem(string item) {
    map_delete(MenuItems, item);
    return MenuItems;
}
 
mapping SetMenuItems(mapping mp) {
    return (MenuItems = mp);
}

/* *********************** barkeep.c modals ************************ */
int CanCarry(int cmt) {
    return 1;
}

mixed CanSell(object who, string what) {
    if( !MenuItems[what] ) {
	return "There is no such thing for sale.";
    }
    return buy::CanSell(who, what);
}

/* *********************** barkeep.c events *********************** */
mixed eventBuyItem(object who, string cmd, string args) {
    mixed tmp;
    
    if( !args || args == "" ) {
        eventForce("speak err, what do you want me to sell?");
        return 1;
    }
    args = remove_article(lower_case(args));
    tmp = CanSell(who, args);
    if( tmp != 1 ) {
	if( tmp ) {
	    who->eventPrint(tmp);
	}
	else {
	    eventForce("speak I cannot sell right now");
	}
	return 1;
    }
    return eventSell(who, args);
}

mixed eventSell(object who, string args) {
    object ob;
    int x;
 
    if( !(ob = load_object(MenuItems[args])) ) {
        eventForce("speak I am having a problem with that item right now.");
        return 1;
    }
    x = GetCost(args);
    if( x > (int)who->GetCurrency(GetLocalCurrency()) ) {
        eventForce("speak You do not have that much in " + GetLocalCurrency());
        return 1;
    }
    ob = new(MenuItems[args]);
    if( !ob ) {
        eventForce("speak I seem to be having some troubles.");
        return 1;
    }
    if( !((int)ob->eventMove(this_object())) ) {
        eventForce("speak Sorry, today is just not my day");
        return 1;
    }
    eventForce("give " + (string)ob->GetKeyName() + " to " +
               (string)who->GetKeyName());
    if( environment(ob) == this_object() ) {
        eventForce("speak heh, you cannot carry that.  I will drop it.");
        eventForce("drop " + (string)ob->GetKeyName());
        if( environment(ob) == this_object()) {
	    ob->eventMove(environment());
	}
    }
    who->AddCurrency(GetLocalCurrency(), -x);
    eventForce("speak Thank you for your business, " +
               (string)who->GetName());
    return 1;
}

int eventList(object who, string cmd, string args) {
    string array drinks = ({});
    string drink;

    if( !sizeof(keys(MenuItems)) ) {
        eventForce("speak I have nothing to serve right now.");
        return 1;
    }
    foreach(drink in keys(MenuItems)) {
	string array adjectives = MenuItems[drink]->GetAdjectives();
	string adj = "";
	
	if( sizeof(adjectives) ) {
	    adj = adjectives[random(sizeof(adjectives))] + " ";
	}
	drinks += ({ adj + drink + " for " + GetCost(drink) });
    }
    eventForce("speak I currently supply " + item_list(drinks) + ".");
    eventForce("speak Prices are in " + GetLocalCurrency() + " of course.");
    return 1;
}

/* ********************* barkeep.c driver applies *********************** */
static void create() {
    sentient::create();
    SetCommandResponses( ([
         ({ "list", "show", "browse" }) : (: eventList :),
         ({ "sell", "serve" }) : (: eventBuyItem :),
     ]) );
}
