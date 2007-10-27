#include <lib.h>
#include <rooms.h>

inherit LIB_ITEM;

int SetBonuses();

mapping Skills = ([]);
mapping Stats = ([]);
mapping Points = ([]);
int Duration = 15;

void create(){
    item::create();
    SetInvis(1);
    SetId("bonus_object");
    SetShort("bonus");
    SetLong("A bonus");
}

void init(){
    item::init();
    set_heart_beat(1);
    if(environment(this_object()) && living(environment(this_object()))) 
        SetBonuses();
}

void heart_beat(){
    if(Duration) Duration--;
    else eventDestruct();
}

mapping SetStats(mapping arg){
    Stats = copy(arg);
    return copy(Stats);
}

mapping GetStats(){
    return copy(Stats);
}

mapping SetSkills(mapping arg){
    Skills = copy(arg);
    return copy(Skills);
}

mapping GetSkills(){
    return copy(Skills);
}

mapping SetPoints(mapping arg){
    Points = copy(arg);
    return copy(Points);
}

mapping GetPoints(){
    return copy(Points);
}

int SetBonusDuration(int i){
    Duration = i;
}

int GetBonusDuration(){
    return Duration;
}

int SetBonuses(){
    object env = environment();
    if(!env || ! living(env)) return 0;
    if(sizeof(Stats))
        foreach(string key, int val in Stats){
        env->AddStatBonus(key, val);
    }
    if(sizeof(Skills))
        foreach(string key, int val in Skills){
        env->AddSkillBonus(key, val);
    }
    if(sizeof(Points))
        foreach(string key, int val in Points){
        switch(key){
        case "HP" : env->AddHp(val);break;
        case "XP" : env->AddExperiencePoints(val);break;
        case "SP" : env->AddStaminaPoints(val);break;
        case "MP" : env->AddMagicPoints(val);break;
        default : break;
        }
    }
    return 1;
}

int RemoveBonuses(){
    object env = environment();
    if(!env || ! living(env)) return 0;
    if(sizeof(Stats))
        foreach(string key, int val in Stats){
        env->RemoveStatBonus(key);
    }
    if(sizeof(Skills))
        foreach(string key, int val in Skills){
        env->RemoveSkillBonus(key);
    }
    return 1;
}

int eventDestruct(){
    RemoveBonuses();
    this_object()->eventMove(ROOM_FURNACE);
    return ::eventDestruct();
}

mixed CanGet(object who) { return 0; }
mixed CanGive(object who) { return 0; }
mixed CanSell(object who) { return 0; }
mixed CanDrop(object who) { return 0; }
mixed CanPut(object who) { return 0; }
