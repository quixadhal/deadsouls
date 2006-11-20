/*    /lib/player.c
 *    from the Dead Souls LPC Library
 *    the player object
 *    created by Descartes of Borg 950321
 *    Version: @(#) player.c 1.26@(#)
 *    Last Modified: 97/01/03
 */

#include <lib.h>
#include <rooms.h>
#include <damage_types.h>
#include <message_class.h>
#include <daemons.h>
#include <config.h>
#include <vision.h>
#include <position.h>
#include "include/player.h"

inherit LIB_INTERACTIVE;
inherit LIB_LIVING;

private string *Titles;
string *Muffed = ({});
private class quest *Quests;
private mapping *Deaths;
private int TrainingPoints, TitleLength;

/* *****************  /lib/player.c driver applies  ***************** */

static void create() {
    interactive::create();
    living::create();

    Titles = ({});
    TitleLength = 1;
    Quests = ({});
}

static void heart_beat() {

    if(!interactive(this_object())) {
	set_heart_beat(0);
	return;
    }
    interactive::heart_beat();
    if( query_idle(this_object()) >= IDLE_TIMEOUT && !creatorp(this_object()) && !present("testchar badge",this_object()) ) {
	cmdQuit();
	return;
    }
    living::heart_beat();
}

static void net_dead() {
    interactive::net_dead();
    set_heart_beat(0);
}

static int Destruct() {
    CHARACTER_D->eventSaveTime();
    living::Destruct();
    return interactive::Destruct();
}

mixed eventAsk(object who, string what) {
    if( what != "convert me" ) return 0;
    if( !GetReligion(1) ) return 0;
    who->SetProperty("converting", GetReligion(1));
    return 1;
}

void eventKillEnemy(object ob) {
    living::eventKillEnemy(ob);
    STATISTICS_D->eventKill(ob);
}

void eventReconnect() {
    interactive::eventReconnect();
    living::eventReconnect();
    set_heart_beat(GetHeartRate());
}

/************** player.c command functions *************/
varargs int eventShow(object who, string str) {
    if( !living::eventShow(who, str) ) return 0;
    if( this_player() != this_object() )
	eventPrint((string)this_player()->GetName() + " looks you over.");
    return 1;
}

/* *****************  /lib/player.c events  *************** */

int eventDisplayStatus() {
    string str;
    int qp, xp, hp, mp, sp, max_hp, max_mp, max_sp;

    hp = GetHealthPoints();
    max_hp = GetMaxHealthPoints();
    mp = GetMagicPoints();
    max_mp = GetMaxMagicPoints();
    sp = GetStaminaPoints();
    max_sp = GetMaxStaminaPoints();
    xp = GetExperiencePoints();
    qp = GetQuestPoints();

    if( percent(hp, max_hp) < 20.0 )
	str = "%^YELLOW%^hp: %^RED%^" + hp + "%^RESET%^/" + max_hp;
    else str = "%^YELLOW%^hp: %^RESET%^" + hp + "/" + max_hp;
    if( percent(mp, max_mp) < 20.0 )
	str += "   %^BLUE%^mp: %^RED%^" + mp + "%^RESET%^/" + max_mp;
    else str += "   %^BLUE%^mp: %^RESET%^" + mp + "/" + max_mp;
    if( percent(sp, max_sp) < 20.0 )
	str += "   %^GREEN%^sp: %^RED%^" + sp + "%^RESET%^/" + max_sp;
    else str += "   %^GREEN%^sp: %^RESET%^" + sp + "/" + max_sp;
    str += "   %^MAGENTA%^xp: %^RESET%^" + xp;
    str += "   %^CYAN%^qp: %^RESET%^" + qp;
    message("status", str, this_object());
    return 1;
}

static void eventDestroyUndead(object agent) {
}

