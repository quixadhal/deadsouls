/*
// File     :  /domains/Praxis/sheriff.c
// Comment  :  handles law enforcement
// 94-08-15 :  Descartes of Borg wrote original code
// 94-08-28 :  Manny of Law executed it
// 94-09-02 :  Pallando of Everywhere fixed it
 */
#include <lib.h>
#include <daemons.h>
#include <dirs.h>

inherit LIB_ROOM;

object *__Prisoners;
mapping Executions;

object *clone_guards(int num);

void create() {
    object *obs;
    int x,y, i, tot;

    Executions = ([
            "hanging" : "death_by_hanging",
            "firing squad" : "death_by_firing_squad",
            "torture" : "death_by_torture",
            "the pit" : "death_by_the_pit",
            // "stoning" : "death_by_stoning",
            // "beheading" : "death_by_beheading",
            ]);
    room::create();
    SetNoClean(1);
    SetProperties( ([ "light":2, "indoors":1, "no bump":1, "no kill":1,
                "no steal":1 ]) );
    SetShort("praxis sheriff's office");
    SetLong(
            "In the corner of this small, one room wooden building is "
            "a barred off area indicating that you have entered the Praxis "
            "sheriff's office.  An old desk belonging to the sheriff occupies "
            "the northeast corner of the room.  Exits to court room and yard are "
            "west and south respectively."
           );
    SetItems( ([ "desk":"A rickety wooden desk hardly ever used, as the "
                "sheriff never does seem to be able to sit for any period of time.",
                ({ "building", "office", "room" }): "A wooden building constructed "
                "long ago for a lawless era.  There is a jail cell in one area.",
                "area": "A jail cell.",
                ({ "cell", "jail cell", "jail" }): (: "la_jail" :),
                "exits": "Boc La Road and Centre Path."]) );
    SetSmell("default", "You can smell the residue of sweaty outlaws.");
    SetExits( ([ "south":"/"+__DIR__+"yard",
                "west":"/"+__DIR__+"court_room" ]) );
    x = POLITICS_D->query_personnel("police");
    y = POLITICS_D->query_spending("police");
    //if(!(tot = (y/x)/currency_rate("gold"))) return;
    for(i=0, obs = allocate(x); i<x; i++)
        obs[i] = new("/"+__DIR__+"obj/mon/police");
    obs->SetKeyName("deputy");
    obs->SetId( ({ "law officer", "praxis police", "police", "deputy",
                "officer" }) );
    obs->SetShort("a Praxis deputy");
    obs->SetLong("A law officer charged with preserving the peace in "
            "Praxis.  If you are hunted by this officer, type \"surrender\" "
            "to surrender yourself and go to jail.");
    obs->SetRace("klingon");
    obs->SetRace("human");
    obs->SetGender(random(2) ? "male" : "female");
    obs->SetLevel(tot/1000+1);
    obs->SetSkill("melee", (tot/1000)*7);
    obs->SetSkill("attack", (tot/1000)*7);
    obs->SetSkill("defense", (tot/1000)*7);
    obs->SetSkill("stealth", (tot/1000)*5);
    obs->SetStat("constitution", (tot/1000)*3 + 15);
    obs->SetStat("dexterity", (tot/1000)*5 + 10);
    obs->SetMorality(random(500));
    obs->move(this_object());
    __Prisoners = ({});
}

void init() {
    room::init();
    add_action("cmd_quit", "quit");
}

static int cmd_quit(string str) {
    message("system", "You cannot quit from the sheriff's office.",
            this_player());
    return 1;
}

void rescue_me(object victim, object outlaw) {
    object *obs;
    int x, i;

    if(!present("deputy")) return;
    x = outlaw->query_level() - victim->query_level();
    if(x < 5) x = 1;
    else x = x/5;
    i = sizeof(obs = all_inventory(this_object()));
    while(i-- && x) {
        if(obs[i]->id("deputy")) {
            obs[i]->set_target(outlaw);
            x--;
        }
    }
}

int release_objects(object ob) {
    return (member_array(ob, __Prisoners) == -1);
}

void add_prisoner(object ob) { __Prisoners += ({ ob }); }

void remove_prisoner(object ob) { __Prisoners -= ({ ob }); }

object *query_prisoners() { return __Prisoners; }

