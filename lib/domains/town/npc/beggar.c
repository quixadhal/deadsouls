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
    SetAction(5, ({
                "!say 'Bree-yark' is hobgoblin for 'we surrender'.",
                "!say Donating to the church sometimes gives you a "+
                "bonus, but don't take from the charity bin if "+
                "you're above level 5! It'll deduct XP!",
                "!say Brandy's a fine girl, and has the purest spice "+
                "sticks in town in her secret menu.",
                "!say I heard the secret cave is straight north in the "+
                "forest.", 
                "!say one time I saw something glowing in the middle of "+
                "the forest.",
                "!say Don't mess with balrogs. Seriously.",
                "!say You ever see an orc or a dwarf try to swim? It "+
                "ain't pretty.",
                "!say Y'ever wonder if them orcs have good reason to be "+
                "so aggro all the time?",
                "!say You don't need to be in the mage's guild to learn some "+
                "spells from Herkimer, but it helps.",
                "!say If you want to be a fighter, I was told you need to "+
                "find something called a 'stargate' and then 'dial tower'."+
                " How's anybody supposed to figure *that* one out?",
                "!say if you can get someone to resurrect your corpse, "+
                    "you won't have a death penalty. But remember there is a "+
                    "difference between resurrection and raising!",
                "!say you can't control a zombie if you don't speak its "+
                    "language.",
                "!say I heard there's a language teacher at the northern "+
                    "fort that teaches for money rather than training points.",
                "!say Be careful traveling whyle flying or invisible. "+
                    "It really drains your stamina!",
                "!say Advancing levels improves your skills, so join a "+
                    "guild before advancing, if you can.",
                "!say if you don't advance levels automatically when you "+
                    "qualify, go see Dirk and ask him to advance.",
                "!say The Yautja are a proud hunter race from another "+
                    "world with powerful magic. Pray you never meet one.",
                "!say Being drunk helps you heal faster, but it's not a "+
                    "good idea to try to figth drunk!",
                "!say Drinking coffee sobers you up some.",
                "!say A belly full of food and water helps you heal faster.",
                "!say If you don't see items in a room that you are sure "+
                    "should be there, maybe you need a light source."
    }));
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
