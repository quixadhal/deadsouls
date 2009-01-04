#include <lib.h>
#include <events.h>

inherit LIB_ITEM;
inherit LIB_ROLL;
inherit LIB_SHAKE;
inherit LIB_CONSULT;

int Denominator = 6;
int Result = 6;
string BaseLong;

int SetDenominator(int denom){
    if(!denom) return;
    else return Denominator = denom;
}

string SetLong(string str){
    item::SetLong(str);
    if(!BaseLong) BaseLong = str;
    return BaseLong;
}

static void create(){
    item::create();
    SetKeyName("die");
    SetId(({"dado","bone", "alea"}));
    SetShort("a six-sided die");
    SetLong("A typical die used for games of chance. It is orange and marked "
            "with numbers rather than dots.");
    SetDenominator(6);
    SetNoCondition(1);
}

void init(){
    item::init();
    if(member_array("d"+Denominator,GetId()) == -1){
        SetId(GetId()+({ "d"+Denominator }) );
    }
}

int doRoll(){
    Result = random(Denominator)+1;
    SetLong(BaseLong + "\nThe die's result is: "+Result+".\n");
    return Result;
}

int eventMove(mixed dest){
    int ret;
    if(ret = (item::eventMove(dest))){
        doRoll();
    }
    return ret;
}

varargs mixed eventResults(mixed args){
    if(intp(args)){
        say("The result of "+this_player()->GetName()+"'s roll is: "+args);
        write("The result of your roll is: "+args);
    }
    return args;
}

varargs mixed eventRoll(object where){
    object dude = this_player();
    string extrathing = "";
    doRoll();
    if(where == environment(this_player())) where = 0;
    if(!where) eventMove(environment(this_player()));
    else {
        if(objectp(where)) extrathing = " on "+where->GetShort();
        if(!inherits(LIB_SURFACE,where) || !eventMove(where)){
            extrathing = "";
            eventMove(environment(this_player()));
        }
    }
    write("You roll your "+strip_article(GetShort())+extrathing+", and the result is: "+Result);
    say(dude->GetName()+" rolls "+possessive(dude)+" "+strip_article(GetShort())+extrathing+", "
            "and the result is: "+Result);
    return Result;
}

varargs mixed eventShake(object dude, string foo){
    write("You shake your "+strip_article(GetShort())+".");
    say(dude->GetName()+" shakes "+possessive(dude)+" "+strip_article(GetShort())+".");
    doRoll();
    return 1;
}

varargs mixed eventConsult(object dude, string foo){
    write("The die result is: \n"+Result);
    say(dude->GetName()+" consults "+possessive(dude)+" "+strip_article(GetShort())+".");
    return 1;
}

varargs mixed CanConsult(object who, string component){
    return 1;
}

varargs mixed CanShake(object who, string component){
    return 1;
}