string la_jail(string arg) {
    string str;
    int i;

    if(!sizeof(__Prisoners)) return "There are no prisoners right now.";
    str = "The following prisoners are in the cell: ";
    i = sizeof(__Prisoners);
    while(i--) str += __Prisoners[i]->query_cap_name()+"    ";
    return str;
}

object *clone_guards(int num) {
    object *obs;
    int y, x, i, tot;

    y = POLITICS_D->query_spending("police");
    x = POLITICS_D->query_personnel("police");
    for(i=0, obs = allocate(num); i<num; i++)
        obs[i] = new("/"+__DIR__+"obj/mon/guard");
    tot = (y/x)/currency_rate("gold");
    tell_object(find_player("manny"), "tot = "+tot);
    obs->SetKeyName("guard");
    obs->SetId( ({ "guard", "city guard", "officer", "praxis guard" }) );
    obs->SetShort("a Praxis guard");
    obs->SetRace("klingon");
    obs->SetRace("human");
    obs->SetGender(random(2) ? "male" : "female");
    obs->SetLevel(tot/1000+1);
    obs->SetSkill("melee", (tot/1000)*7);
    obs->SetSkill("attack", (tot/1000)*7);
    obs->SetSkill("defense", (tot/1000)*7);
    obs->SetSkill("stealth", (tot/1000)*5);
    obs->SetStat("constitution", (tot/1000)*3 + 15);
    obs->SetStat("dexterity", (tot/1000)*5 + 10);
    obs->SetMorality(random(500));
    return obs;
}

    string death_by_hanging(object who) {
        if(member_array(who, __Prisoners) == -1) 
            return who->query_cap_name()+" is not a prisoner.";
        message("say", "%^RED%^You hear the rythmic pounding of "
                "the city guard approaching the door.  Two large, burly "
                "men in blue step through the door and, without emotion, "
                "bind you hands tightly together with thick rope.",
                who);
        shout("%^BOLD%^%^RED%^Distant bells ring thrice, "
                "signaling the pending execution of "+who->query_cap_name()+
                ".");
        call_out("hanging_part_two", 2, who);
        return "The execution has begun.";
    }

void hanging_part_two(object who) {
    message("say", "\n%^CYAN%^The guard whispers to you: "
            "%^RESET%^Scumbag.", who);
    call_out("hanging_part_three", 3, who);
}

void hanging_part_three(object who) {
    int x, y;
    object *obs;

    message("say", "\n%^RED%^The two guards securely grasp your "
            "arms and begin dragging you to the town square.", who);
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 1);
    new("/"+__DIR__+"obj/misc/handcuffs")->move(who);
    obs = clone_guards(2);
    obs->set_target(who);
    obs->move(DIR_STANDARD_DOMAIN+"/square");
    obs[0] = new("/"+__DIR__+"obj/misc/gallows");
    obs[0]->set_hangman(who);
    obs[0]->move(DIR_STANDARD_DOMAIN+"/square");
    who->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square");
    call_out("hanging_part_four", 4, who);
}

void hanging_part_four(object who) {
    message("say", "\n%^RED%^Meanancing jeers from the gathering "
            "bloodthirsty crowd fill your ears as one of the guards "
            "covers your head with a black hood.  Burned into your "
            "mind is the last image of life you shall ever see, an "
            "image filled with faces of hate and disgust.  You are "
            "overcome with a sincere regret for your crimes, a burning "
            "emotion extinguished by the realization that it is "
            "too late.", who);
    message("say", who->query_cap_name()+" is dragged up the stairs onto "
            "the wooden platform.  Menacing jeers fill the crowd as "
            "one of the more burly guards places a large black hood over "
            "the paniced face of "+who->query_cap_name()+".  Any mercy "
            "for "+who->query_possessive()+" soul is quickly replaced "
            "by hatred and disgust for such a lowly individual.",
            environment(who), who);
    call_out("hanging_part_five", 3, who);
}

void hanging_part_five(object who) {
    message("say", "\n%^RED%^A tight grip upon your shaking "
            "forarm leads you up a short set of stairs onto a wooden "
            "platform.  A heavy rope is placed around your neck, then "
            "tightened.  The rythmic beat of a nearby drum is all that "
            "fills your conscience, as you do not dare fathom what awaits "
            "you on the other side...", who);
    message("say", "\nThe rythmic beat of the drum of death brings a "
            "hush over the surrounding crowd.", environment(who), who);
    call_out("hanging_part_six", 4, who);
}

