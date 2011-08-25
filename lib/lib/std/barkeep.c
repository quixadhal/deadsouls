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
private mapping SpecialMenuItems     = ([]);

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
    object ob;
    string obstr;
    if( !obstr = MenuItems[item] ){
        obstr = SpecialMenuItems[item];
    }
    if(!obstr || !(ob = load_object(obstr))){
        return 0;
    }
    if( f < 0.1 ){
        f = 1.0;
    }
    return query_value(ob->GetBaseCost(),query_base_currency(),this_object()->GetLocalCurrency());
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

mapping AddSpecialMenuItem(mixed item, string file){
    string *item_arr;
    if(stringp(item)) item_arr = ({ item });
    else item_arr = item;
    SpecialMenuItems[item_arr] = file;
    return SpecialMenuItems;
}

mapping GetMenuItems(){
    return MenuItems;
}

mapping GetSpecialMenuItems(){
    return SpecialMenuItems;
}

string array GetSpecials(){
    return keys(SpecialMenuItems);
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

mapping RemoveSpecialMenuItem(string item){
    string *item_arr;
    if(!sizeof(SpecialMenuItems)) return SpecialMenuItems;
    foreach(string *key, string arr in SpecialMenuItems){
        if(member_array(item,key) != -1) item_arr = key;
    }
    if(sizeof(item_arr)) map_delete(SpecialMenuItems, item_arr);
    return SpecialMenuItems;
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

mapping SetSpecialMenuItems(mapping mp){
    mapping mp2 = ([]);
    foreach(mixed key, mixed val in mp){
        string *key2;
        if(stringp(key)) key2 = ({ key });
        else key2 = key;
        mp2[key2] = val;
    }
    return (SpecialMenuItems = copy(mp2));
}

/* *********************** barkeep.c modals ************************ */
int CanCarry(int cmt){
    return 1;
}

mixed CanSell(object who, string item){
    string *what = ({});
    mapping FullMenu = add_maps(MenuItems, SpecialMenuItems);
    foreach(string *key, string val in FullMenu){
        if(member_array(item,key) != -1) what = key;
    }
    if( !FullMenu[what] ){
        return "There is no such thing for sale.";
    }
    return 1;
}

/* *********************** barkeep.c events *********************** */
mixed eventBuyItem(object who, string cmd, string item){
    mixed tmp;
    if( !item || item == "" ){
        eventForce("speak err, what do you want me to sell?");
        return 1;
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
    mapping FullMenu = add_maps(MenuItems, SpecialMenuItems);
    foreach(string *key, string val in FullMenu){
        if(member_array(args,key) != -1) what = key;
    }
    if( !(ob = load_object(FullMenu[what])) ){
        eventForce("speak I am having a problem with that item right now.");
        return 1;
    }
    x = query_value(ob->GetBaseCost(),query_base_currency(),GetLocalCurrency());
    if( x > who->GetCurrency(GetLocalCurrency()) ){
        eventForce("speak You do not have that much in " + GetLocalCurrency());
        return 1;
    }
    ob = new(FullMenu[what]);
    if( !ob ){
        eventForce("speak I seem to be having some troubles.");
        return 1;
    }
    if( !(ob->eventMove(this_object())) ){
        eventForce("speak Sorry, today is just not my day");
        return 1;
    }
    eventForce("give " + ob->GetKeyName() + " to " +
            who->GetKeyName());
    if( environment(ob) == this_object() ){
        eventForce("speak heh, you cannot carry that.  I will drop it.");
        eventForce("drop " + ob->GetKeyName());
        if( environment(ob) == this_object()){
            ob->eventMove(environment());
        }
    }
    who->AddCurrency(GetLocalCurrency(), -x);
    eventForce("speak Thank you for your business, " +
            who->GetName());
    return 1;
}

int eventList(object who, string cmd, string args, int special){
    string array drinks = ({});
    string *drink;
    mapping Menu = (special ? SpecialMenuItems : MenuItems);
    if( !sizeof(keys(Menu)) ){
        eventForce("speak I have nothing to serve right now.");
        return 1;
    }
    foreach(drink in keys(Menu)){
        string array adjectives = Menu[drink]->GetAdjectives();
        string adj = "";
        if( sizeof(adjectives) ){
            adj = adjectives[random(sizeof(adjectives))] + " ";
        }
        //drinks += ({ adj + drink[0] + " for " + GetCost(drink) });
        drinks += ({ drink[0] + " for " + GetCost(drink) });
    }
    eventForce("speak I currently supply " + item_list(drinks) + ".");
    eventForce("speak Prices are in " + GetLocalCurrency() + " of course.");
    return 1;
}

int eventSpecialList(object who, string cmd){
    return eventList(who, cmd, "", 1);
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
                ({ "special menu" }) : (: eventSpecialList :),
                ]) );
}
