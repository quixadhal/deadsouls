#include <lib.h>
#include <rooms.h>
inherit LIB_ITEM;

int traptype = 0;
int traplevel = 0;
string shadow_object = "";

int SetTrapType(int i){
    traptype = i;
    return traptype;
}

int GetTrapType(){
    return traptype;
}

string SetShadowObject(string str){
    if(last(str,2) == ".c") str = truncate(str,2);
    if(!file_exists(str+".c")) error("Cant find  shadow file: "+str+".c");
    if(strsrch(str,"/shadows/")) error("Invalid location for a shadow file.");
    shadow_object = str;
    return shadow_object;
}

string GetShadowObject(){
    return shadow_object;
}

void create(){
    item::create();
    SetId( ({ "trap", "sample trap" }) );
    SetAdjectives( ({ "simple","sample" }) );
    SetShort("a sample trap");
    SetLong("It is a simple sample trap.");
    SetMass(50);
    SetBaseCost(500);
    SetNoCondition(1);
}

void init(){
    item::init();
}

varargs int CanBoobytrap(object who, mixed what){
    return 1;
}

varargs int eventBoobytrap(object who, mixed target, mixed trap){
    object shadowtrap;
    if(stringp(target)) target = get_object(target);
    if(!target){
        write("It seems that cannot be boobytrapped.");
        return 1;
    }
    if(living(target)){
        write("You cannot boobytrap a living thing.");
        return 1;
    }
    shadowtrap = new(shadow_object);
    if(!shadowtrap){
        write("There is somethign wrong with the boobytrap. You fail to set it.");
        return 1;
    }
    traplevel = 0;
    if(this_player()->GetSkill("concealment"))
        traplevel += this_player()->GetSkill("concealment")["level"];
    traplevel += random(this_player()->GetStat("luck")["level"]);
    traplevel += this_player()->GetStat("coordination")["level"];

    shadowtrap->SetTrapDescription(this_object()->GetLong());
    shadowtrap->SetTrapType(traptype);
    shadowtrap->SetTrapLevel(traplevel);
    shadowtrap->eventShadow(target);
    write("You boobytrap "+target->GetShort()+" with your "+remove_article(trap->GetShort())+".");
    say(this_player()->GetCapName()+" seems to attach something to "+target->GetShort()+".");
    this_object()->eventMove(ROOM_FURNACE);
    return 1;
}