void hanging_part_six(object who) {
    message("say", "\n%^RED%^The drum's beat falls into a steady "
            "roll...", who);
    message("say", "\nThe drum's beat falls into a steady "
            "roll...", environment(who), who);
    call_out("hanging_part_seven", random(3) + 2, who);
}

void hanging_part_seven(object who) {
    object here;

    here = environment(who);
    message("say", "\n%^RED%^The klunk of the floor falling beneath "
            "you is the final sound to reach your ears.", who);
    message("say", "\nSuddenly without warning, the floor beneath "+
            who->query_cap_name()+"'s feet falls away, leaving "+
            who->query_cap_name()+" suspended only by the rope attached "
            "to "+who->query_possessive()+" neck.  "+
            who->query_cap_name()+" struggles for a moment, then falls "
            "limp.", environment(who), who);
    present("handcuffs", who)->destruct();
    who->die();
    call_out("hanging_part_eight", 3, here);
}

void hanging_part_eight(object where) {
    object stuff;
    message("say", "The guards dispurse, removing the gallows and cleaning up "
            "the area before leaving.", where);
    if(stuff = present("guard", where)) stuff->destruct();
    if(stuff = present("guard", where)) stuff->destruct();
    if(stuff = present("gallows", where)) stuff->destruct();
    where->SetProperty("no bump", 0);
}

string death_by_firing_squad(object who) {
    object *obs;
    if(member_array(who, __Prisoners) == -1) 
        return who->query_cap_name()+" is not a prisoner.";
    message("say", "%^RED%^A large, burly guard enters, accompanied by "
            "a priest in black robes mumbling a prose from a small book "
            "he carries.", who);
    obs=clone_guards(2);
    obs[1]->SetKeyName("priest");
    obs[1]->SetShort("a priest");
    obs->move(environment(who));
    shout("%^RED%^Distant bells ring, signaling the execution "
            "of "+who->query_cap_name()+" in the town square.");
    call_out("squad_part_two", 2, who);
    return "The execution has begun.";
}

void squad_part_two(object who) {
    object pole, *obs;
    int x;

    message("say", "\n%^RED%^%^BOLD%^The guard tells you:%^RESET%^ Your "
            "time has come.", who);
    message("say", "\n%^RED%^Opening the cell, one of the guards ties "
            "your hands behind your back with a thick rope.  A guard then "
            "places a tight grip upon your arm and drags you out of the "
            "building and towards the town square.", who);
    new("/"+__DIR__+"obj/misc/handcuffs")->move(who);
    message("say", "A Praxis guard enters carrying a large bloodstained "
            "pole.  Walking to the far side of the square, he digs a small "
            "hole and firmly plants the pole.", DIR_STANDARD_DOMAIN+"/square");
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 1);
    clone_guards(1)->move(DIR_STANDARD_DOMAIN+"/square");
    pole=new(LIB_ITEM);
    pole->SetKeyName("pole");
    pole->SetId( ({ "pole", "bloodstained pole", "large pole" }) );
    pole->SetShort("a large, bloodstained pole");
    pole->SetLong("A large pole firmly planted in the ground.  Blood "
            "spots are scattered in various places near the top half of "
            "the pole.");
    pole->SetPreventGet("The pole is firmly planted in the ground.");
    pole->move(DIR_STANDARD_DOMAIN+"/square");
    message("say", "A small set of guards enter and form a line opposite "
            "of the pole.", DIR_STANDARD_DOMAIN+"/square");
    clone_guards(5)->move(DIR_STANDARD_DOMAIN+"/square");
    call_out("squad_part_three", 5, who);
}

void squad_part_three(object who) {
    object thing;
    if(thing=present("guard", environment(who))) 
        thing->move(DIR_STANDARD_DOMAIN+"/square");
    else clone_guards(1)->move(DIR_STANDARD_DOMAIN+"/square");
    if(thing=present("priest", environment(who))) 
        thing->move(DIR_STANDARD_DOMAIN+"/square");
    else clone_guards(1)->move(DIR_STANDARD_DOMAIN+"/square");
    who->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square");
    message("say", "\n%^RED%^In the once familar town square, a large "
            "area has been cleared out, with one pole near a wall on the "
            "far side of the square.  This pole is in the direction you "
            "and the ever stoic guard are headed.", who);
    message("say", "\n%^RED%^The guard ties your hands securly behind the "
            "rough blood-stained pole, facing a line of the town guards, "
            "each wielding a large bow with a single arrow.", who);
    message("say", "\n%^RED%^The guard pulls out a large black "
            "hood which is placed over your head and steps away.", who);
    new("/"+__DIR__+"obj/misc/hood")->move(who);
    message("say", who->query_cap_name()+" is dragged towards a pole "
            "on the far side of the square, where he is tied by one of "
            "the guards.  The guard places a large black hood over the "
            "criminal's head and steps away.", environment(who), who);
    message("say", "\nA nearby drum begins a steady, rythmic beat.",
            environment(who));
    call_out("squad_part_four", 4, who);
}

