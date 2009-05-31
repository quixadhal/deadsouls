#include <lib.h>
#include ROOMS_H
#include <damage_types.h>
#include <vendor_types.h>
#include <daemons.h>

inherit LIB_DUMMY;

string WoundLong(){
    int num;
    string noun, poss, verb, ret;
    string being, plural;
    object env = environment();

    if(!env){
        return "Error.";
    }

    num = env->GetLead(); 

    if(env == this_player()){
        noun = "You";
        verb = "have";
        poss = "your";
    }
    else {
        noun = env->GetName();
        verb = "has";
        poss = possessive(env);
    }
    num = env->GetLead();
    if(!num){
        verb = (env == this_player() ? "are" : "is");
        ret =  noun + " " + verb + " recovering from gunfire wounds.";   
        return ret;
    }
    if(num > 1){
        being = "are";
        plural = "s";
    }
    else {
        being = "is";
        plural = "";
    }

    ret = noun + " " + verb + " been wounded by gunfire. There "+ 
        being + " " + cardinal(this_player()->GetLead())+" bullet hole"+
        plural + " in " + poss + " body.";

    return ret;
}

void create(){
    ::create();
    SetKeyName("wound");
    SetId(({"wound","firearms_wound","wounds"}));
    SetShort("a gunshot wound");
    SetLong( (: WoundLong :) );
    set_heart_beat(20);
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob) || ob->GetUndead()) return 0;
    return ob->GetName() + " is suffering from gunshot wounds.";
}

void heart_beat(){
    object ob = environment();
    int num;
    if(!ob || !living(ob)){
        this_object()->eventMove(ROOM_FURNACE);
        return;
    }
    num = ob->GetLead();
    if(!num){
        int tmp = ob->GetLead("gunshot_wounds");
        ob->AddLead("gunshot_wounds", -tmp);
        tmp = ob->GetLead("rifleshot_wounds");
        ob->AddLead("rifleshot_wounds", -tmp);
        this_object()->eventMove(ROOM_FURNACE);
        return;
    } 
    if(!RACES_D->GetNonMeatRace(ob->GetRace()) && interactive(ob)
            && !(ob->GetUndead())){
        tell_object(ob,"You bleed heavily from your gunshot wounds.");
        tell_room(environment(ob), ob->GetName()+
                " bleeds heavily from "+possessive(ob)+" gunshot "+
                "wounds.",environment());
    }
}

mixed CanGet(object ob) { return "#Your hands slip on the gunshot wounds.";}
mixed CanDrop(object ob) { return "#Your hands slip on the gunshot wounds.";}
