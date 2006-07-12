#include <lib.h>
#include <rooms.h>
#include "../customdefs.h"
inherit LIB_SENTIENT;

static void create(){
    sentient::create();
    SetKeyName("sergeant of the guard");
    SetId( ({"human","sergeant"}) );
    SetShort("The sergeant of the guard");
    SetLong("This is an especially tough-looking professional soldier. He wears "+
      "the proud crimson insignia of the Fortress Guards, a legendary "+
      "force of warriors.");
    SetWanderSpeed(20);
    SetLevel(7);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
	MY_ARMOR "/f_platemail.c" : "wear platemail",
	MY_ARMOR "/leather_boot_r.c" : "wear boot",
	MY_ARMOR "/leather_boot_l.c" : "wear boot",
	MY_ARMOR "/steel_glove_l.c" : "wear glove",
	MY_ARMOR "/steel_glove_r.c" : "wear glove",
	MY_ARMOR "/steel_helmet.c" : "wear helmet",
	MY_ARMOR "/crimson_cloak.c" : "wear cloak",
	MY_WEAP "/nco_sword.c" : "wield sword in right hand",
      ]));
}
int SeekHelp(object *defendants){
    object backup;
    string imsg = "A Praetorian Guard strides in.";
    if(!present("praetorian guard",environment(this_object()))){
	backup = new(MY_NPC + "/praetorian_guard");
	backup->eventMove(ROOM_VOID);
	backup->eventMoveLiving(environment(this_object()),"foo?",imsg);
	foreach(object bully in defendants){
	    backup->AddEnemy(bully);
	}
    }
    return 1;
}
int eventExtraAction(){
    object *defendants;
    int aggressor = 0;

    if(GetHealthPoints() > GetMaxHealthPoints() * 0.5) return 1;

    if(!this_object()->GetInCombat()) {
	return 2;
    }

    defendants = this_object()->GetEnemies();
    if(!sizeof(defendants)) {
	return 3;
    }

    foreach(object jerk in defendants){
	if(present(jerk->GetKeyName(),environment(this_object()))){
	    aggressor++;
	}
    }

    if(aggressor > 0 && !present("praetorian guard",environment(this_object()))) {
	eventForce("shout Praetorian Guard! I need backup!");
	call_out((: SeekHelp, defendants :), 5);
    }
    return 1;
}
void init(){
    ::init();
}
