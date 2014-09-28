/*    /lib/player.c
 *    from the Dead Souls LPC Library
 *    the player object
 *    created by Descartes of Borg 950321
 *    Version: @(#) player.c 1.26@(#)
 *    Last Modified: 97/01/03
 */

#include <lib.h>
#include ROOMS_H
#include <damage_types.h>
#include <message_class.h>
#include <daemons.h>
#include <vision.h>
#include <position.h>
#include "include/player.h"

#undef LIB_PLAYER_SKILL_LOSS
#define PERCENT_XP 0.20
#define PERCENT_SP 0.70
#define PERCENT_MP 0.95
#define PERCENT_HP 0.70

inherit LIB_INTERACTIVE;
inherit LIB_LIVING;

private string *Titles;
string *Muffed = ({});
private mapping *Deaths;
private int TrainingPoints, TitleLength;
private static int heartcount;
private mixed telopt_ip;

/* *****************  /lib/player.c driver applies  ***************** */

static void create(){
    interactive::create();
    living::create();

    Titles = ({});
    TitleLength = 1;
}

static void heart_beat(){
    mixed heartping;
    int idle;

    if(!interactive(this_object())) return;

    heartping = GetProperty("keepalive");
    idle = query_idle(this_object());
    heartcount++;

    if(GetProperty("reply")){
        int t = GetProperty("reply_time");
        if(t > 1 && (time() - t) > 90000){
            RemoveProperty("reply");
            RemoveProperty("reply_time");
        }
    }
    interactive::heart_beat();
    living::heart_beat();

    if( IDLE_TIMEOUT && idle >= IDLE_TIMEOUT 
            && !builderp(this_object()) 
            && !present("testchar badge",this_object()) 
            && !present("idler_amulet",this_object()) 
            && !testp(this_object()) ){
        cmdQuit();
        return;
    }
#ifdef __DSLIB__
    if(heartping){
        send_nullbyte(this_object());
    }
#endif
}

static void net_dead(){
    interactive::net_dead();
    set_heart_beat(0);
}

static int Destruct(){
    CHARACTER_D->eventSaveTime();
    living::Destruct();
    return interactive::Destruct();
}

mixed eventAsk(object who, string what){
    if( what != "convert me" ) return 0;
    if( !GetReligion(1) ) return 0;
    who->SetProperty("converting", GetReligion(1));
    return 1;
}

void eventKillEnemy(object ob){
    living::eventKillEnemy(ob);
    STATISTICS_D->eventKill(ob);
}

void eventReconnect(){
    interactive::eventReconnect();
    living::eventReconnect();
    set_heart_beat(GetHeartRate());
}

/************** player.c command functions *************/
varargs int eventShow(object who, string str){
    if( !living::eventShow(who, str) ) return 0;
    if( this_player() != this_object() )
        eventPrint(this_player()->GetName() + " looks you over.");
    return 1;
}

/* *****************  /lib/player.c events  *************** */

varargs mixed eventDisplayStatus(int simple){
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
        str += "  %^BLUE%^mp: %^RED%^" + mp + "%^RESET%^/" + max_mp;
    else str += "  %^BLUE%^mp: %^RESET%^" + mp + "/" + max_mp;
    if( percent(sp, max_sp) < 20.0 )
        str += "  %^GREEN%^sp: %^RED%^" + sp + "%^RESET%^/" + max_sp;
    else str += "  %^GREEN%^sp: %^RESET%^" + sp + "/" + max_sp;
    if(!simple){
        str += "  %^MAGENTA%^xp: %^RESET%^" + xp;
        str += "  %^CYAN%^qp: %^RESET%^" + qp;
    }
    if(!simple){
        message("status", str, this_object());
        return 1;
    }
    return str;
}

static void eventDestroyUndead(object agent){
}

varargs int eventDie(mixed agent){
    int x;
    string agentname;

    this_object()->SetDying(0);

    if(!agent) agent = previous_object();
    if(!agent) agent = this_object();
    if(stringp(agent)) agentname = agent;
    else agentname = agent->GetName();

    if( (x = living::eventDie(agent)) != 1 ) return x;

    if(!Deaths || !sizeof(Deaths)) 
        Deaths = ({([ "date" : ctime(time()), "enemy" : agentname ])});
    else Deaths += ({ ([ "date" : ctime(time()), "enemy" : agentname ]) });

    if( !GetUndead() ){
        eventDestroyUndead(agent);
    }
    else  {
        message("my_action", "Consciousness passes from you after one last "
                "gasp for air.", this_object());
        message("my_action", "You awake, but you find your body feels "
                "different, and the world about you is unfamiliar.",
                this_object());
        if( agent ){
            message("other_action", GetName() + " is killed by "
                    + agentname + ".",
                    environment(this_object()), ({ agent, this_object() }));
            message("other_action", "You send " + GetName() + " into the "
                    "Underworld.", agent);
        }
        else message("other_action", GetName() + " dies.",
                environment(), ({ this_object() }) );

        NewBody(GetRace());

        eventCompleteHeal(GetMaxHealthPoints()/2);
        AddMagicPoints(-(random(GetMagicPoints())));
        this_object()->eventMove(ROOM_DEATH);
        this_object()->save_player(this_object()->GetKeyName());
        this_object()->eventDescribeEnvironment();
    }
    flush_messages();
    return 1;
}