varargs int eventDie(mixed agent) {
    int x, expee, subexpee;

    if(!agent) agent = previous_object();
    if(!agent) agent = this_object();

    if( (x = living::eventDie(agent)) != 1 ) return x;

    if(!Deaths || !sizeof(Deaths)) 
	Deaths = ({([ "date" : ctime(time()), "enemy" : ((agent->GetName()) ? agent->GetName() : agent) ])});
else Deaths += ({ ([ "date" : ctime(time()), "enemy" : agent->GetName() ]) });

if( !GetUndead() ) {
    eventDestroyUndead(agent);
}
else {
    message("my_action", "Consciousness passes from you after one last "
      "gasp for air.", this_object());
    message("my_action", "You awake, but you find your body feels "
      "different, and the world about you is unfamiliar.",
      this_object());
    if( agent ) {
	message("other_action", GetName() + " drops dead by the hand "
	  "of " + (string)agent->GetName() + ".",
	  environment(this_object()), ({ agent, this_object() }));
	message("other_action", "You send " + GetName() + " into the "
	  "Underworld.", agent);
    }
    else message("other_action", GetName() + " drops dead.",
	  environment(), ({ this_object() }) );

    NewBody(GetRace());

    expee = this_object()->GetExperiencePoints();
    subexpee = to_int(expee * 0.25);

    eventCompleteHeal(GetMaxHealthPoints()/2);
    AddMagicPoints(-(random(GetMagicPoints())));
    this_object()->eventMove(ROOM_DEATH);
    this_object()->AddExperiencePoints(-subexpee);
    this_object()->save_player((string)this_object()->GetKeyName());
    this_object()->eventForce("look");
}
flush_messages();
return 1;
}

mixed eventTurn(object who) {
    if( !living::eventTurn(who) ) {
	return 0;
    }
    eventDestroyUndead(who);
    return 1;
}

void eventRevive() {
    string skill;

    this_object()->SetDead(0);
    this_object()->SetDeathEvents(0);

    if( !GetUndead() ) return;
    SetUndead(0);
    if(this_player()->GetPoison() > 0){
	this_player()->AddPoison(0 - this_player()->GetPoison());
    }
    foreach(skill in GetSkills()) {
	int x;

	if( !random(4) ) {
	    continue;
	}
	if( newbiep(this_object()) ) {
	    x = 2;
	}
	else {
	    x = 10;
	}
	x = random(x - (2*GetSkillClass(skill)))/2;
	if( x > 0 ) {
	    while( x-- ) {
		AddSkillPoints(skill,
		  -GetMaxSkillPoints(skill,
		    GetBaseSkillLevel(skill)));
	    }
	}
    }
    NewBody(GetRace());
    eventCompleteHeal(GetMaxHealthPoints());
    SetSpecialTarget( ({}) );
    AddMagicPoints(-(GetMaxMagicPoints()/2));
    AddStaminaPoints(-(GetMaxStaminaPoints()/2));
    AddHealthPoints(-(GetMaxHealthPoints()/2));
    if(creatorp()) interactive::SetShort("$N the reborn");
}

int eventMove(mixed dest) {
    int ret;

    if(environment(this_player()) && base_name(environment(this_player()))) {
	this_player()->SetProperty("LastLocation",
	  base_name(environment(this_player())));
    }

    ret = interactive::eventMove(dest);
    if( this_object() && environment(this_object())) eventMoveFollowers(environment(this_object()));
    return ret;
}