void squad_part_four(object who) {
    message("say", "\n%^RED%^The drums' beat becomes a steady roll, and "
            "you hear a guard shout a single, inaudiable word.  You hear "
            "the rustle of weapons being cocked.", who);
    message("say", "\nA lead guard yells out a single inaudiable word "
            "as the line of guards ready their weapons.", environment(who), 
            who);
    call_out("squad_part_five", 4, who);
}

void squad_part_five(object who) {
    message("say", "\n%^RED%^A guard yells a second word as the firing "
            "squad prepare the arrows for launching.", who);
    message("say", "\nThe lead guard yells a second word as the firing "
            "line pull back their arrows in preparation.", environment(who),
            who);
    call_out("squad_part_six", 4, who);
}

void squad_part_six(object who) {
    int x, dam;

    message("say", "\n%^RED%^The guard yells a thrid and final word, and "
            "dozens of arrows peirce your entire body.  The darkness surrounding "
            "you becomes even darker, and vague voices begin to cackle evily "
            "within you mind..", who);
    message("say", "\nThe guard yells out a third and final word, and "
            "the entire squad releases their arrows into the helpless body "
            "of "+who->query_cap_name()+".  The body instantly falls limp.",
            environment(who), who);
    dam= (who->query_max_hp())/5;
    present("hood", who)->destruct();
    present("handcuffs", who)->destruct();
    for(x=0; x<6; ++x) 
        who->do_damage("torso", dam + 10);
    call_out("squad_part_seven", 4, environment(who));
    who->die();
}

void squad_part_seven(object place) {
    object thing;
    message("say", "The guards begin marching away, one removing the pole "
            "on his way out.  He leaves the dead body on the "
            "ground to rot away, just as it deserves.", 
            DIR_STANDARD_DOMAIN+"/square");
    place->SetProperty("no bump", 0);
    while(thing = present("guard", place)) 
        thing->destruct();
    if(thing=present("pole", place)) thing->destruct();
}

    string death_by_torture(object who) {
        if(member_array(who, __Prisoners) == -1) 
            return who->query_cap_name()+" is not a prisoner.";
        message("say", "%^RED%^A large guard enters your cell and ties "
                "your hands with a large rope.  Saying nothing, he drags you "
                "out of the room.", who);
        new("/"+__DIR__+"obj/misc/handcuffs")->move(who);
        call_out("torture_part_two", 3, who);
        return "The execution has begun.";
    }

void torture_part_two(object who) {
    who->eventMoveLiving("/"+__DIR__+"torture_room");
    message("say", "%^RED%^The guard brings you into a room of which "
            "the likes you have only heard about.  Your heart sinks at the "
            "thought of parting with life in such a cruel way.", who);
    message("say", "\n%^RED%^The guard forces you to lie down on a "
            "small table in the middle of the room.   Heavy straps are "
            "placed over your legs, arms and forehead, making all "
            "movement impossible.  When you look up you finally see the "
            "object of your death: a large, clean blade at the end of "
            "a long pole.", who);
    call_out("torture_part_three", 4, who);
}

void torture_part_three(object who) {
    message("say", "\n%^RED%^%^BOLD%^The guard tells you: %^RESET%^Now "
            "just sit tight and it will all be over in a few hours.", who);
    message("say", "\nThe guard grins evilly.", who);
    message("say", "\n%^RED%^The guard reaches up and grabs the blade, "
            "swinging it like a pendulium.", who);
    call_out("torture_part_four", 4, who);
}

void torture_part_four(object who) {
    message("say", "\nThe guard ponders for a moment.", who);
    call_out("torture_part_five", 4, who);
}

