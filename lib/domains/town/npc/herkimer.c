#include <lib.h>
#include <daemons.h>

inherit LIB_LEADER;
int JoinGuild(object ob, string verb, string what);
int TeachSpell(object who, string verb, string spell);

string *spells;
mapping AvailableSpells, RestrictedSpells;

int WieldStaff(){
    if(!present("staff",this_object())){
        new("/domains/town/weap/leostaff")->eventMove(this_object());
        tell_room(environment(),"Herkimer claps his hands and a large "+
                "wooden staff materializes.");
        this_object()->eventForce("wield staff");
        this_object()->eventForce("say you poor fool!");
        this_object()->eventForce("cast buffer");
    }
    return 1;
}

static void create() {
    leader::create();
    SetKeyName("herkimer");
    SetId(({"archwizard","wizard","herkermer","herkirmer"}));
    SetAdjectives(({"old","gentle","kindly"}));
    SetShort("Herkimer the kind wizard");
    SetLong("This kindly old wizard is the master of the "+
            "local magic shop, which doubles as the Mages' Guild. "+
            "You can ask Herkimer to teach you a spell, and if "+
            "you're capable of learning it, he will teach you, "+
            "for a fee. His penetrating blue eyes seem to "+
            "see right through you, but the smile from beneath "+
            "his white beard is reassuring.");
    SetLevel(300);
    SetUnique(0);
    SetRace("human");
    SetGender("male");
    SetClass("mage");
    SetInventory(([
                "/domains/town/armor/robe":"wear robe",
                "/domains/town/armor/wizard_hat":"wear hat",

                ]));
    SetSpellBook( ([ "buffer" : 100, "meditate" : 100, "missile" : 100, "fireball" : 100, "whip" : 100 ]) );
    SetAction(5, ({
                "Herkimer scratches his beard thoughtfully.",
                "Herkimer seems to be trying to remember something.", "Herkimer ponders.",
                "Herkimer thinks.", "Herkimer thinks carefully.",
                "Herkimer says, \"%^BOLD%^CYAN%^You don't have to be a mage to learn a spell, but it sure helps.%^RESET%^\"",
                "Herkimer says, \"%^BOLD%^CYAN%^The more you use your magic skills, the more proficient you become.%^RESET%^\"",
                "Herkimer says, \"%^BOLD%^CYAN%^Casting spells costs you mana points. Make sure you don't run out unexpectedly!%^RESET%^\"",
                "Herkimer says, \"%^BOLD%^CYAN%^If you join us and then leave the guild, you will not be alowed to become a mage again.%^RESET%^\"",
                "Herkimer says, \"%^BOLD%^CYAN%^Some spells require total concentration. If you move or if you are attacked, it may interrupt the casting.%^RESET%^\"",
                "Herkimer says, \"%^BOLD%^CYAN%^All magical training is free of charge to guild members, of course.%^RESET%^\"",
                }));
    SetCombatAction(50, ({ (: WieldStaff :), "say what is your deal?",
                "say don't make me destroy you","cast fireball", "cast missile", "cast buffer"}) );
    SetCommandResponses( ([
                "join": (: JoinGuild :),
                "teach" : (: TeachSpell :),
                "learn" : "I have much to learn, young one, but not from you.",
                ]) );
    AvailableSpells = ( ([
                "fireball" : 1000,
                "greater fireball" : 3000,
                "light" : 100,
                "missile" : 500,
                "buffer" : 200,
                "greater buffer" : 2000,
                "meditate" : 500,
                "whip" : 10000,
                ]) );
    RestrictedSpells = ( ([
                "greater fireball" : 3000,
                "light" : 100,
                "greater buffer" : 2000,
                "whip" : 10000,
                ]) );
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
    SetCustomXP(10);
}

void init(){
    string key;
    int val;
    ::init();
    spells = ({});
    foreach(key,val in AvailableSpells){
        spells += ({ key });
    }
}

int JoinGuild(object ob, string verb, string what){
    if(ob->GetClass() == "mage") {
        eventForce("say You're already in, doofus.");
        return 1;
    }
    eventForce("say Hmm...want to join our guild, do you?");

    if(!this_player()->GetQuest("Orc Slayer Quest")){
        eventForce("say First you must prove yourself worthy. Help my brother Leo find the Orcslayer and I will happily welcome you into our family. Until then, you may not join the mages.");
        return 1;
    }
    if(ob->GetClass() == "explorer" || !ob->GetClass()){
        eventForce("say very well, "+ob->GetName()+".");
        eventForce("say Welcome to the mages' guild! Now you are *extremely* magic! You must practice your magic often, since you are now physically weaker than before. The stronger your magic, the stronger you are!");
        ob->ChangeClass("mage");
#if 0
        if(ob->GetLevel() == 1){
            eventForce("say you have been demoted to a Level 1 player. However, you still retain your experience and your questing history, so just ask Dirk to promote you again a few times.");
        }
#endif
        return 1;
    }
    eventForce("say I'm sorry, it looks to me like you have already chosen your specialty. As a member of the Guildmasters' Guild, I am forbidden from removing you from the "+capitalize(ob->GetClass())+"'s Guild.");
    return 1;
}

int TeachSpell(object who, string verb, string spell){
    int cost, onhand;
    mapping myspells;
    if(!spell || spell == ""){
        eventForce("say Please ask again, but this time specify the spell you'd like to learn.");
        return 1;
    }
    if(!SPELLS_D->GetSpell(spell)){
        eventForce("say I'm sorry, that is not a spell I can teach you.");
        return 1;
    }
    if(who->GetClass() != "mage" &&
            member_array(spell, keys(RestrictedSpells)) != -1){
        eventForce("That spell is for guild members only.");
        return 1;
    }
    if(member_array(spell,spells) == -1){
        eventForce("say That spell is not available right now.");
        eventForce("say The spells I can teach you are "+implode(spells,", ")+".");
        return 1;
    }
    myspells = who->GetSpellBook();
    if(myspells[spell]){
        eventForce("say You already know that spell.");
        return 1;
    }
    cost = AvailableSpells[spell];
    onhand = who->GetCurrency("silver");
    if(who->GetClass() != "mage" && onhand < cost) {
        eventForce("say You lack enough silver coins to pay for that spell.");
        eventForce("say "+spell+" costs "+cost+" silver and you "+
                "only have "+onhand+".");
        eventForce("say I can only accept silver. It's a magic thing. If you want, you can try to exchange your other currency for silver at the bank across the street.");
        return 1;
    }
    tell_room(environment(this_object()),"Herkimer closes his eyes, mutters under his breath, and waves his hands.");
    if(!who->eventLearnSpell(spell)){
        eventForce("shake");
        eventForce("say You do not appear capable of learning that spell right now.");
        return 1;
    }
    eventForce("smile");
    eventForce("say Congratulations. You now possess the mystical knowledge of the "+spell+" spell. Use it wisely.");
    if(who->GetClass() != "mage") who->AddCurrency("silver",-cost);
    return 1;
}

int eventReceiveObject() {
    object ob, player;
    ob = previous_object();
    player = this_player();

    if( !ob || !::eventReceiveObject() ) return 0;
    if( ob->GetKeyName() == "orc slayer" ) {
        eventForce("say I'm sorry, you've got the wrong archwizard.");
        eventForce("say You're looking for Leo. His workroom is the church basement.");
        eventForce("give orcslayer to "+this_player()->GetKeyName());
        return 1;
    }

    AddCarriedMass(ob->GetMass());
    return 1;
}
