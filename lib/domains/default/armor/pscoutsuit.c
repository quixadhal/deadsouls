#include <lib.h>
#include <armor_types.h>
#include <size_types.h>
#include <damage_types.h>
inherit LIB_BANE;
inherit LIB_WORN_STORAGE;
object scoutshadow;
int active = 1;
int charge = 0;
int maxcharge = 3000;
int disguised = 0;
string owner;

varargs mixed GetSuitHelp(mixed who, string where);

string LongDesc(){
    string ret = "A highly advanced armored suit of Poleepkwa design, "+ 
        "used by elements of the Host whose role requires them to have "+
        "some protection from environmental hazards.";
    if(!active) return ret;
    if(charge < percent_of(10, maxcharge)) ret += " A %^RED%^red%^RESET%^ light is illuminated on it.";
    else if(charge < percent_of(50, maxcharge)) ret += " A %^YELLOW%^yellow%^RESET%^ light is illuminated on it.";
    else if(charge < percent_of(80, maxcharge)) ret += " A %^GREEN%^green%^RESET%^ light is illuminated on it.";
    else ret += " A %^BLUE%^blue%^RESET%^ light is illuminated on it.";
    return ret;
}

static void create(){
    ::create();
    SetKeyName("scout suit");
    SetId(({"suit", "armor"}));
    SetAdjectives(({"poleepkwa","scout","suit of","powered","formidable", "formidable looking"}));
    SetShort("a suit of poleepkwa scout armor");
    SetLong((:LongDesc:));
    SetMass(500);
    SetMatching(0);
    SetBaseCost("silver",5000);
    SetArmorType(A_EXO);
    SetRestrictLimbs( ({ 
                "torso", "head", "neck",
                "right arm", "left arm",
                "right leg", "left leg",
                "right hand", "left hand",
                "right foot", "left foot",
                }) );
    AddSave( ({ "charge", "disguised", "owner", "maxcharge" }) );
    SetMaxCarry(500);
    SetSize(S_SOMEWHAT_LARGE);
    SetProtection(BLUNT,20);
    SetProtection(BLADE,20);
    SetProtection(KNIFE,20);
    SetDamagePoints(100);
    SetWear((: GetSuitHelp :));
    AddItem( ({"light","status light"}), "A status light.");
    set_heart_beat(5);
}

void init(){
    ::init();
    add_action("GetSuitHelp", "help");
}

varargs mixed GetSuitHelp(mixed who, string where){
    string ret, ret2, str;
    object env;
    if(!who){
        return 0;
    }
    if(stringp(who)){
        str = who;
        who = this_player();
    }
    else str = GetKeyName();
    env = environment(who);
    if(query_verb() == "wear" || (str && answers_to(str, this_object()))){
        if(environment() == who){
            ret = "The suit's Heads Up Display crackles to life and reads:\n ";
            ret += "%^B_BLACK%^CYAN%^";
            ret2 = "From the Host you get identity. From the many "+
                "we are Host. You are protected to serve. Serve the Host "+
                "with this suit. This suit protects you. Use it to serve. "+
                "\nWhen suit light is yellow or red, hide."
                "\nWhen suit light is green or blue, serve."
                "\nYour black juice makes the suit go."
                "\nThe suit pulls from you the black juice."
                "\nWhen it is full of black juice, the light is blue."
                "\nWhen the light is on you can breathe."
                "\nWhen the light is on you can take big hurt."
                "\nWhen the light is on you can see good."
                "\nWhen the light is on you don't get sick.";
            "\nYou can not trade suits with others.";
            if(query_verb() == "wear" && !GetWorn()){
                who->eventPrint("You wear "+GetShort()+".");
                if(env) tell_room(env, who->GetName()+" wears "+
                        GetShort()+".", ({who}));
            }
            ret2 = translate(ret2, who->GetLanguageLevel("Poleepkwa"));
            who->eventPrint(ret+ret2+"%^RESET%^");
            return 1;
        }
    }
    return 0;
}

