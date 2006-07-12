#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;
inherit "/domains/fort/etc/call_cops";

object gate;
string gate_file, undesirables, scumline;
string *scum;
string *enemy_races;

int runnerup;

int eventRequest(mixed arg1, mixed arg2, mixed arg3);

static void create(){
    sentient::create();
    SetKeyName("Provost Guard");
    SetId( ({"guard", "provost", "soldier"}) );
    SetShort("a provost guard");
    SetLong("This is a professional soldier. He wears "+
      "the dark crimson insignia of the Fortress Provost "+
      "Guards, the feared security apparatus of the "+
      "Fortress on the Frontiers.");
    SetPermitLoad(1);
    SetWanderSpeed(5);
    SetLevel(9);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
	MY_ARMOR "/chainmail.c" : "wear chainmail",
	MY_ARMOR "/leather_boot_r.c" : "wear boot",
	MY_ARMOR "/leather_boot_l.c" : "wear boot",
	MY_ARMOR "/steel_glove_l.c" : "wear glove",
	MY_ARMOR "/steel_glove_r.c" : "wear glove",
	MY_ARMOR "/steel_helmet.c" : "wear helmet",
	MY_ARMOR "/crimson_cloak.c" : "wear cloak",
	MY_ARMOR "/platemail_pants.c" : "wear pants",
	MY_ARMOR "/shield.c" : "wear shield on left arm",
	MY_WEAP "/std_sword.c" : "wield sword in right hand",
      ]));
    runnerup = 0;
    enemy_races = ({ "orc", "troll", "balrog","demon","gargoyle","gnoll","goblin","kobold","half-orc","lizard-man","ogre","klingon","satyr","wraith" });
}

int eventForgetElection(){
    runnerup = 0;
}

int eventLoseElection(){
    runnerup = 1;
}

int eventUnelect(){
    foreach(object dude in livings(environment(this_object()))){
	if(member_array("guard",dude->GetId()) != -1){
	    dude->eventForgetElection();
	}
    }
    return 1;
}

int eventElect(){
    foreach(object dude in livings(environment(this_object()))){
	if(member_array("guard",dude->GetId()) != -1){
	    dude->eventLoseElection();
	}
    }
    return 1;
}

int eventRecognize(string race, string gender){
    string insult;
    if(gender == "male") insult = "bastard";
    else insult = "bitch";
    if(runnerup) return 1;
    eventElect();
    eventForce("say you "+insult+"! Back for more, eh?");
    if(member_array(race,enemy_races) != -1) {
	eventForce("say damned "+pluralize(race)+" don't know when to quit!");
    }
    call_out( (: eventUnelect :), 5);
    return 1;
}

void init(){
    ::init();

    undesirables = "/domains/fort/etc/non_gratae.cfg";
    gate = present("great gate",environment(this_object()));
    if(gate) gate_file = gate->GetDoor();

    //eventForce("say howdy, "+previous_object()->GetKeyName());

if(file_exists(undesirables) && sizeof(unguarded( (: read_file(undesirables) :) ))){
    scum = explode( unguarded( (: read_file(undesirables) :) ),":");
    if(member_array(previous_object()->GetKeyName(), scum) != -1 ||
      member_array(previous_object()->GetRace(),enemy_races) != -1){
	this_object()->AddEnemy(previous_object());
	call_out( (: eventRecognize, previous_object()->GetRace(),previous_object()->GetGender() :), 1);
    }
}
}

int eventExtraAction(){
    int closed, locked;

    call_cops::eventExtraAction();

    if(this_object()->GetInCombat()){
	foreach(object aggressor in this_object()->GetEnemies()){
	    if(member_array(aggressor->GetKeyName(),scum) == -1){
		scum += ({ aggressor->GetKeyName() });
		scumline = implode(scum,":");
		unguarded( (: write_file(undesirables,scumline,1) :) );
	    }
	}
    }

    return 1;
}
