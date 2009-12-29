#include <lib.h>

inherit LIB_SENTIENT;

varargs int ReceiveCoat(mixed already, object who){
    if(already > 1){
        eventForce("say uh...no thanks. I'm set.");
        eventForce("drop a coat");
    }
    else {
        string noun;
        switch(who->GetGender()){
            case "male" : noun = "lad";break;
            case "female" : noun = "lass";break;
            default : noun = who->GetRace();break;
        }
        call_out("eventForce",1,"say Excellent! Well done, young "+noun+"!");
        call_out("eventForce",2,"remove badge");
        call_out("eventForce",3,"emote fumbles awkwardly with the coat.");
        call_out("eventForce",4,"drop badge");
        call_out("eventForce",5,"wear coat");
        call_out("eventForce",6,"say Ahh, much better. Thank you!");
    }
    return 1;
}

int EncounterResponse(int i){
    if(!i) i = random(i)+2;
    switch(i){
        case 1 : 
            eventForce("yell DON'T SHOOT! I'M WITH THE SCIENCE TEAM!");break;
        case 2 :
            eventForce("emote scratches his head thoughtfully.");break;
        case 3 :
            eventForce("frown");break;
        case 4 :
            eventForce("hmm");break;
        default :
            eventForce("emote nods thoughtfully.");break;
    }
    return 1;
}

int CheckResponse(){
    eventForce("say I think you can see I'm busy.");
    if(!present("lab coat",this_object())){
        eventForce("say Say, I don't suppose you could find "+
                "my lab coat for me? I just don't have time to go "+
                "looking for it right now!");
    }
    return 1;
}

int EncounterCheck(mixed who){
    object *inv;
    if(!objectp(who) || !living(who)) return 0;
    inv = filter(all_inventory(who), (: inherits(LIB_FIREARM, $1) :));
    inv = filter(inv, (: sizeof($1->GetWorn()) :));
    if(sizeof(inv)){
        call_out("EncounterResponse",0,1);
    }
    else {
        call_out("EncounterResponse",4,random(4)+2);
    }
    return 0;
} 

static void create() {
    ::create();
    SetKeyName("kleiner");
    SetId(({"npc","mob","character","scientist","genius"}));
    SetAdjectives(({"old", "white haired","beloved"}));
    SetShort("Dr. Kleiner");
    SetLong("An old, white-haired scientist, Dr. Isaac Kleiner is a beloved campus personality with his affable, kind nature and genius for exotechnology.");
    SetPacifist(1);
    SetLevel(10);
    SetWimpy(95);
    SetRace("human");
    SetGender("male");
    SetEncounter( (: EncounterCheck :) );
    SetInventory(([
                "/domains/campus/obj/omni" : 1,
                "/domains/campus/armor/badge2" : "wear badge",
                ]));
    SetLanguage("common",100);
    SetDefaultLanguage("common");
    SetConsultResponses( ([ 
                "default" : (: CheckResponse :),
                ({ "gate", "gates", "stargate", "stargates", "portal", "portals" }) :
                "Fascinating, aren't they? A legacy of some highly advanced "+ 
                "civilization. Be very careful if you enter one!",
                ({ "door", "badge", "sliding door" }) : "Oh yes, the security "+
                "door. Sorry, only badged employees like me have access to "+
                "the hazardous materials lab.",
                ({ "hazardous materials lab", "materials lab", "lab" }) :
                "I wouldn't try to get in there if I were you. The "+
                "artifacts there can be too powerful for the unskilled.",
                ]) );
    SetRequestResponses( ([
                "default" : (: CheckResponse :),
                ({"a good grade","good grades"}) : "Those are yours to earn.",
                ]) );
    SetTalkResponses( ([ 
                ({"hi","hello","kleiner" }) : "I'm sorry, young person, but "+
                "I'm terribly, terribly busy analyzing this fascinating "+
                "alien teleportation technology.",
                ]) );
}

void init(){
    ::init();
}

int eventFreakout(mixed agent){
    string name;
    if(stringp(agent)) name = agent;
    if(objectp(agent)) name = agent->GetName();
    else name = "young person";
    if(!this_object()->GetDying()){
        if(present("omni", this_object())){
            eventForce("yell OH MY GOD! THEY'LL KILL US ALL!");
            eventForce("push button on omni");
        }
        else if(objectp(agent)){
            int utterance = random(5)+1;
            switch(utterance){
                case 1 :
                    eventForce("yell "+name+", stop this madness at once!");
                    break;
                case 2 :
                    eventForce("yell What are you doing, "+name+
                            "? I am your *friend*!");
                    break;  
                case 3 :
                    eventForce("yell Fine! Fine! I'll give you an A!");
                    break;
                case 4 :
                    eventForce("yell Oh God, "+name+
                            ", I don't want to die!");
                    break;
                case 5 :
                    eventForce("yell Please don't kill me, "+name+"!");
                    break;
                default :
                    eventForce("kneel");
                    eventForce("say I'm praying to you! Look in your heart!");
                    break;
            }
        }
        else {
            int utterance = random(5)+1;
            switch(utterance){
                case 1 : 
                    eventForce("say I hope someone is taking notes on "+
                            agent+". I wouldn't want my death to be in vain.");
                    break;
                case 2 :
                    eventForce("say If I don't survive "+agent+
                            ", please promise me you'll take care of Lamarr.");
                    break;
                default :
                    eventForce("emote whimpers.");
            }
        }
    }
    return 1;
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal,
        mixed limbs){
    call_out("eventFreakout",0,agent);
    return ::eventReceiveDamage(agent, type, x, internal, limbs);
}

int eventReceiveObject(object ob) {
    int howmany = sizeof(filter(all_inventory(this_object()),
                (: answers_to("lab coat",$1) :) ));
    if( !ob || !::eventReceiveObject(ob) ) return 0;
    if(answers_to("lab coat",ob)){
        call_out("ReceiveCoat", 0, howmany, this_player());
    }
    return 1;
}
