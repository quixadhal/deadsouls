/*    /lib/meal.c
 *    from the Dead Souls LPC Library
 *    standard food and drink code
 *    created by descartes of Borg 950603
 *    Version: @(#) meal.c 1.3@(#)
 *    Last modified: 97/01/03
 */
 
#include <lib.h>
#include <function.h>
#include <meal_types.h>
#include <rounds.h>
#include <vendor_types.h>
#include "include/meal.h"
 
inherit LIB_BAIT_WITH;
inherit LIB_ITEM;
 
private int MealStrength, MealType;
private string EmptyItem, EmptyName, OtherMessage;
private mixed EmptyShort, EmptyLong, MyMessage;
 
static void create() {
    item::create();
    MealType = MEAL_ALCOHOL;
    EmptyItem = 0;
    EmptyName = "bottle";
    EmptyShort = "an empty bottle";
    EmptyLong = "This empty bottle once contained some alcohol.";
    MyMessage = "You drink your drink.";
    OtherMessage = "$N drinks $P drink.";
    SetVendorType(VT_DRINK);
    AddSave( ({ "MealStrength", "MealType", "BaitStrength" }) );
}

mixed direct_drink_obj() { 
    if( environment() != this_player() ) return "#You don't have that!";
    if( MealType & MEAL_FOOD ) return "Wouldn't you rather eat it?";
    return (int)this_player()->CanDrink(this_object());
}

mixed direct_drink_from_obj() {
    if( MealType & MEAL_FOOD ) return 0;
    if( environment() != this_player() ) return "#You don't have that!";
    return (int)this_player()->CanDrink(this_object());
}

mixed direct_eat_obj() {
    if( environment() != this_player() ) return "#You don't have that!";
    if( !(MealType & MEAL_FOOD) ) return "Wouldn't you rather drink it?";
    return (int)this_player()->CanEat(this_object());
}

static mixed AddSave(mixed *vars) { return item::AddSave(vars); }

string GetShort() { return item::GetShort(); }

int eventDestruct() { return item::eventDestruct(); }

mixed eventDrink(object who) {
    mixed tmp;
    int x;
 
    if( (tmp = (mixed)who->eventDrink(this_object())) != 1 ) return tmp;
    if( (x = functionp(MyMessage)) && !(x & FP_OWNER_DESTED) ) {
	evaluate(MyMessage, who);
    }
    else {
        string mymsg, othermsg;
 
        mymsg = replace_string(MyMessage, "$P", "your");
        othermsg = replace_string(OtherMessage, "$P", possessive(who));
        mymsg = replace_string(mymsg, "$N", "you");
        othermsg = replace_string(othermsg, "$N", (string)who->GetName());
        who->eventPrint( capitalize(mymsg) );
        environment(who)->eventPrint( capitalize(othermsg), who );
    }
    if( GetEmptyItem() ) {
        object ob;
 
        ob = new(GetEmptyItem() || LIB_ITEM);
        if( base_name(ob) == LIB_ITEM ) {
            ob->SetKeyName(GetEmptyName());
            ob->SetId( ({ GetEmptyName(), "container", "empty container" }) );
            ob->SetShort(GetEmptyShort());
            ob->SetLong(GetEmptyLong());
            ob->SetValue(10);
            ob->SetMass(100);
            ob->SetDestroyOnSell();
        }
        if( !((int)ob->eventMove(who)) ) {
            who->eventPrint("You drop " + (string)ob->GetShort() + ".");
            environment(who)->eventPrint((string)who->GetName() +
                             " drops " + (string)ob->GetShort() + ".", who);
            ob->eventMove(environment(who));
        }
    }
    if( x = GetPoison() ) {
        if( random((int)who->GetStatLevel("luck")) > 35 )
            who->eventPrint("That didn't seem to taste quite right.");
        who->AddPoison(x);
    }
    Destruct();
    return 1;
}
 
mixed eventEat(object who) {
    mixed tmp;
    int x;
 
    if( (tmp = (mixed)who->eventEat(this_object())) != 1 ) return tmp;
    if( (x = functionp(MyMessage)) && !(x & FP_OWNER_DESTED) ) {
	evaluate(MyMessage, who);
    }
    else {
        string mymsg, othermsg;
 
        mymsg = replace_string(MyMessage, "$P", "your");
        othermsg = replace_string(OtherMessage, "$P",
                                  possessive(who));
        mymsg = replace_string(mymsg, "$N", "you");
        othermsg = replace_string(othermsg, "$N", (string)who->GetName());
        who->eventPrint( capitalize(mymsg) );
        environment(who)->eventPrint( capitalize(othermsg), who );
    }
    if( x = GetPoison() ) {
        if( random((int)who->GetStatLevel("luck")) > 35 )
            who->eventPrint("You notice a strange aftertaste.");
        who->AddPoison(x);
    }
    Destruct();
    return 1;
}
 
int eventPoison(object who, object agent, int x) {
    who->eventPrint("You put some poison "
                    "in " + add_article(GetShort()) + ".");
    environment(who)->eventPrint( (string)who->GetName()+" puts something "
                      "in " + add_article(GetShort()) + ".", who);
    AddPoison(x);
    return 1;
}
 
string SetEmptyItem(string file) { return (EmptyItem = file); }
 
string GetEmptyItem() { return EmptyItem; }
 
string SetEmptyName(string str) { return (EmptyName = str); }
 
string GetEmptyName() { return EmptyName; }
 
mixed SetEmptyShort(mixed val) { return (EmptyShort = val); }
 
mixed GetEmptyShort() { return EmptyShort; }
 
mixed SetEmptyLong(mixed val) { return (EmptyLong = val); }
 
mixed GetEmptyLong() { return EmptyLong; }
 
int SetMealType(int x) {
    int vt = 0;
 
    if( x & MEAL_FOOD ) {
        vt |= VT_FOOD;
        MyMessage = "You eat your food.";
        OtherMessage = "$N eats $P food.";
    }
    if( x & MEAL_DRINK ) vt |= VT_DRINK;
    SetVendorType(vt);
    return (MealType = x);
}
 
int GetMealType() { return MealType; }
 
int SetStrength(int x) { return (MealStrength = x); }
 
int GetStrength() { return MealStrength; }
 
varargs void SetMealMessages(mixed array val...) {
    if( !arrayp(val) ) {
	error(sprintf("Bad argument 1 to SetMealMessages(): %O\n", val));
    }
    if( sizeof(val) == 1 ) {
	if( arrayp(val[0]) ) SetMealMessages(val[0]...);
	else if( stringp(val[0]) || functionp(val[0]) ) MyMessage = val[0];
	else error(sprintf("Bad argument 1 to SetMealMessages(): %O\n",
			   val[0]));
	return;
    }
    if( !stringp(val[0]) )
      error(sprintf("Bad argument 1 to SetMealMessages(): %O\n", val[0]));
    else if( !stringp(val[1]) ) 
      error(sprintf("Bad argument 2 to SetMealMessages(): %O\n", val[1]));
    MyMessage = val[0];
    OtherMessage = val[1];
}
 
mixed *GetMealMessages() { return ({ MyMessage, OtherMessage }); }
 
int GetValue() {
    int x;
 
    x = GetStrength();
    return (x * (6 + x)) / 10 + 10;
}