varargs int eventMoveLiving(mixed dest, string omsg, string imsg) {
    object *inv;
    object prev;
    string prevclim, newclim;

    if( prev = environment() ) {
	prevclim = (string)prev->GetClimate();
	if( stringp(dest) ) {
	    if(dest[0] != '/') {
		string *arr;

		arr = explode(file_name(prev), "/");
		dest = "/"+implode(arr[0..sizeof(arr)-2], "/")+"/"+dest;
	    }
	}
	if( !eventMove(dest) ) {
	    eventPrint("You remain where you are.", MSG_SYSTEM);
	    return 0;
	}
	inv = filter(all_inventory(prev), (: (!GetInvis($1) && living($1) &&
	      !GetProperty("stealthy") &&    
	      ($1 != this_object())) :));
	if( !omsg || omsg == "" ) {
	    omsg = GetMessage("telout");
	    imsg = GetMessage("telin");
	}
	else if(GetPosition() == POSITION_SITTING ||
	  GetPosition() == POSITION_LYING ){
	    omsg = GetName()+" crawls "+omsg+".";
	    imsg = GetName()+" crawls in.";
	}
	else if(GetPosition() == POSITION_FLYING ){
	    omsg = GetName()+" flies "+omsg+".";
	    imsg = GetName()+" flies in.";
	}

	else {
	    omsg = GetMessage("leave", omsg);
	    imsg = GetMessage("come", imsg);
	}
	inv->eventPrint(omsg, MSG_ENV);
    }
    else if( !eventMove(dest) ) {
	eventPrint("You remain where you are.", MSG_SYSTEM);
	return 0;
    }
    inv = filter(all_inventory(environment()),
      (: (!GetInvis($1) && !GetProperty("stealthy") &&
	  living($1) && ($1 != this_object())) :));

    inv->eventPrint(imsg, MSG_ENV);
    if(GetInvis()) {
	if(!creatorp(this_object())) AddStaminaPoints(-(15-(GetSkillLevel("stealth")/10)));
	AddSkillPoints("stealth", 30 + GetSkillLevel("stealth")*2);
	eventPrint("%^RED%^You move along quietly....%^RESET%^\n");
    }
    if(GetProperty("stealthy")) {
	if(!creatorp(this_object())) AddStaminaPoints(-3 - random(3));
	AddSkillPoints("stealth", 10 + GetSkillLevel("stealth")*2);
    }
    eventDescribeEnvironment(GetBriefMode());
    newclim = (string)environment()->GetClimate();
    if( !GetUndead() ) switch( newclim ) {
    case "arid":
	if(!creatorp(this_object())) AddStaminaPoints(-0.3);
	break;
    case "tropical":
	if(!creatorp(this_object())) AddStaminaPoints(-0.3);
	break;
    case "sub-tropical":
	if(!creatorp(this_object())) AddStaminaPoints(-0.2);
	break;
    case "sub-arctic":
	if(!creatorp(this_object())) AddStaminaPoints(-0.2);
	break;
    case "arctic":
	if(!creatorp(this_object())) AddStaminaPoints(-0.3);	  
	break;
    default:
	if(!creatorp(this_object())) AddStaminaPoints(-0.1);	  
	break;	    
    }
    if( prevclim != newclim && prevclim != "indoors" && newclim != "indoors" ){
	switch(prevclim) {
	case "arid":
	    if( newclim == "tropical" || newclim == "sub-tropical" )
		message("environment", "The air is much more humid.",
		  this_object());
	    else message("environment", "The air is getting a bit cooler.",
		  this_object());
	    break;
	case "tropical":
	    if( newclim != "arid" )
		message("environment", "The air is not quite as humid.",
		  this_object());
	    else message("environment", "The air has become suddenly dry.",
		  this_object());
	    break;
	case "sub-tropical":
	    if( newclim == "arid" )
		message("environment", "The air has become suddenly dry.",
		  this_object());
	    else if( newclim == "tropical" )
		message("environment","The air has gotten a bit more humid.",
		  this_object());
	    else message("environment", "The air is not quite as humid.",
		  this_object());
	    break;
	case "temperate":
	    if( newclim == "arid" )
		message("environment", "The air is a bit drier and warmer.",
		  this_object());
	    else if( newclim == "tropical" )
		message("environment", "The air is much more humid.",
		  this_object());
	    else if( newclim == "sub-tropical" )
		message("environment", "The air is a bit more humid.",
		  this_object());
	    else message("environment", "The air is a bit colder now.",
		  this_object());
	    break;
	case "sub-arctic":
	    if( newclim == "arid" || newclim == "tropical" ||
	      newclim == "sub-tropical" )
		message("environment", "It has suddenly grown very hot.",
		  this_object());
	    else if( newclim == "arctic" )
		message("environment", "It is a bit cooler than before.",
		  this_object());
	    else message("environment", "It is not quite as cold as "
		  "before.", this_object());
	    break;
	case "arctic":
	    if( newclim == "sub-arctic" )
		message("environment", "It is not quite as cold now.",
		  this_object());
	    else message("environment", "It is suddenly much warmer than "
		  "before.", this_object());
	}
    }
    eventMoveFollowers(environment(this_object()));
    return 1;
}


