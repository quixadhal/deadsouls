#include <lib.h>
#include <position.h>

inherit LIB_BARKEEP;

string array flatterers, specials;
string save_str = "";
string saved_str = "";

mixed NoKill(object attacker);

int FlatterResponse(mixed args...){
    string who = args[0]->GetKeyName();
    if(member_array(who, flatterers) == -1){
        flatterers += ({ args[0]->GetKeyName() });
        flatterers = distinct_array(flatterers);
        save_str = save_variable(flatterers);    
        unguarded( (: write_file("/domains/town/save/brandy.o", save_str, 1) :) );
        args[0]->AddExperiencePoints(300);
    }
    this_object()->eventForce("say Thank you, "+args[0]->GetName()+". Feel free to ask me "+
      "for my special menu.");
    return 1;
}

static void create() {
    barkeep::create();
    flatterers = ({}); 
    unguarded( (: saved_str = read_file("/domains/town/save/brandy.o") :) );
    if(sizeof(saved_str)) flatterers = restore_variable(saved_str);
    SetKeyName("brandy");
    SetAdjectives( ({ "bar", "fine", "bartender", "keeper", "good", "serving" }) );
    SetId( ({ "brandy", "barmaid", "barkeep", "bartender", "keeper", "girl", "wench", "maid" }) );
    SetShort("Brandy the barmaid");
    SetLevel(1);
    SetLong("With eyes that could steal a sailor from the sea, Brandy "
      "works in this harbor town laying whiskey down, fetching round "
      "after round for her fiercely loyal clientele. Word is she's receptive "
      "to flattery.");
    SetMenuItems(([
                ({ "guinness","beer","stein" }) : "/domains/town/meals/stein",
                ({ "wine","bottle of wine" }) : "/domains/town/meals/winebottle",
                ({ "whiskey","bottle of whiskey" }) : "/domains/town/meals/rebel_yell",
                ]));
    SetSpecialMenuItems(([
        "javarin" : "/domains/town/meals/javarin",
        "speedalin" : "/domains/town/meals/speedalin",
        "spazalin" : "/domains/town/meals/spazalin",
        //"somnitol" : "/domains/town/meals/potion_healing",
        //"phoqitol" : "/domains/town/meals/potion_healing",
        //"comatol" : "/domains/town/meals/potion_healing",
        "red spice" : "/domains/town/meals/red_spice",
        "green spice" : "/domains/town/meals/green_spice",
        "blue spice" : "/domains/town/meals/blue_spice",
    ]));
    SetGender("female");
    SetMorality(40);
    SetRace("human");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("gold");
    SetAttackable( (: NoKill :) );
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
    SetInventory(([
      "/domains/town/armor/locket" : "wear necklace",
      "/domains/town/armor/barsmock" : "wear smock",
    ]));
    SetTalkResponses( ([
        "randy, you're a fine girl" : (: FlatterResponse :),
        "randy, youre a fine girl" : (: FlatterResponse :),
        "randy you're a fine girl" : (: FlatterResponse :),
        "randy youre a fine girl" : (: FlatterResponse :),
        "what a good wife you would be" : (: FlatterResponse :),
    ]));
}

void init(){
    ::init();
}

mixed eventSell(object who, string args){
    tc("flatterers: "+identify(flatterers));
    if(member_array(args, GetSpecials()) != -1 &&
      member_array(who->GetKeyName(), flatterers) == -1){
        eventForce("say I don't feel comfortable selling that to you, "+
        who->GetName()+".");
      return 1;
    }
    return ::eventSell(who, args);
}

varargs int eventSpecialList(object who, string what){
    if(member_array(who->GetKeyName(), flatterers) == -1){
      eventForce("say I'm not sure I have a special menu for you, "+
        who->GetName()+".");
      return 1;
    }
    return ::eventSpecialList(who, what);
}

mixed NoKill(object attacker){
    return "Brandy is such a fine girl you find yourself unable to attack her.";
}