mixed eventTurn(object who){
    if( !living::eventTurn(who) ){
        return 0;
    }
    eventDestroyUndead(who);
    return 1;
}

varargs void eventRevive(int nopenalty){
    string skill;

    this_object()->SetDead(0);
    if(!creatorp(this_object())) this_object()->SetInvis(0);
    this_object()->SetDeathEvents(0);

    if( !GetUndead() ) return;
    SetUndead(0);
    if(this_player()->GetPoison() > 0){
        this_player()->AddPoison(0 - this_player()->GetPoison());
    }
    if(!nopenalty && newbiep(this_object())) {
        nopenalty = 1;
        write("As a newbie you don't incur an experience penalty"
              " for this death.\n");
    }
    if(!nopenalty){
        int expee, subexpee;
#ifdef LIB_PLAYER_SKILL_LOSS
        foreach(skill in GetSkills()){
            int x;

            if( !random(4) ){
                continue;
            }
            if( newbiep(this_object()) ){
                x = 2;
            }
            else {
                x = 10;
            }
            x = random(x - (2*GetSkillClass(skill)))/2;
            if( x > 0 ){
                while( x-- ){
                    AddSkillPoints(skill,
                            -GetMaxSkillPoints(skill,
                                GetBaseSkillLevel(skill)));
                }
            }
        }
#endif
        expee = this_object()->GetExperiencePoints();
        subexpee = to_int(expee * PERCENT_XP);
        this_object()->AddExperienceDebt(subexpee);
    }
    NewBody(GetRace());
    eventCompleteHeal(GetMaxHealthPoints());
    SetSpecialTarget( ({}) );
    AddMagicPoints(-(GetMaxMagicPoints() * PERCENT_MP));
    AddStaminaPoints(-(GetMaxStaminaPoints() * PERCENT_SP));
    AddHealthPoints(-(GetMaxHealthPoints() * PERCENT_HP));
    if(this_object()->GetLead()){
        int shots = this_object()->GetLead("gunshot_wounds");
        if(shots) this_object()->AddLead("gunshot_wounds", -shots);
        shots = this_object()->GetLead("rifleshot_wounds");
        if(shots) this_object()->AddLead("rifleshot_wounds", -shots);
    }
    if(creatorp()){
        string livingtitle = this_object()->GetLivingShort();
        if(!livingtitle) livingtitle = "$N the reborn";
        interactive::SetShort(livingtitle);
    }
}

int eventMove(mixed dest){
    int ret;
    object env = environment();
    string location;

    if(!env) location = ROOM_START;
    else if(clonep(env)) location = file_name(env);
    else location = base_name(env);

    if(location) this_object()->SetProperty("LastLocation", location);

    ret = interactive::eventMove(dest);
    if( this_object() && environment(this_object())) eventMoveFollowers(environment(this_object()));
    return ret;
}

int eventReleaseObject(object foo){
    object ob;

    ob = previous_object();
    if( !ob || !interactive::eventReleaseObject() ) return 0;
    if( ob->GetMass() ){
        AddCarriedMass( -(ob->GetMass()) );
        if(environment()) environment()->AddCarriedMass(-(ob->GetMass()));
    }
    return 1;
}

void eventLoadObject(mixed *value, int recurse){ }

/* *****************  /lib/player.c modal functions  ***************** */

int CanReceive(object ob){ return CanCarry(ob->GetMass()); }

mixed CanUse(){ return 1; }

/* *****************  /lib/player.c local functions  ***************** */