mixed eventEquip(object who, string array limbs){
    mixed success = ::eventEquip(who, limbs);
    scoutshadow = new("/shadows/pscout");
    if(scoutshadow) scoutshadow->SetDisguised(disguised);
    if(success){
        if(scoutshadow) scoutshadow->eventShadow(who);
    }
    else if(scoutshadow) destruct(scoutshadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = ::eventUnequip(who);
    if(success){
        if(scoutshadow) who->unscoutshadow();
    }
    return success;
}

int GetActive(){
    if(!charge) return 0;
    return active;
}

int SetActive(int i){
    if(i) active = 1;
    else active = 0;
    return active;
}

int eventDecrementCharge(int i){
    int perc;
    object env = environment();
    if(!env) return 0;

    if(!GetWorn()) return 0;
    if(previous_object() != scoutshadow) return 0;
    if(charge < 1) charge = 0;
    else { 
        if(!i) charge--;
        else charge -= i;
        if(charge < 0) charge = 0;
    }

    perc = to_int(percent(charge, maxcharge));
    if(perc < 2){
        if(living(env) && creatorp(env)){
            env->eventPrint("Your creator powers magically recharge the "+
                    remove_article(GetShort())+".");
            charge = maxcharge;
            return charge;
        }
        tell_object(env,"The "+remove_article(GetShort())+" beeps loudly!");
        return charge;
    }

    if(perc < 2){
        if(living(env) && creatorp(env)){
            env->eventPrint("Your creator powers magically recharge the "+
                    remove_article(GetShort())+".");
            charge = maxcharge;
            return charge;
        }
        tell_object(env,"The "+remove_article(GetShort())+" beeps softly.");
        return charge;
    }

    return charge;
}

int GetRemainingCharge(){
    if(!charge || !GetWorn()){
        //if(scoutshadow) scoutshadow->eventUnshadow();
        return 0;
    }
    return charge;
}

int GetMaxCharge(){
    return maxcharge;
}

int SetCharge(int i){
    charge = i;
    return charge;
}

int SetMaxCharge(int i){
    maxcharge = i;
    return maxcharge;
}

string array GetBane(){
    if(GetActive()) return ({ "all" });
    else return ({});
}

string array QueryBane(){
    if(GetActive()) return ({ "all" });
    else return ({});
}

void heart_beat(){
    int notify;
    string str, envname;     
    object env = environment();
    object room = room_environment();
    if(!env || !room) return;
    envname = env->GetKeyName();
    if(owner && GetWorn() && envname != owner){
        tell_object(env, "The suit makes a brief cranking, buzzing sound.");
        if(active) active = 0;
        return;
    }
    if(active && !GetWorn()){
        tell_room(env, "The "+remove_article(GetShort())+
                " whines.");
        active = 0;
    }
    else if(GetWorn() && charge < maxcharge){
        int fuel;
        fuel = env->GetMagicPoints();
        if(fuel < 5){
            if(!charge && active){
                active = 0;
                if(environment(env)){
                    tell_room(environment(env), env->GetName()+"'s "+
                            "powered suit whines.", ({env}));
                    env->eventPrint("Your powered suit whines.");
                }
            }
        }
        else {
            env->AddMagicPoints(-5);
            charge += 5;
            //env->eventPrint("adding 5. charge: "+charge+". "+
            //  (active ? "active" : "inactive"));
            if(!owner) owner = env->GetKeyName();
        }
    }
    if(GetWorn() && !active && charge > 9){
        active = 1;
        if(environment(env)){
            tell_room(environment(env), env->GetName()+"'s "+
                    "powered suit chirps.", ({env}));
            env->eventPrint("Your powered suit chirps.");
        }
    }
}

//Every 100 points of damage received by the suit, it lowers
//the max charging capacity by 100.
void eventDeteriorate(int type){
    object env = environment();
    if(maxcharge > 100){
        if(active){
            if(env) env->eventPrint("Your powered suit emits a harsh buzz and "+
                    "the indicator light dims briefly.");
        }
        maxcharge -= 100;
    }
    ::eventDeteriorate(type);
}

int eventReceiveDamage(mixed agent, int type, int amt, int i, mixed array l){
    if(!active || charge < 20){
        return ::eventReceiveDamage(agent, type, amt, i, l);
    }
    return 0;
}

