#include <lib.h>
#include <daemons.h>
#include <position.h>

inherit LIB_SENTIENT;
inherit LIB_DONATE;

static void create() {
    sentient::create();
    SetKeyName("beggar");
    SetId(({"beggar"}));
    SetAdjectives(({"dirty"}));
    SetShort("a dirty beggar");
    SetLong("This beggar has something strangely noble about his aspect. "
            "He certainly doesn't look like he has always been a beggar.");
    SetCanBite(0);
    SetWimpy(90);
    SetPacifist(1);
    SetInventory(([
                "/domains/town/obj/map" : 1,
                ]));
    SetLevel(1);
    SetAutoStand(0);
    SetRace("human");
    SetGender("male");
    SetConsultResponses( ([
                "map" : "It's so you have an idea how to get around.",
                ({ "levels", "leveling", "level" }) : "I don't know such stuff. "+
                "Ask Dirk in the Adventurers' Guild."
                ]) );
    SetPosition(POSITION_SITTING);
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}

//Fix courtesy of  Jonez
string GetLong(string str){
    return sentient::GetLong(str);
}

int GiveMap(object ob){
    object map;
    if(ob && present(ob->GetKeyName(),environment(this_object()))
            && !this_object()->GetInCombat() &&
            member_array(ob->GetRace(),RACES_D->GetRaces(1)) != -1 &&
            !creatorp(ob) &&
            !stringp(ob->CanManipulate()) ){
        if(ob->GetGender() == "male"){
            eventForce("say take this, brother. May it serve you well.");
        }
        else if(ob->GetGender() == "female"){
            eventForce("say take this, sister. May it serve you well.");
        }
        else eventForce("say here, you might need this");
        eventForce("give my first map to "+ob->GetKeyName());
    }
    if(map = present("map",this_object())) {
        map->eventDestruct(); 
        eventForce("shrug");
    }
    return 1;
}

int SayHi(object ob){
    if(this_object() && ob &&
            present(ob->GetKeyName(),environment(this_object()))
            && !this_object()->GetInCombat() &&
            member_array(ob->GetRace(),RACES_D->GetRaces(1)) != -1)
        eventForce("say Hi, "+ob->GetName());
    return 1;
}

void init(){
    ::init();

    call_out((: SayHi, this_player() :), 1);

    if(!present("town map",this_player())){
        new("/domains/town/obj/map")->eventMove(this_object());
        call_out((: GiveMap, this_player() :), 4);
    }
    //if(!present("cold virus",this_object())){
    //    new("/domains/town/obj/cold")->eventInfect(this_object());
    //}
}