void torture_part_five(object who) {
    message("say", "\n%^RED%^%^BOLD%^The guard tells you:%^RESET%^ Ya' "
            "know, I respect yer kind.  Nun too many pe'ple gots the balls "
            "ta do wha' ya did.", who);
    message("say", "\n%^RED%^%^BOLD%^The guard tells you:%^RESET%^ I's "
            "gonna show ya a lit'le bit o' mercy, make it quiker fer ya.", who);
    message("say", "\n%^RED%^The guard grabs the pole above the "
            "bladed pendulium and pulls down upon it, bringing the blade "
            "closer to your neck.  He gives it a hardy shove, and it begins "
            "it's steady swing again.", who);
    call_out("torture_part_six", 3, who);
}
void torture_part_six(object who) {
    message("say", "\n%^RED%^After a moment of careful examination, the "
            "guard gives a satisfied smirk.", who);
    message("say", "\n%^BOLD%^%^RED%^The guard tells you: %^RESET%^Have "
            "a happy afterlife.", who);
    message("say", "\n%^RED%^The guard grins evilly and stalks out of "
            "the room, locking the door behind him.", who);
    call_out("torture_part_seven", 6, ({ who, 1 }) );
}

void torture_part_seven(mixed *stuff) {
    object who;
    who = stuff[0];
    switch(random(7)) {
        case 0:
        case 1:
        case 2: tell_object(who, "\n%^RED%^You notice the swinging blade seems "
                        "to be getting closer"); break;
        case 3: tell_object(who, "\n%^RED%^You feel a very strong urge to "
                        "use the restroom."); break;
        case 4: tell_object(who, "\n%^RED%^Shivers run through your body.");
                break;
        case 5: tell_object(who, "\n%^RED%^A small rat scurries across the "
                        "floor");
        case 6: tell_object(who, "\n%^RED%^With closer examination, the blade "
                        "swinging just above your neck seems remarkably clean.  "
                        "You can't help but to wonder if you're the first life "
                        "to be claimed by this horrific device.");
    }
    if(++stuff[1] == 10) 
        call_out("torture_part_eight", 5, who);
    else call_out("torture_part_seven", 6, stuff);
}

void torture_part_eight(object who) {
    message("say", "\n%^RED%^The blade swings dangerously close to your "
            "neck.  By your best estimate, you have a single breath left...",
            who);
    call_out("torture_part_nine", 3, who);
}

void torture_part_nine(object who) {
    message("say", "\n%^RED%^In one final swoop, the huge metal blade "
            "places a clean cut into your throat.  Your struggle for more "
            "air only fills your mouth and lungs with your life blood.", who);
    message("say", "\n%^BOLD%^%^BLACK%^The ever-persistant darkness "
            "finally covers your eyes.  Distant, evil voices cackle with "
            "glee at thier newest victim...", who);
    present("handcuffs", who)->destruct();
    who->die();
}

    string death_by_the_pit(object who) {
        if(member_array(who, __Prisoners) == -1)
            return who->query_cap_name()+" is not a prisoner.";
        message("say", "%^RED%^A large, burly guard enters the cell and "
                "securly ties your hands with a peice of rope.", who);
        message("say", "\n%^RED%^%^BOLD%^The guard tells you:%^RESET%^ It's "
                "to the pit with your sorry self.  Hehehe.", who);
        message("say", "\n%^RED%^The guard drags you out of the building "
                "towards the dreaded pit...", who);
        shout("%^RED%^Distant bells signal the pending death "
                "of "+who->query_cap_name()+" in the dreaded Pit of Spiders.");
        new("/"+__DIR__+"obj/misc/handcuffs")->move(who);
        call_out("pit_part_two", 5, who);
        return "The execution has begun.";
    }


void pit_part_two(object who) {
    string orig_long;

    message("say", "Two guards enter and clear off an area "
            "revealing a large stone covering.  The guards lift the heavy "
            "plate, opening a deep, black hole on the northeastern side of "
            "the square.", DIR_STANDARD_DOMAIN+"/square");
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 1);
    orig_long = (DIR_STANDARD_DOMAIN+"/square")->GetLong();
    (DIR_STANDARD_DOMAIN+"/square")->AddExit("down",
            DIR_STANDARD_DOMAIN+"/spider_pit", (: "prevent_down" :) );
    (DIR_STANDARD_DOMAIN+"/square")->SetLong(
            orig_long+"\n\nA large, deep pit has been uncovered in "
            "the middle of the square.");
    clone_guards(2)->move(DIR_STANDARD_DOMAIN+"/square");
    call_out("pit_part_three", 3, ({ who, orig_long }));
}

