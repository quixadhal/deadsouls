/*    /domains/Ylsrim/npc/fighter.c
 *    From the Dead Souls Mud Library
 *    An example class leader
 *    Created by Lassondra@Dead Souls
 */

#include <lib.h>
inherit LIB_TRAINER;

static string save_file = save_file("/domains/Ylsrim/save/roshd");
static string quest_object = "/domains/town/armor/orc_helmet";
string *ok_join = ({});

int JoinGuild(object ob, string verb, string what);
int TeachFeat(object who, string verb, string feat);
int AllowPass(object who, object what){
    if(who->GetClass() == "fighter") return 1;
    return ::AllowPass(who, what);
}

int CheckVisitor(object who){
    eventForce("say Only fighters are allowed there!");
    return 1;
}

static void create() {
    trainer::create();
    RestoreObject(save_file);
    if(!ok_join) ok_join = ({});
    SetKeyName("roshd burlyneck");
    SetId("roshd", "roshd burlyneck");
    SetShort("Roshd Burlyneck, Master Warrior");
    SetLong("Roshd Burlyneck is the master warrior of "
            "the fighters. Immense in size, Roshd stands well "
            "over seven feet tall. Long brown hair streams "
            "out behind him, and his biceps are so large "
            "it would take several hand spans to fit around "
            "them. Roshd's armor is worn and covered in dirt "
            "as if he has just returned from combat. "
            "Roshd initiates new members into the ways of "
            "combat. If you choose to become "
            "a fighter, \"ask roshd to join\".\n\nDespite his "
            "devotion to his guild, Roshd will train outsiders in "
            "combat skills, if they are capable. For example: \"ask "
            "roshd to train blunt defense\"");
    SetRace("orc");
    SetClass("fighter");
    SetLevel(45);
    SetGender("male");
    SetMorality(-400);
    SetProperty("no bump", 1);
    SetGuard("north", (: CheckVisitor :));
    //SetLanguage("Common", 100);
    //SetDefaultLanguage("Common");
    SetNoSpells(1);
    AddTrainingSkills(({"multi-weapon","multi-hand","blunt attack","blunt defense","blade attack","blade defense","knife attack","knife defense"}));
    // stick him on the fighter line
    if( clonep() ) {
        AddChannel("fighter");
    }
    SetCommandResponses( ([
                "train" : (: eventTrain :),
                "teach" : (: eventTrain :),
                "default" : (: eventHelp :),
                "help" : (: eventHelp :),
                "join" : (: JoinGuild :),
                ]) );
    AddRequestResponses( ([
                "help" : (: eventHelp :),
                ]) );
}

int JoinGuild(object ob, string verb, string what){
    string keyname = ob->GetKeyName();
    string name = ob->GetName();
    if(ob->GetClass() == "fighter") {
        eventForce("say You're already in, genius.");
        return 1;
    }
    eventForce("say Hmm...want to join our guild, do you?");

    if(member_array(keyname, ok_join) == -1){
        eventForce("say First you must prove yourself worthy. Bring me "
                "the skull helmet stolen by the wannabe orc shaman and I will "
                "deem you fit to join our family. Until then, you may not join "
                "the fighters.");
        return 1;
    }
    if(ob->GetClass() == "explorer" || !ob->GetClass()){
        eventForce("say very well, "+ob->GetName()+".");
        ob->ChangeClass("fighter");
        eventForce("say Welcome to the fighters' guild! You now have "
                "access to training in special skills designed to make you into "
                "a fine and respectable meat shield!");
        eventForce("laugh");
        eventForce("say For weapon based training, come to me. For melee "
                "skills, also known as unarmed combat, see our martial artist "
                "trainer in the room north of here.");
        return 1;
    }
    eventForce("say I'm sorry, it looks to me like you have already chosen your
            specialty. As a member of the Guildmasters' Guild, I am forbidden from removing
            you from the "+capitalize(ob->GetClass())+"'s Guild.");
    return 1;
}

int CompleteQuest2(object ob){
    string *quests;
    object hat = present_file("/domains/town/armor/orc_helmet", this_object());
    quests = ob->GetQuests();
    if(!ob->GetQuest("Orc Master Quest")){
        ob->AddQuest("the Orc Master","Orc Master Quest");
        eventForce("say You have solved the Orc Master Quest. Congratulations!");
        eventForce("say I hereby award you 7 quest points, and 2000 experience points!");
        ob->AddQuestPoints(7);
        ob->AddExperiencePoints(2000);
        if(hat) hat->eventDestruct();
        reload("/domains/town/room/valley",0,1);
        reload("/domains/town/room/orc_fortress",0,1);
        reload("/domains/town/room/orc_temple",0,1);
    }
    return 1;
}

int eventReceiveObject(object foo){
    int ret;
    object ob, player;
    ob = previous_object();
    player = this_player();

    if( !ob || !(ret = ::eventReceiveObject(foo)) ) return 0;
    if(base_name(ob) == quest_object){
        call_out("EnableJoin", 0, player, ob);
    }
    if(base_name(ob) == "/domains/cave/obj/letter" && interactive(player)){
        call_out("CompleteQuest", 0, player);
    }
    return ret;
}

mixed EnableJoin(object player, object thing){
    string uname = thing->GetUniqueId();
    string name, keyname;
    if(!player) return 0;
    keyname = player->GetKeyName();
    if(member_array(keyname, ok_join) == -1){
        name = lower_case(player->GetName());
        ok_join += ({ keyname });
        SaveObject(save_file,1);
        if(name != keyname) keyname = capitalize(name);
        if(sizeof(present_file(quest_object, this_object(), 1)) > 1){
            eventForce("say Nice. I'm all set for now, though, so "+
                    "you can have this back.");
            eventForce("give "+uname+" to "+player->GetKeyName());
        } 
        else {
            eventForce("say Thanks, "+keyname+". It's just what I wanted.");
            eventForce("wear "+uname);
        }
        if(player->GetClass() != "fighter"){
            eventForce("say If you want, you may now ask to join the "+
                    "fighter's guild: ask roshd to join");
        }
        call_out("CompleteQuest2", 0, player);
    }
    else {
        eventForce("say I think we went through this already.");
        eventForce("give "+uname+" to "+player->GetKeyName());
    }
    return 1;
}

int CompleteQuest(object ob){
    string *quests;
    object letter = present("kletter", this_object());
    quests = ob->GetQuests();
    if(!ob->GetQuest("A Soldier's Lament")){
        ob->AddQuest("the Postman","A Soldier's Lament");
        eventForce("read letter");
        eventForce("cry");
        eventForce("say However you got this, I thank you, and Kasatka thanks you.")
            ;
        eventForce("say I hereby award you 5 quest points, and 800 experience 
                points.");
        ob->AddQuestPoints(5);
        ob->AddExperiencePoints(800);
        if(letter) letter->eventDestruct();
        reload("/domains/cave/room/kurogane",0,1);
    }
    return 1;
}

void init(){
    ::init();
}