int eventReceiveObject(object foo) {
    object ob;

    ob = previous_object();
    if( !ob || !interactive::eventReceiveObject() ) return 0;
    AddCarriedMass((int)ob->GetMass());
    return 1;
}

int eventReleaseObject(object foo) {
    object ob;

    ob = previous_object();
    if( !ob || !interactive::eventReleaseObject() ) return 0;
    if( ob->GetMass() )
	AddCarriedMass( -((int)ob->GetMass()) );
    return 1;
}

void eventLoadObject(mixed *value, int recurse) { }

static mixed eventUse(object used, string cmd) {
    object old_agent;
    mixed tmp;
    string mess = "";

    mess += "------\n";
    mess += timestamp()+":\n";
    mess += "/lib/player.c: eventUse() hit.\n";
    mess += "stack: "+get_stack()+"\n";
    mess += "previous: "+identify(previous_object(-1))+"\n"; 
    mess += "------\n";
    log_file("player_errors",mess);
    return 0;
    old_agent = this_agent(used);
    tmp = parse_sentence(cmd);
    this_agent(old_agent);
    message("info", tmp, this_object());
    if( stringp(tmp) ) message("error", tmp, this_object());
    else return tmp;
}

/* *****************  /lib/player.c modal functions  ***************** */

int CanReceive(object ob) { return CanCarry((int)ob->GetMass()); }

mixed CanUse() { return 1; }

/* *****************  /lib/player.c local functions  ***************** */

int Setup() {
    string classes;
    if( !interactive::Setup() ) return 0;
    if( !GetClass() ) SetClass("explorer");
    if( GetClass() ) {
	foreach(classes in (string array)CLASSES_D->GetClasses())
	if( ClassMember(classes) && classes != GetClass() )
	    AddChannel(classes);
    }
    if(sizeof(GetExtraChannels())) AddChannel(GetExtraChannels());
    set_heart_beat(GetHeartRate());

    if(GetProperty("brand_spanking_new")){
	object jeans, shirt, book;

	if(ENGLISH_ONLY) this_object()->SetNativeLanguage("English");
	PLAYERS_D->AddPlayerInfo(this_object());

	foreach(classes in (string array)CLASSES_D->GetClasses())
	if( ClassMember(classes) && classes != GetClass() )
	    AddChannel(classes);
	if( avatarp() ) AddChannel(({ "avatar" }));
	if( high_mortalp() ) AddChannel( ({ "newbie", "hm" }) );
	if( newbiep() ) AddChannel( ({ "newbie" }) );
	else {
	    RemoveChannel( ({ "newbie" }) );
	}
	AddChannel( ({ "gossip" }) );
	if( councilp() ) AddChannel( ({ "council" }) );
	AddChannel(GetClass());

	jeans = new("/domains/default/armor/jeans");
	shirt = new("/domains/default/armor/shirt");
	book = new("/domains/default/obj/handbook");

	if(jeans) jeans->eventMove(this_object());
	if(shirt) shirt->eventMove(this_object());
	if(book && !present("handbook",this_object()))  book->eventMove(this_object());
	else if(book) book->eventMove(ROOM_FURNACE);

	if(jeans) this_object()->eventForce("wear jeans");
	if(shirt) this_object()->eventForce("wear shirt");
	SetProperty("brand_spanking_new",0);
    }

    if(this_object()->GetTown() == "FirstAdmin"){
	object robe, hat, staff, book, book2;
	string home;

	this_object()->SetTown("World");

	robe = new("/domains/default/armor/robe");
	hat = new("/domains/default/armor/wizard_hat");
	staff = new("/secure/obj/staff");
	book = new("/domains/default/obj/guide");
	book2 = new("/domains/default/obj/handbook");

	if(robe) robe->eventMove(this_object());
	if(hat) hat->eventMove(this_object());
	if(staff) staff->eventMove(this_object());
	if(book) book->eventMove(this_object());
	if(book2) book2->eventMove(this_object());

	if(robe) this_object()->eventForce("wear robe");
	if(hat) this_object()->eventForce("wear hat");

	home = "/realms/"+this_player()->GetKeyName()+"/workroom";

	if(file_exists(home+".c")) 
	    this_object()->eventMoveLiving(home);

	this_object()->AddChannel( ({"admin", "error", "cre", "newbie", "gossip", "ds", "ds_test", "lpuni", "death", "connections","intercre","dchat"}) );

	SetShort("First Admin $N");
    }

    return 1;
}

