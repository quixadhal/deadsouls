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

int indirect_sell_obj_to_liv(){ 
    write("Your offer is refused.");
    say(this_player()->GetName()+"'s sell offer is refused.");
    return 0;
}


/* ******************* barkeep.c attributes *********************** */
int GetCost(string *item){
    float f = currency_rate(GetLocalCurrency());

    if( !MenuItems[item] ){
        return 0;
    }
    if( f < 0.1 ){
        f = 1.0;
    }
    return query_value(load_object(MenuItems[item])->GetBaseCost(),query_base_currency(),this_object()->GetLocalCurrency());
}

string GetLocalCurrency(){
    return LocalCurrency;
}

string SetLocalCurrency(string str){
    return (LocalCurrency = str);
}

mapping AddMenuItem(mixed item, string file){
    string *item_arr;
    if(stringp(item)) item_arr = ({ item });
    else item_arr = item;
    MenuItems[item_arr] = file;
    return MenuItems;
}

mapping GetMenuItems(){
    return MenuItems;
}

mapping RemoveMenuItem(string item){
    string *item_arr;
    if(!sizeof(MenuItems)) return MenuItems;
    foreach(string *key, string arr in MenuItems){
        if(member_array(item,key) != -1) item_arr = key;
    }
    if(sizeof(item_arr)) map_delete(MenuItems, item_arr);
    return MenuItems;
}

mapping SetMenuItems(mapping mp){
    mapping mp2 = ([]);
    foreach(mixed key, mixed val in mp){
        string *key2;
        if(stringp(key)) key2 = ({ key });
        else key2 = key;
        mp2[key2] = val;
    }
    return (MenuItems = copy(mp2));
}

/* *********************** barkeep.c modals ************************ */
int CanCarry(int cmt){
    return 1;
}

mixed CanSell(object who, string item){
    string *what = ({});
    foreach(string *key, string val in MenuItems){
        if(member_array(item,key) != -1) what = key;
    }
    if( !MenuItems[what] ){
        return "There is no such thing for sale.";
    }
    return 1;
}

/* *********************** barkeep.c events *********************** */
mixed eventBuyItem(object who, string cmd, string item){
    mixed tmp;
    string *what = ({});
    if( !item || item == "" ){
        eventForce("speak err, what do you want me to sell?");
        return 1;
    }
    item = remove_article(lower_case(item));
    foreach(string *key, string val in MenuItems){
        if(member_array(item,key) != -1) what = key;
    }
    tmp = CanSell(who, item);
    if( tmp != 1 ){
        if( tmp ){
            who->eventPrint(tmp);
        }
        else {
            eventForce("speak I cannot sell right now");
        }
        return 1;
    }
    return eventSell(who, item);
}

mixed eventSell(object who, string args){
    object ob;
    int x;
    string *what;
    foreach(string *key, string val in MenuItems){
        if(member_array(args,key) != -1) what = key;
    }
    if( !(ob = load_object(MenuItems[what])) ){
        eventForce("speak I am having a problem with that item right now.");
        return 1;
    }
    x = query_value(ob->GetBaseCost(),query_base_currency(),GetLocalCurrency());
    if( x > (int)who->GetCurrency(GetLocalCurrency()) ){
        eventForce("speak You do not have that much in " + GetLocalCurrency());
        return 1;
    }
    ob = new(MenuItems[what]);
    if( !ob ){
        eventForce("speak I seem to be having some troubles.");
        return 1;
    }
    if( !((int)ob->eventMove(this_object())) ){
        eventForce("speak Sorry, today is just not my day");
        return 1;
    }
    eventForce("give " + (string)ob->GetKeyName() + " to " +
            (string)who->GetKeyName());
    if( environment(ob) == this_object() ){
        eventForce("speak heh, you cannot carry that.  I will drop it.");
        eventForce("drop " + (string)ob->GetKeyName());
        if( environment(ob) == this_object()){
            ob->eventMove(environment());
        }
    }
    who->AddCurrency(GetLocalCurrency(), -x);
    eventForce("speak Thank you for your business, " +
            (string)who->GetName());
    return 1;
}

int eventList(object who, string cmd, string args){
    string array drinks = ({});
    string *drink;

    if( !sizeof(keys(MenuItems)) ){
        eventForce("speak I have nothing to serve right now.");
        return 1;
    }
    foreach(drink in keys(MenuItems)){
        string array adjectives = MenuItems[drink]->GetAdjectives();
        string adj = "";
        if( sizeof(adjectives) ){
            adj = adjectives[random(sizeof(adjectives))] + " ";
        }
        drinks += ({ adj + drink[0] + " for " + GetCost(drink) });
    }
    eventForce("speak I currently supply " + item_list(drinks) + ".");
    eventForce("speak Prices are in " + GetLocalCurrency() + " of course.");
    return 1;
}

/* ********************* barkeep.c driver applies *********************** */
static void create(){
    sentient::create();
    SetCommandResponses( ([
                ({ "list", "show", "browse" }) : (: eventList :),
                ({ "sell", "serve" }) : (: eventBuyItem :),
                ]) );
    SetRequestResponses( ([
                ({ "menu" }) : (: eventList :),
                ]) );
}