void pit_part_three(mixed *stuff) {
    stuff[0]->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square");
    message("say", "\n%^RED%^Upon the arrival of the all too familar "
            "town square, two guards have already removed the cover from "
            "the pit.  Deep in the dark hole, the erie floor seems to "
            "be crawling...", stuff[0]);
    message("say", "\n"+stuff[0]->query_cap_name()+" enters, dragged by the "
            "town guards.", environment(stuff[0]), stuff[0]);
    call_out("pit_part_four", 4, stuff);
}

void pit_part_four(mixed *stuff) {
    object here;

    message("say", "\n%^RED%^%^BOLD%^A guard tells you:%^RESET%^ Say "
            "hi to the Leaper for me!", stuff[0]);
    message("say", "\n%^RED%^The guard cackles evilly as he shoves you "
            "into the pit.", stuff[0]);
    message("say", "\nThe guard cackles evilly as he shoves "+
            stuff[0]->query_cap_name()+" into the pit", environment(stuff[0]),
            stuff[0]);
    here=environment(stuff[0]);
    stuff[0]->eventMoveLiving("/"+__DIR__+"spider_pit",
            "falls screaming into the pit!!");
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 0);
    message("say", "\nThe guard pulls the protective covering over the "
            "hole.  Muffled screams of pain echo from within as you realize "
            "the criminal's chances of a peacful death are zero.", 
            DIR_STANDARD_DOMAIN+"/square");
    call_out("pit_part_five", 10, ({ here, stuff[1] }));
}

void pit_part_five(mixed *stuff) {
    object thing;

    message("say", "The guards, satisfied that this world is a better "
            "place, replace the cover upon the pit of spiders and"
            "walk off.", stuff[0]);
    stuff[0]->RemoveExit("down");
    stuff[0]->SetLong(stuff[1]);
    if(thing = present("guard", stuff[0])) thing->destruct();
    if(thing = present("guard", stuff[0])) thing->destruct();
}

int prevent_down() {
    message("say", "A guard blocks your way, making it clear that "
            "you really don't want to go down there.", this_player());
    return 0;
}

    string death_by_stoning(object who) {
        if(member_array(who, __Prisoners) == -1) 
            return who->query_cap_name()+" is not a prisoner.";
        message("say", "%^RED%^A guard enters and ties your hands together "
                "with a thick rope.", who);
        message("say", "\n%^BOLD%^%^RED%^A guard tells you:%^RESET%^ Right "
                "this way, you scumbag.", who);
        message("say", "\n%^RED%^The guard grabs you by the arm and drags you "
                "towards the town square.", who);
        // shout("%^RED%^Distant bells sound, signaling the execution "
        // "of "+who->query_cap_name()+" by stoning in the town square.");
        call_out("stoning_part_two", 5, who);
    }

void stoning_part_two(object who) {
    object *townsfolk, *homes;
    string *wanted;
    int x, y;

    wanted = ({ "horace", "lars", "beggar", "knight", "waitress" });
    townsfolk=({ });
    homes = ({ });
    for(x=0, y=0; y<sizeof(wanted); y++)
        if(find_living(wanted[y])) {
            homes[x]=environment(townsfolk[x]);
            message("say", townsfolk[x]->query_cap_name()+" cheers in excitment "
                    "for the upcomming execution!", environment(townsfolk[x]));
            x++;
        }
    townsfolk->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square", "to join in the "
            "stoning of "+who->query_cap_name());
    call_out("control_townsfolk", 1, ({ who, townsfolk, homes }));
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 1);
    new("/"+__DIR__+"obj/misc/stones")->move(DIR_STANDARD_DOMAIN+"/square");
    who->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square");
    message("say", "\n%^RED%^Upon arrival to the town square, the guard "
            "ties you to a wooden post, facing the jeering crowd.   With a "
            "grim smirk upon his face, he wishes you a good afterlife and "
            "leaves you to die at the hands of the countless victims of "
            "your crimes.", who);
    message("say", "Upon arrival to the town square, the guard ties "+
            who->query_cap_name()+" to the post, smirks, and leaves.  "
            "Immedately the towns people pick up their rocks and expend "
            "thier frustration upon the criminal who caused them so much "
            "grief.", environment(who), who);
}