/* ***************** /lib/player.c data functions  ***************** */

int AddCurrency(string type, int amount) {
    if( currency_value(amount, type) > 999 )
	log_file("currency", GetCapName() + " received "+amount+" "+type+
	  " "+ctime(time())+"\n"+identify(previous_object(-1))+"\n");
    return living::AddCurrency(type, amount);
}

int AddBank(string bank, string type, int amount) {
    if( currency_value(amount, type) > 999 )
	log_file("bank", GetCapName() + " deposited "+amount+" "+type+
	  " "+ctime(time())+" into bank: "+bank+"\n" +
	  identify(previous_object(-1))+"\n");
    return living::AddBank(bank, type, amount);
}

string *GetMuffed(){
    return Muffed;
}

string *SetMuffed(string *muffed){
    if(sizeof(muffed)) Muffed = muffed;
    else Muffed = ({});
    return Muffed;
}

string *AddMuffed(string muffed){
    string tmpstr;
    if(!muffed || muffed == "" || !sizeof(muffed)) return Muffed;
    if(grepp(muffed,"@")) {
	tmpstr = INTERMUD_D->GetMudName(muffed[1..sizeof(muffed)-1]);
    }
    if(sizeof(tmpstr)) muffed = tmpstr;
    muffed = lower_case(muffed);
    if(member_array(muffed,Muffed) == -1) Muffed += ({ muffed });
    return Muffed;
}

string *RemoveMuffed(string unmuffed){
    string tmpstr;
    if(!sizeof(unmuffed)) return Muffed;
    if(grepp(unmuffed,"@")) {
	tmpstr = INTERMUD_D->GetMudName(unmuffed[1..sizeof(unmuffed)-1]);
    }
    if(sizeof(tmpstr)) unmuffed = tmpstr;
    unmuffed = lower_case(unmuffed);
    if(member_array(unmuffed,Muffed) != -1) Muffed -= ({ unmuffed });
    return Muffed;
}

string *SetTitles(string *titles) {
    if( sizeof(distinct_array(titles)) != sizeof(titles) ) return Titles;
    Titles = titles;
    SetShort("whatever");
}

string *AddTitle(string title) {
    if( !stringp(title) ) return Titles;
    else if( member_array(title, Titles) != -1 ) return Titles;
    else {
	Titles = ({ title }) + Titles;
	SetShort("whatever");
	return Titles;
    }
}

string *RemoveTitle(string title) {
    if( !stringp(title) ) return Titles;
    if( member_array(title, Titles) == -1 ) return Titles;
    else {
	Titles -= ({ title });
	SetShort("whatever");
	return Titles;
    }
}