int Setup(){
    string classes;
    string oldparties = PARTY_D->GetOldParties();
    if( !interactive::Setup() ) return 0;
    if( !GetClass() ) SetClass("explorer");
    RemoveExtraChannels(oldparties);
    foreach(string oldparty in oldparties){
        RemoveChannel(oldparty);
    }
    RemoveExtraChannels(oldparties);
    if( GetClass() ){
        foreach(classes in CLASSES_D->GetClasses())
            if( ClassMember(classes) && classes != GetClass() )
                AddChannel(classes);
    }
    if(sizeof(GetExtraChannels())) AddChannel(GetExtraChannels());
    set_heart_beat(GetHeartRate());
    if(builderp()){
        AddChannel( ({ "builder" }) );
        AddSearchPath( ({ DIR_BUILDER_CMDS, DIR_SECURE_BUILDER_CMDS }) );
    }
    PLAYERS_D->CheckBuilder(this_object());

    if(GetProperty("brand_spanking_new")){
        object jeans, shirt, book;
        if(!(this_object()->GetProperty("screen reader"))){
            this_object()->SetProperty("minimapping", 1);
        }
        else this_object()->SetProperty("minimapping", 0);
        if(ENGLISH_ONLY) this_object()->SetNativeLanguage("English");
        PLAYERS_D->AddPlayerInfo(this_object());

        foreach(classes in CLASSES_D->GetClasses())
            if( ClassMember(classes) && classes != GetClass() )
                AddChannel(classes);
        if( avatarp() ) AddChannel(({ "avatar" }));
        AddChannel( ({ "gossip", "newbie" }) );
        if( high_mortalp() ) AddChannel( ({ "hm" }) );
        else {
            //RemoveChannel( ({ "newbie" }) );
        }
        AddChannel( ({ "gossip" }) );
        if( councilp() ) AddChannel( ({ "council" }) );
        AddChannel(GetClass());

        jeans = new("/domains/default/armor/jeans");
        shirt = new("/domains/default/armor/shirt");
        book = new("/domains/default/obj/handbook");

        if(jeans) jeans->eventMove(this_object());
        if(shirt) shirt->eventMove(this_object());
        if(book && !present("handbook",this_object()))  
            book->eventMove(this_object());
        else if(book) book->eventMove(ROOM_FURNACE);

        if(jeans) this_object()->eventForce("wear jeans");
        if(shirt) this_object()->eventForce("wear shirt");
        SetProperty("brand_spanking_new",0);
    }

    if(this_object()->GetTown() == "FirstAdmin"){
        object robe, hat, staff, book, book2;
        string home;

        this_object()->SetTown("World");
        this_object()->SetProperty("minimapping", 0);
        if(!(this_object()->GetProperty("screen reader"))){
            this_object()->SetProperty("wizmapping", 1);
        }
        else this_object()->SetProperty("wizmapping", 0);
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
    if(!creatorp(this_object())) this_object()->SetInvis(0);
    RemoveProperty("reply");
    RemoveProperty("reply_time");
    return 1;
}

/* ***************** /lib/player.c data functions  ***************** */

    int AddCurrency(string type, int amount){
        if( currency_value(amount, type) > 999 )
            log_file("currency", GetCapName() + " received "+amount+" "+type+
                    " "+ctime(time())+"\n"+identify(previous_object(-1))+"\n");
        return living::AddCurrency(type, amount);
    }

    int AddBank(string bank, string type, int amount){
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
    if(muffed[0..0] == "@"){
        tmpstr = INTERMUD_D->GetMudName(muffed[1..]);
    }
    if(sizeof(tmpstr)) muffed = tmpstr;
    muffed = lower_case(muffed);
    if(member_array(muffed,Muffed) == -1) Muffed += ({ muffed });
    return Muffed;
}

string *RemoveMuffed(string unmuffed){
    string tmpstr;
    if(!sizeof(unmuffed)) return Muffed;
    if(grepp(unmuffed,"@")){
        tmpstr = INTERMUD_D->GetMudName(unmuffed[1..sizeof(unmuffed)-1]);
    }
    if(sizeof(tmpstr)) unmuffed = tmpstr;
    unmuffed = lower_case(unmuffed);
    if(member_array(unmuffed,Muffed) != -1) Muffed -= ({ unmuffed });
    return Muffed;
}

string *SetTitles(string *titles){
    if( sizeof(distinct_array(titles)) != sizeof(titles) ) return Titles;
    Titles = titles;
    SetShort("whatever");
}

string *AddTitle(mixed title){
    if(arrayp(title)){
        if(this_object()->GetGender() == "female") title = title[1];
        else title = title[0];
    }
    if( !stringp(title) ) return Titles;
    else if( member_array(title, Titles) != -1 ) return Titles;
    else {
        Titles = ({ title }) + Titles;
        SetShort("whatever");
        return Titles;
    }
}

string *RemoveTitle(mixed title){
    if( stringp(title) ) title = ({ title });
    foreach(string sub in title){
        Titles -= ({ sub });
        SetShort("whatever");
    }
    return Titles;
}

string *GetTitles(){ return Titles; }
int SetTitleLength(int x){
    if( x > 2 ) x = 2;
    return (TitleLength = x);
}

int GetTitleLength(){ return TitleLength; }

string SetShort(string irrelevant){
    string title, tmp;
    int i;

    if( high_mortalp() ) title = "High mortal ";
    else title = "";
    if(title && title != "") title += " $N ";
    else title = "$N ";
    if( GetUndead() && (tmp = GetUndeadType()) )
        return interactive::SetShort(title + "the " + tmp);
    if( avatarp() || creatorp() ){
        return interactive::SetShort(irrelevant);
    }
    if( !(i = sizeof(Titles)) )
        return interactive::SetShort(title + "the unaccomplished");
    else title += Titles[0];
    if( i > 1 && TitleLength > 1 ) title += " and " + Titles[1];
    return interactive::SetShort(title);
}

string GetPlainShort(){
    return interactive::GetShort();
}

int SetUndead(int x){
    x = living::SetUndead(x);
    SetShort("nonsense");
    return x;
}

string GetName(){
    if(GetInvis() && (!this_player() ||!this_player()->GetWizVision())){
        return "A shadow";
    }
    else return interactive::GetName();
}

varargs string GetLong(string str){
    mapping counts;
    string item;
    string *affects = ({});

    str = this_object()->GetShort() + "\n";
    str += interactive::GetLong() + "\n";
    str += living::GetLong(nominative(this_object()));
    foreach(item in map(all_inventory(),
                (: $1->GetAffectLong(this_object()) :))){
        if(item && member_array(item,affects) == -1) affects += ({ item });
    }
    if(sizeof(affects)) str += implode(affects,"\n")+"\n";
    if(this_object()->GetAffectLong()) str += this_object()->GetAffectLong();
    counts = ([]);
    foreach(item in map(
                filter(all_inventory(), (: !($1->GetInvis(this_object())) :)),
                (: $1->GetEquippedShort() :)))
        if( item ) counts[item]++;
    if( sizeof(counts) ) str += GetCapName() + " is carrying:\n";
    foreach(item in keys(counts))
        str += capitalize(consolidate(counts[item], item)) + "\n";
    return str;
}

string GetCapName(){ return interactive::GetCapName(); }

int ResetLevel(){
    int x, y;

    x = GetLevel();
    if( x != (y = living::ResetLevel()) ){
        string file;

        if( x > y ) file = "decline";
        else file = "advance";
        log_file(file, GetCapName() + " went from level " + x + " to "
                "level " + y + " (" + ctime(time()) + ")\n");
        if( x < y ){
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

string SetClass(string str){
    if( GetClass() != living::SetClass(str) ){
        int points = TrainingPoints;
        string classes;

        ResetLevel();
        TrainingPoints = points;   /* leave points alone */
        AddChannel(GetClass());
        foreach(classes in CLASSES_D->GetClasses())
            if( ClassMember(classes) && classes != GetClass() )
                AddChannel(classes);
    }
    return GetClass();
}

varargs mixed GetEffectiveVision(mixed location, int raw_score){
    if( newbiep(this_object()) ) return VISION_CLEAR;
    else if(raw_score && location) return living::GetEffectiveVision(location,raw_score);
    else if(location) return living::GetEffectiveVision(location);
    else return living::GetEffectiveVision();
}

varargs static int AddHealthPoints(int x, string limb, object agent){
    int hp, ret, undead;
    hp = GetHealthPoints();
    undead = GetUndead();
    ret = living::AddHealthPoints(x, limb, agent);
    if( hp <= (hp = GetHealthPoints()) || undead != GetUndead() ) return ret;
    eventDisplayStatus();
    return ret;
}

int GetLanguageLevel(string lang){
    if( newbiep() ) return 100;
    else return living::GetLanguageLevel(lang);
}

mapping *GetDeaths(){
    if( !Deaths ) return ({});
    return copy(Deaths);
}

int AddTrainingPoints(int x){
    log_file("TrainingPoints", GetName() + " received " + x + " training "
            "points at " + ctime(time()) + "\ncall chain: " +
            sprintf("%O\n", previous_object(-1)) );
    return (TrainingPoints += x);
}

int RemoveTrainingPoints(int x){
    return (TrainingPoints -= x);
}

int GetTrainingPoints(){ return TrainingPoints; }

varargs int eventTrain(string skill, int points){
    float x = 0;
    mapping mp;

    if( points < 1 ) points = 1;
    if( !(mp = GetSkill(skill)) ) return 0;
    if( TrainingPoints < points ) return 0;
    while( points-- ){
        int max = GetMaxSkillPoints(skill, mp["level"]);
        switch( mp["class"] ){
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

mixed GetTeloptIp(){
    if(previous_object() && base_name(previous_object()) != SEFUN &&
            !(archp(this_player())) && previous_object() != this_object()) return 0;
    return telopt_ip;
}

mixed SetTeloptIp(mixed str){
    if(base_name(previous_object()) != LIB_CONNECT &&
            !(archp(this_player()))) return 0;
    return telopt_ip = str;
}
