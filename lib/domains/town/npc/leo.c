
#include <lib.h>

inherit LIB_SENTIENT;
int give_it(string str);

int WieldStaff(){
    if(!present("staff",this_object())){
	new("/domains/town/weap/leostaff")->eventMove(this_object());
	tell_room(environment(),"Leo claps his hands and a large "+
	  "wooden staff materializes.");
	this_object()->eventForce("wield staff");
	this_object()->eventForce("say you poor fool!");
    }
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("leo");
    SetId(({"archwizard","wizard"}));
    SetAdjectives(({"portly","the portly","wizard","archwizard"}));
    SetShort("Leo the portly archwizard");
    SetLong("Leo is a large, friendly-looking wizard with a big "+
      "beard and a huge belly. He seems pretty busy and "+
      "somewhat preoccupied.");
    SetLevel(300);
    SetUnique(1);
    SetRace("human");
    SetGender("male");
    SetClass("mage");
    SetInventory(([
	"/domains/town/armor/robe":"wear robe",
	"/domains/town/armor/wizard_hat":"wear hat",

      ]));
    SetAction(25, ({
	"Leo scratches his beard thoughtfully.",
	"Leo seems to be trying to remember something.", "Leo ponders.",
	"Leo thinks.", "Leo thinks carefully.",
	"Leo says, \"Where in blazes did I put that Orcslayer?\"",
	"Leo says, \"I just hope those pesky orcs didn't find it.\"",
	"Leo says, \"I just don't have time to fight those orcs.\""
      }));
    SetCombatAction(100, (: WieldStaff :));
}

int CompleteQuest(object ob){
    string *quests;
    quests = ob->GetQuests();
    if(ob->GetQuest("Orc Slayer Quest") == "nohit"){
	ob->AddQuest("the Orc Slayer","Orc Slayer Quest");
	eventForce("say You have solved the Orc Slayer Quest. Congratulations!");
	eventForce("say I hereby award you 10 quest points, and 2000 experience points!");
	ob->AddQuestPoints(10);
	ob->AddExperiencePoints(2000);
    }
    return 1;
}

int eventReceiveObject() {
    object ob, player;
    ob = previous_object();
    player = this_player();

    if( !ob || !::eventReceiveObject() ) return 0;
    if( ob->GetKeyName() == "orc slayer" ) CompleteQuest(player);

    AddCarriedMass((int)ob->GetMass());
    return 1;
}