string *GetTitles() { return Titles; }
int SetTitleLength(int x) {
    if( x > 2 ) x = 2;
    return (TitleLength = x);
}

int GetTitleLength() { return TitleLength; }

void AddQuest(string title, string desc) {
    class quest tmp;
    object * PartyMember;
    object ob;
    string pname;

    if( !title || !desc ) return;

    if( (string)GetParty() ) {
	pname = (string)GetParty();
	PartyMember = "/daemon/party"->GetPartyMembers(pname) - ({ this_player() });;
	foreach(ob in PartyMember) {
	    ob->AddPartyQuest(title,desc);
	}
    }
    tmp = new(class quest);
    tmp->Date = time();
    tmp->Description = desc;
    Quests += ({ tmp });
    AddTitle(title);
}

void AddQuestSkillPoints(string skill, int amount) {
    object * PartyMember;
    object ob;
    string pname;

    if(!skill || !amount) return;

    if( (string)GetParty() ) {
	pname = (string)GetParty();
	PartyMember = "/daemon/party"->GetPartyMembers(pname);
	foreach(ob in PartyMember) {
	    ob->AddSkillPoints(skill, amount);
	}
    }
    else AddSkillPoints(skill, amount);
}


void AddQuestStatPoints(string stat, int amount) {
    object * PartyMember;
    object ob;
    string pname;

    if(!stat || !amount) return;


    if( (string)GetParty() ) {
	pname = (string)GetParty();
	PartyMember = "/daemon/party"->GetPartyMembers(pname);
	foreach(ob in PartyMember) {
	    ob->AddStatPoints(stat, amount);
	}
    }
    else AddStatPoints(stat, amount);
}


void AddQuestCurrency(string type, int amount) {
    object * PartyMember;
    object ob;
    string pname;

    if(!type || !amount) return;

    if( (string)GetParty() ) {
	pname = (string)GetParty();
	PartyMember = "/daemon/party"->GetPartyMembers(pname);
	foreach(ob in PartyMember) {
	    ob->AddCurrency(type, amount);
	}
    }
    else AddCurrency(type, amount);
}

void AddPartyQuest(string title, string desc) {
    class quest tmp;

    if( !title || !desc ) return;
    if( member_array(title, Titles) != -1 ) return;
    tmp = new(class quest);
    tmp->Date = time();
    tmp->Description = desc;
    Quests += ({ tmp });
    AddTitle(title);
}

mixed *GetQuests() {
    return map(Quests, (: ({ ((class quest)$1)->Date,
	  ((class quest)$1)->Description }) :));
}

int GetQuest(string str){
    foreach(mixed component in GetQuests()){
	if(str == component[1]) return 1;
    }
    return 0;
}

string SetShort(string irrelevant) {
    string title, tmp;
    int i;

    if( high_mortalp() ) title = "High mortal ";
    else title = "";
    if(title && title != "") title += " $N ";
    else title = "$N ";
    if( GetUndead() && (tmp = GetUndeadType()) )
	return interactive::SetShort(title + "the " + tmp);
    if( avatarp() || creatorp() ) {
	return interactive::SetShort(irrelevant);
    }
    if( !(i = sizeof(Titles)) )
	return interactive::SetShort(title + "the unaccomplished");
    else title += Titles[0];
    if( i > 1 && TitleLength > 1 ) title += " and " + Titles[1];
    return interactive::SetShort(title);
}

int SetUndead(int x) {
    x = living::SetUndead(x);
    SetShort("nonsense");
    return x;
}

string GetName() {
    if(GetInvis()) return "A shadow";
    else return interactive::GetName();
}