void control_townsfolk(mixed *them) {
    int x;

    for(x=0; x<sizeof(them[1]); x++)
        switch(random(5)) {
            case 0 : message("saY", them[1][x]->query_cap_name()+" cheers "
                             "enthustically!", environment(them[1][x])); break;
            case 1 : message("say", them[1][x]->query_cap_name()+"yells: "
                             "Scumbag!!", environment(them[1][x])); break;
            case 2 : message("say", them[1][x]->query_cap_name()+" spits on " +
                             them[0]->query_cap_name()+"!", environment(them[1][x]));
                     break;
            case 3 : message("say", them[1][x]->query_cap_name()+"boos "
                             "and hisses.", environment(them[1][x])); break;
            case 4 : them[1][x]->command("get stone");
                     them[1][x]->command("throw stone at "+them[0]->query_name());
                     break;
        }
    if(them[0]->query_ghost()) {
        them[1]->command("cheer");
        call_out("stoning_part_three", 3, ({ them[1], them[2] }) );
    } else call_out("control_townsfolk", 1, them);
}

void stoning_part_three(mixed *townsfolk) {
    int x;
    message("say", "The townsfolk head back to their respective "
            "homes.", environment(townsfolk[0][0]));
    present("stone pile", environment(townsfolk[0][0]))->destruct();
    for(x=0; x < sizeof(townsfolk[0]); x++)
        townsfolk[0][x]->eventMoveLiving(townsfolk[1][x]);
}

    string death_by_beheading(object who) {
        if(member_array(who, __Prisoners) == -1) 
            return who->query_cap_name()+" is not a prisoner.";
        message("say", "%^RED%^A large, burly guard enters your cell and securly "
                "ties your hands with a thick rope.  He grabs your arm and pulls "
                "you towards the exit of the prison, in the direction of the town "
                "square.", who);
        shout("%^RED%^Distant bells ring thrice, signaling the "
                "public beheading of "+who->query_cap_name()+" at the town "
                "square");
        new("/"+__DIR__+"obj/misc/handcuffs")->move(who);
        call_out("beheading_part_two", 2, who);
        return "The execution has begun";
    }

void beheading_part_two(object who) {
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 1);
    clone_guards(4)->move(DIR_STANDARD_DOMAIN+"/square");
    message("say", "A group of guards enter, bringing with them a large "
            "platform with a thick round wooden stump covered with dried "
            "blood.", DIR_STANDARD_DOMAIN+"/square");
    who->eventMoveLiving(DIR_STANDARD_DOMAIN+"/square");
    message("say", "\n%^RED%^Upon arrival to the town square, you notice "
            "a large platform with a blood-stained wooden stump.  Standing "
            "atop the platform is a black-hooded man with a large axe.  It "
            "is upon this platform, next to the hooded man, where you are "
            "lead by the guard and commanded to kneal.", who);
    call_out("beheading_part_three", 3, who);
}

void beheading_part_three(object who) {
    message("say", "\n%^RED%^From atop the platform you see the gathering "
            "crowd, very unfriendly.  A rope is placed around your neck, and "
            "you are forced to place your head over the stump, you neck "
            "fearfully exposed to the man in the black hood.  A nearby drum "
            "begins it's steady death beat...", who);
    message("say", "\nA guard places a rope around "+who->query_cap_name()+
            "'s neck and is forced to lie over the stump.  There is a desparate "
            "look on "+who->query_cap_name()+"'s face as a nearby drum begins "
            "a steady beat...", environment(who), who);
    call_out("beheading_part_four", 3, who);
}

void beheading_part_four(object who) {
    message("say", "\n%^RED%^Without warning, the drum beat comes to a "
            "sudden stop.", who);
    message("say", "\nThe drum beat comes to a sudden end, and the black "
            "hooded brings his axe down through "+who->query_cap_name()+"'s "
            "neck without hesitation.  "+who->query_cap_name()+"'s head drops "
            "to the platform with a loud thunk, the expression of desparation "
            "permanently affixed to "+who->query_possessive()+" face.", 
            environment(who), who);
    (DIR_STANDARD_DOMAIN+"/square")->SetProperty("no bump", 0);
    who->die();
}


varargs mapping valid_execution( string type )
{
    if( type ) return Executions[type];
    return Executions;
}

string execute( object who, string type )
{
    return call_other( this_object(), Executions[type], who );
}
