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
	//"/domains/town/obj/cold" : 1,
      ]));
    SetLevel(1);
    SetAutoStand(0);
    SetRace("human");
    SetGender("male");
    SetOwner("The LPC Preservation Foundation");
    SetPosition(POSITION_SITTING);
}

//Fix courtesy of  Jonez
string GetLong(string str){
    return sentient::GetLong(str);
}

int GiveMap(object ob){
    object map;
    if(present(ob->GetKeyName(),environment(this_object()))
      && !this_object()->GetInCombat() &&
      member_array(ob->GetRace(),RACES_D->GetRaces(1)) != -1 &&
      !creatorp(ob) &&
      !stringp(ob->CanManipulate()) ){
	eventForce("say here, you might need this");
	eventForce("give my first map to "+ob->GetKeyName());
    }
    if(map = present("map",this_object())) {
	map->eventDestruct(); 
	eventForce("shrug");
    }
    return 1;
}

int SayHi(object ob){
    if(present(ob->GetKeyName(),environment(this_object()))
      && !this_object()->GetInCombat() &&
      member_array(ob->GetRace(),RACES_D->GetRaces(1)) != -1)
	eventForce("say Hi, "+this_player()->GetName());
    return 1;
}

void init(){
    ::init();

    call_out((: SayHi, this_player() :), 1);

    if(!present("town map",this_player())){
	new("/domains/town/obj/map")->eventMove(this_object());
	call_out((: GiveMap, this_player() :), 4);
    }
    if(!present("cold virus",this_object())){
	new("/domains/town/obj/cold")->eventInfect(this_object());
    }
}