varargs string GetLong(string str) {
    mapping counts;
    string item;

    str = GetShort() + "\n";
    str += interactive::GetLong() + "\n";
    str += living::GetLong(nominative(this_object()));
    foreach(item in map(all_inventory(),
	(: (string)$1->GetAffectLong(this_object()) :)))
    if( item ) str += item + "\n";
    if(this_object()->GetAffectLong()) str += this_object()->GetAffectLong();
    counts = ([]);
    foreach(item in map(
	filter(all_inventory(), (: !((int)$1->GetInvis(this_object())) :)),
	(: (string)$1->GetEquippedShort() :)))
    if( item ) counts[item]++;
    if( sizeof(counts) ) str += GetCapName() + " is carrying:\n";
    foreach(item in keys(counts))
    str += capitalize(consolidate(counts[item], item)) + "\n";
    return str;
}

string GetCapName() { return interactive::GetCapName(); }

int ResetLevel() {
    int x, y;

    x = GetLevel();
    if( x != (y = living::ResetLevel()) ) {
	string file;

	if( x > y ) file = "decline";
	else file = "advance";
	log_file(file, GetCapName() + " went from level " + x + " to "
	  "level " + y + " (" + ctime(time()) + ")\n");
	if( x < y ) {
	    eventPrint("%^YELLOW%^You are now a more experienced " + 
	      GetClass() + ".");
	    TrainingPoints += ( (y-x) * 4 );
	}
	else TrainingPoints -= ( (x-y) * 4 );
	SetShort("whatever");
	if( x > 49 && y < 50 ) RemoveChannel("avatar");
	if( x > 24 && y < 25 ) RemoveChannel("hm");
    }
    return y;
}

string SetClass(string str) {
    if( GetClass() != living::SetClass(str) ) {
	int points = TrainingPoints;
	string classes;

	ResetLevel();
	TrainingPoints = points;   /* leave points alone */
	AddChannel(GetClass());
	foreach(classes in (string array)CLASSES_D->GetClasses())
	if( ClassMember(classes) && classes != GetClass() )
	    AddChannel(classes);
    }
    return GetClass();
}

varargs mixed GetEffectiveVision(mixed location, int raw_score) {
    if( newbiep(this_object()) ) return VISION_CLEAR;
    else if(raw_score && location) return living::GetEffectiveVision(location,raw_score);
    else if(location) return living::GetEffectiveVision(location);
    else return living::GetEffectiveVision();
}

varargs static int AddHealthPoints(int x, string limb, object agent) {
    int hp, ret, undead;

    hp = GetHealthPoints();
    undead = GetUndead();
    ret = living::AddHealthPoints(x, limb, agent);
    if( hp <= (hp = GetHealthPoints()) || undead != GetUndead() ) return ret;
    eventDisplayStatus();
    return ret;
}

int GetLanguageLevel(string lang) {
    if( newbiep() ) return 100;
    else return living::GetLanguageLevel(lang);
}

mapping *GetDeaths() {
    if( !Deaths ) return ({});
    return copy(Deaths);
}

int AddTrainingPoints(int x) {
    log_file("TrainingPoints", GetName() + " received " + x + " training "
      "points at " + ctime(time()) + "\ncall chain: " +
      sprintf("%O\n", previous_object(-1)) );
    return (TrainingPoints += x);
}

int RemoveTrainingPoints(int x) {
    return (TrainingPoints -= x);
}

int GetTrainingPoints() { return TrainingPoints; }

varargs int eventTrain(string skill, int points) {
    float x = 0;
    mapping mp;

    if( points < 1 ) points = 1;
    if( !(mp = GetSkill(skill)) ) return 0;
    if( TrainingPoints < points ) return 0;
    while( points-- ) {
	int max = GetMaxSkillPoints(skill, mp["level"]);
	switch( mp["class"] ) {
	case 1: x = 50.0; break;
	case 2: x = 40.0; break;
	case 3: x = 30.0; break;
	case 4: x = 20.0; break;
	default: return 0;
	}
	TrainingPoints--;
	AddSkillPoints(skill, to_int( (max * x) / 100 ));
    }
    return 1;
}
