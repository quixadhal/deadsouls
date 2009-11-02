/*    /lib/comp/holder.c
 *    From the Dead Souls Mud Library
 *    A composite object that holds inanimate items
 *    Created by Descartes of Borg 960113
 *    Version: @(#) holder.c 1.6@(#)
 *    Last modified: 96/12/22
 */

#include <lib.h>

inherit LIB_CARRY;      // Handles capacity issues
inherit LIB_CONTAINER;  // Determines whether something can be contained
inherit LIB_GET_FROM;   // Let's players things from it and put them in
inherit LIB_INVENTORY;  // Let's cres specify an initial inventory

private static int Persist = 0;

int isBag(){
    return 1;
}

int GetOpacity(){
    return container::GetOpacity();
}

int SetOpacity(int x){
    return container::SetOpacity(x);
}

varargs string GetInternalDesc(){
    object array items = all_inventory();
    string desc;
    int surfacep;

    surfacep = inherits(LIB_SURFACE , this_object());

    desc = (container::GetInternalDesc() || "");
    if(this_object()->CanClose() && GetOpacity() > 33){
        if(this_object()->GetClosed()) desc += " It is closed. ";
        else desc += " It is open. ";
    }
    if(!surfacep) desc = desc + capitalize(add_article(GetShort(), 1));
    if(surfacep) desc = "On "+add_article(GetShort(), 1);
    items = filter(items, (: !($1->isDummy()) && !($1->GetInvis()) :));
    if( sizeof(items) ){
        if(surfacep){ 
            desc = desc+" you see " + item_list(items) + ".";
        }
        else desc = desc + " contains " + item_list(items) + ".";
    }
    else {
        if(!surfacep) desc = desc + " is completely empty.";
        else desc = desc + " you see nothing.";
    }
    return desc;
}

static mixed array AddSave(mixed array vars){
    return ({});
}

int SetSaveRecurse(int x){
    return 0;
}

int CanReceive(object ob){
    if( !CanCarry(ob->GetMass()) ){
        return 0;
    }
    return container::CanReceive(ob);
}

static void eventLoadInventory(){
    if( !environment() || Persist ){
        return;
    }
    Persist = 1;
    inventory::eventLoadInventory();
}

int eventReceiveObject(object ob){
    mixed tmp = container::eventReceiveObject(ob);

    if( tmp != 1 ){
        return tmp;
    }
    AddCarriedMass(ob->GetMass());
    if(environment()) environment()->AddCarriedMass(ob->GetMass());
    parse_refresh();
    return 1;
}

int eventReleaseObject(object ob){
    int x;

    x = container::eventReleaseObject(ob);
    if(ob->GetMass()){
        AddCarriedMass(-(ob->GetMass()));
        if(environment()) environment()->AddCarriedMass(-(ob->GetMass()));
    }
    parse_refresh();
    return x;
}

static void create(){
    AddSave(({ "Persist" }));
    SetSaveRecurse(1);
    call_out((: reset :), 0);
}

mixed inventory_visible(){
    return container::inventory_visible();
}
