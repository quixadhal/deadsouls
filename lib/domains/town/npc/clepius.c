#include <lib.h>

inherit LIB_NPC;

object *slips;
int busy;

static void create() {
    npc::create();
    SetKeyName("clepius");
    SetId(({"clepius","doctor","healer"}));
    SetAdjectives(({"a.s.", "A.S.", "doctor","dr","Dr.","dr."}));
    SetShort("Clepius the healer");
    SetLong("Clepius is a kindly old man, legendary for his "
            "nearly superhuman powers of healing. He is even "
            "rumored capable of resurrecting the dead. He runs "
            "a medical care service in town, and can be asked to "
            "help when needed, though his help is not free.");
    SetLevel(50);
    SetRace("human");
    SetClass("mage");
    SetGender("male");
    SetInventory(([
                "/domains/town/weap/rod":1,
                "/domains/town/obj/medbag":1,
                "/domains/town/armor/toga":"wear toga",
                ]) );
    slips = ({});
    busy=0;
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
    SetCustomXP(10);
}

void init(){
    ::init();
}

int CanReceive() {
    if(present("healer token", this_object())){
        return 0;
    }

    eventForce("nod");
    set_heart_beat(1);
    return 1;
}

int CheckBag(){
    object bag,slip;
    string player, problem;
    slip = present("healer token", this_object());
    if(!slip && busy == 1) {
        eventForce("say That's a bit odd...\n\n");
        eventForce("I'm sorry but I must have misplaced your slip. Do you have another one?");
        busy = 0;
        return 1;
    }
    if(slip){
        busy = 1;
        eventForce("say mmhmmm...");
        problem = slip->GetProperty("problem");
        player = slip->GetPatient();
        this_object()->DiagPatient(player,problem);
    }
    return 1;
}

void heart_beat(){
    if(busy == 0 && present("healer token", this_object())){
    }

    this_object()->CheckBag();
}

int ejectRabble(string str){
    object *riffraff,patient;
    environment()->SetProperty("busy",1);
    riffraff = get_livings(environment(this_object()));
    if(! patient = present(str,environment(this_object())) ){
        patient = find_player(str);
    }
    foreach(object bum in riffraff){
        if( bum->GetKeyName() != patient->GetKeyName() &&
                bum->GetKeyName() != "clepius" ){
            tell_object(bum,"The doctor ejects you from his office in order to "+
                    "treat "+patient->GetName()+".\n\n");
            bum->eventMoveLiving("/domains/town/room/healer");
            tell_object(patient,"The doctor ejects "+bum->GetName()+" from "+
                    "his office in order to treat you.");
        }
    }
    return 1;
}

int NextPatient(){
    eventForce("put my first slip in bin");
    environment()->SetProperty("busy",0);
    tell_room(load_object("/domains/town/room/healer"),"From the back room "+
            "you hear the doctor holler: \"%^BOLD%^CYAN%^NEXT!%^RESET%^\"");
    tell_room(environment(),"The doctor leans into the east doorway "+
            "and hollers: \"%^BOLD%^CYAN%^NEXT!%^RESET%^\"");
    busy = 0;
    environment()->SetProperty("busy",0);
    return 1;

}

int PerformHeal(string dude){
    object person;
    int hp,mhp;
    person = present(dude,environment());
    if(!person) {
        eventForce("look");
        eventForce("shrug");
        return 1;
    }
    eventForce("say All right...let's take a look at you.");
    hp = person->GetHealthPoints();
    mhp = person->GetMaxHealthPoints()-10;
    if(hp > mhp){
        eventForce("say You look fine to me. Take back your slip and save it for when you really need it.");
        eventForce("give my first slip to "+dude);
        return 1;
    }
    if(present("clepius mojo",person)){
        eventForce("say You already have salve on you. Give it a chance to work, then come back later.");
        eventForce("give my first slip to "+dude);
        return 1;
    }
    eventForce("say Yes, you can use a little help.");
    tell_room(environment(),"Clepius vigorously rubs a healing salve onto your body. You can feel it enhancing your body's recovery.");
    new("/secure/obj/mojo")->eventMove(person);
    return 1;
}

int PerformRegenerate(string dude){
    int i;
    mapping stumps;
    object person;
    person = present(dude,environment());
    stumps=person->GetMissingLimbs();

    eventForce("say Let's count limbs, then. Hmmmm...");

    if(!sizeof(stumps)) {
        eventForce("say You are not missing any limbs. Go sell this slip back to James.");
        eventForce("give my first slip to "+dude);
        return 1;
    }
    for(i=0;i<sizeof(stumps);i++){
        person->RestoreLimb(stumps[i]);
        tell_object(person,"The doctor regenerates your "+stumps[i]+".");
        return 1;
    }
}

int PerformExcision(string dude){
    string what,whom;
    object person,thing,slug;
    int firearms_wounds, wounds;

    person = present(dude,environment(this_object()));
    slug=present("firearms_wound",person);
    wounds=0;
    wounds=person->GetLead();
    if(!slug){
        eventForce("say You have no foreign bodies.");
        eventForce("give my first slip to "+dude);
        return 1;
    }

    tell_room(environment(this_object()),"Clepius deftly extracts a lead slug from "+ dude+".", ({person}));
    tell_object(person,"Clepius deftly extracts a lead slug from your body.");
    slug=new("/domains/town/obj/spent");
    if(wounds > 0) {
        person->AddLead("firearms_wounds", -1);
        slug->SetShort("a spent firearm slug");
        --wounds;
        slug->eventMove(this_object());
        if(person->GetLead() < 1){                                         
            slug=present("firearms_wound",person);
            if(slug) slug->eventDestruct();
        }
        return 1;
    }

    slug->eventMove(this_object());
    slug=present("firearms_wound",person);
    if(wounds < 1 && slug){                                 
        slug->eventDestruct();
    }
    return 1;
}

int DiagPatient(string dude, string problem){
    if(!present(dude,environment())) {
        NextPatient();
        return 1;
    }

    ejectRabble(dude);

    if(problem == "regenerate") {
        PerformRegenerate(dude);
        NextPatient();
    }
    if(problem == "heal"){
        PerformHeal(dude);
        NextPatient();
    }
    if(problem == "excise"){
        PerformExcision(dude);
        NextPatient();
    }
    return 1;
}

