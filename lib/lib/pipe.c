#include <lib.h>

inherit LIB_ITEM;
inherit LIB_SMOKE;
inherit LIB_BURN;

private int BurnRate = 5;
int lastpuff;

static void create(){
    item::create();
    burn::create();
}

static void heart_beat(){
    eventDecreaseFuel(1);
    if( GetFuelAmount() < 1){
        eventBurnOut();
    }
}

mixed direct_light_obj(){
    mixed tmp = CanBurn(this_player());

    if( tmp == 1 ){
        return "Light it with what?";
    }

    if( GetFuelAmount() < 1 )
        return "There is no " + GetFuelType() + " in " + GetShort() + ".";
    return 1;
}

mixed eventDarken(){
    burn::eventDarken();
    set_heart_beat(0);
    return 1;
}

varargs mixed eventLight(object who, object tool){
    if( GetLit() ) return burn::eventLight(who, tool);
    burn::eventLight(who, tool);
    set_heart_beat(BurnRate);
    return 1;
}

int GetBurnRate(){
    return BurnRate;
}

static int SetBurnRate(int x){
    return (BurnRate = x);
}

int GetRadiantLight(int ambient){
    if( !GetLit() ) return 0;
    else return item::GetRadiantLight(ambient);
}

string GetShort(){
    return item::GetShort() + burn::GetShort();
}

varargs string GetLong(string unused){
    string tmp;

    tmp = burn::GetLong(unused);
    if( tmp != "" ) return item::GetLong(unused) + "\n" + tmp;
    else return item::GetLong(unused);
}

int GetLastPuff(){
    return lastpuff;
}

mixed eventSmoke(object who, object what){
    if(!GetLit()){
        write("It is not lit!");
        return 1;
    }
    write("You smoke your "+remove_article(GetShort())+".");
    say(who->GetName()+" smokes from "+possessive(who)+" "
            +remove_article(GetShort())+".");
    lastpuff = time();
    eventDecreaseFuel(1);
    if( !GetFuelAmount() ) eventBurnOut();
    return 1;
}
