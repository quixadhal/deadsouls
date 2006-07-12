#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;
inherit MY_AREA + "/etc/call_cops";

object gate;
string gate_file, undesirables, scumline;
string *scum;
string *enemy_races;

int runnerup;

int eventRequest(mixed arg1, mixed arg2, mixed arg3);

static void create(){
    sentient::create();
    SetKeyName("guard");
    SetId( ({"human","gate guard","warrior","soldier"}) );
    SetShort("a guard");
    SetLong("This is a large professional soldier. He wears "+
      "the proud crimson insignia of the Fortress Guards, a legendary "+
      "force of warriors.");
    SetLevel(5);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    AddCommandResponse("open", (: eventRequest :));
    AddCommandResponse("for", (: eventRequest :));
    AddCommandResponse("access", (: eventRequest :));
    SetTalkResponses( ([ "open sesame" : "yeah right",
	"guard to" : "Please be specific. Ask which guard?", 
	"guard for" : "Please be specific. Ask which guard?" ]) );
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

    undesirables = MY_AREA + "/etc/non_gratae.cfg";
    gate = present("great gate",environment(this_object()));
    if(gate) gate_file = gate->GetDoor();

    //eventForce("say howdy, "+previous_object()->GetKeyName());

    scum = explode( unguarded( (: read_file(undesirables) :) ),":");
    if(member_array(previous_object()->GetKeyName(), scum) != -1 ||
      member_array(previous_object()->GetRace(),enemy_races) != -1){
	this_object()->eventForce("close gate");
	gate_file->SetLocked(0);
	this_object()->AddEnemy(previous_object());
	call_out( (: eventRecognize, previous_object()->GetRace(),previous_object()->GetGender() :), 1);
    }
}

int OpenGate(){
    gate_file->SetLocked(0);
    eventForce("open gate");
    return 1;
}

int eventRequest(mixed arg1, mixed arg2, mixed arg3){
    string *stuff;
    string *stuff2;

    stuff = ({"gate","the gate","the great gate"});
    stuff += ({"great gate","fortress gate"});

    stuff2 = ({"access","access inside","access within"});
    stuff2 += ({"access into the fort"});

    //if(arg1) eventForce("say arg1 is a "+typeof(arg1));
    //if(arg2) eventForce("say arg2 is a "+typeof(arg2));
    //if(arg3) eventForce("say arg2 is a "+typeof(arg3));
    //if(arg1) eventForce("say arg1 is "+arg1->GetName());
    //if(arg2) eventForce("say arg2 is "+arg2);
    //if(arg3 && stringp(arg3)) eventForce("say arg3 is "+arg3);
    if(GetInCombat()){
	eventForce("say I think you can see I'm busy.");
	return 1;
    }
    if(arg3 && member_array(arg3,stuff) != -1) OpenGate();
    else if(arg2 &&  member_array(arg2,stuff2) != -1) OpenGate();
    else {
	eventForce("say I don't understand. What do you want to ask me for?");
    }

    return 1;
}

int eventExtraAction(){
    int closed, locked;

    call_cops::eventExtraAction();

    if(gate && this_object()->GetInCombat()){

	closed = gate_file->GetClosed();
	locked = gate_file->GetLocked();
	if(!closed) eventForce("close gate");
	if( closed && !locked) 	gate_file->SetLocked(1);
    }

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

