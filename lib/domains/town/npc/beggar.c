#include <lib.h>
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
    SetMaxHealthPoints(40);
    SetOwner("The LPC Preservation Foundation");
    SetPosition(POSITION_SITTING);
}

string GetLong(){
    return sentient::GetLong();
}

int GiveMap(object ob){
    object map;
    if(present(ob->GetKeyName(),environment(this_object()))
      && !this_object()->GetInCombat()){
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
      && !this_object()->GetInCombat())
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
}
