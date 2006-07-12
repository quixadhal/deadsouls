#include <lib.h>

inherit LIB_SENTIENT;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc") return 0;
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"dirty"}));
    SetShort("a dirty orc");
    SetLong("This orc is typical of its breed: nasty, brutish, and short. It appears "
      "to be a juvenile or adolescent, making it somewhat less "
      "dangerous but more hostile. ");
    SetLevel(2);
    SetRace("orc");
    //SetClass("fighter");
    SetGender("male");
    SetMaxHealthPoints(100);
    SetEncounter(0);
    SetInventory(([
	"/domains/town/weap/dagger":"wield dagger",
      ]) );

}

void init(){
    ::init();

    foreach(object dude in get_livings(environment(this_object()))){
	int ret;
	string guy;

	guy = dude->GetName();

	ret = query_invis(dude, this_object());

	if(ret > 0) eventForce("say I DO NOT see "+guy+".");
	else  eventForce("say I see "+guy+".");
    }

}
