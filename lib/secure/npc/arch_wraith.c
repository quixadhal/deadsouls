#include <position.h>
#include <lib.h>
#include ROOMS_H

inherit LIB_SENTIENT;
object *enemies = ({});
object quarry;
int draining, self_destruct;
string qname, ip;

int eventDrain(mixed args...){
    int avoid;
    if(!environment()) return 1;
    enemies = this_object()->GetEnemies();
    if(quarry && draining) enemies += ({ quarry });
    if(!sizeof(enemies)) return 1;
    enemies = filter(enemies, (: environment($1) :));
    enemies = filter(enemies, (: environment($1) == environment() :));
    if(!sizeof(enemies)) return 1;
    if(sizeof(enemies) > 5) enemies = scramble_array(enemies)[0..4];
    foreach(object enemy in enemies){
        int which, level;
        string what;
        mixed *skills = enemy->GetSkills();
        mixed *stats = enemy->GetStats();
        if(quarry && enemy == quarry && random(100) > draining) avoid = 100;
        switch(which = random(8)+avoid){
            case 0 : 
                what = skills[random(sizeof(skills)-1)];
                level = enemy->GetSkillLevel(what);
                level = level/2;
                enemy->SetSkill(what,level);
                break;
            case 1 : 
                what = stats[random(sizeof(stats)-1)];
                level = enemy->GetStatLevel(what);
                level = level/2;
                enemy->SetStat(what,level);
                break;
            case 2 : 
                what = "level";
                level = enemy->GetLevel();
                level = level/2;
                enemy->ChangeLevel(level);
                break;
            case 3 :
                what = "health";
                level = enemy->GetHealthPoints();
                level = level/2;
                enemy->AddHP(-level);
                break;
            case 4 :
                what = "stamina";
                level = enemy->GetStaminaPoints();
                level = level/2;
                enemy->AddStaminaPoints(-level);
                break;
            case 5 :
                what = "stamina";
                level = enemy->GetMagicPoints();
                level = level/2;
                enemy->AddMagicPoints(-level);
                break;
            case 6 :
                what = "experience";
                level = enemy->GetExperiencePoints();
                level = level/2;
                enemy->AddExperiencePoints(-level);
                break;
            case 7 :
                what = "quest";
                level = enemy->GetQuestPoints();
                level = level/2;
                enemy->AddQuestPoints(-level);
                break;
            default: break;
        }
        if(which < 100){
            tell_object(enemy,"The wraith drains your vital essence!");
            tell_room(environment(this_object()), "The wraith drains "+
                    enemy->GetName()+" of precious bodily essence!", ({ enemy }));
        }
    }
    return 0;
}

static void create() {
    object tp = this_player();
    if(!tp || !archp(tp)) self_destruct = 1;
    sentient::create();
    SetKeyName("archwraith");
    SetAdjectives( ({"arch", "shadowy", "undead", "unholy", "malevolent", "spiteful"}) );
    SetId( ({"archwraith", "wraith", "specter", "ghost", "apparition", "manifestation"}) );
    SetShort("an archwraith");
    SetLong("This shadowy manifestation is an undead, unholy apparition, oozing malevolence and spite.");
    SetPosition(POSITION_FLYING);
    SetRace("wraith");
    SetClass("fighter");
    SetLevel(20);
    SetMelee(1);
    SetInvis(1);
    SetGender("neuter");
    SetUndead(1);
    SetUndeadType("wraith");
    SetAttackable(0);
    //SetPacifist(1);
    SetCombatAction(100, (: eventDrain :));
    SetNoClean(1);
}

varargs int eventReceiveDamage(mixed args...){
    return 0;
}

void init(){
    ::init();
    add_action("add_target","wraithseek");
    add_action("track_target","wraithtrack");
}

void heart_beat(){
    int here;
    mixed *dest;
    ::heart_beat();
    if(!quarry && qname) quarry = find_player(qname);
    if(quarry && environment(quarry) && environment(quarry) == environment()){
        eventDrain(quarry);
    }
    if(sizeof(enemies) || quarry){
        foreach(object enemy in enemies + ({ quarry })){
            if(!enemy) continue;
            if(environment(enemy) && environment(enemy) == environment()){
                here = 1;
            }
        }
        if(!here){
            dest = filter(enemies, (: environment($1) :));
            if(quarry) dest = (environment(quarry) ? ({ quarry }) : dest );
            if(sizeof(dest)){
                eventMove(environment(dest[random(sizeof(dest)-1)]));
            }
            else {
                this_object()->eventMove(load_object(ROOM_DEATH));
            }
        }
    }
}

int add_target(string str){
    object target = find_player(str);
    if(!this_player() || (this_player() && !adminp(this_player()))){
        return 0;
    }
    if(target){
        write("seeking "+str);
        if(environment(target)){
            this_object()->eventMove(environment(target));
            this_object()->AddEnemy(target);
        }
    }
    return 1;
}

int track_target(string str){
    object target = find_player(str);
    if(!this_player() || (this_player() && !adminp(this_player()))){
        return 0;
    }
    qname = str;
    if(target){
        write("tracking "+str);
        if(environment(target)){
            quarry = target;
            this_object()->eventMove(environment(target));
        }
    }
    return 1;
}

int eventDestruct(){
    if(this_player() && adminp(this_player())){
        return ::eventDestruct();
    }
    return 0;
}

int SetDraining(int i){
    if(!this_player() || (this_player() && !adminp(this_player()))){
        return 0;
    }
    draining = i;
    return draining;
}
