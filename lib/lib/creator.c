/*    /lib/creator.c
 *    from the Dead Souls LPC Library
 *    a very simple creator object
 *    created by Descartes of Borg 950321
 *    Version: @(#) creator.c 1.14@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <daemons.h>
#include <vision.h>
#include <position.h>
#include <message_class.h>
#include "include/creator.h"

inherit LIB_PLAYER;

#ifdef __PACKAGE_DATABASE_DB__
inherit LIB_ISQL;
#endif /* __PACKAGE_DATABASE_DB__ */

private int showgrid, wizvision, CreatorAge, CreatorBirth;
private static int LastCreatorAge;
private string LivingShort;

/* *****************  /lib/creator.c driver applies  ***************** */

static void create(){
    player::create();
    CreatorAge = 0;
    LastCreatorAge = time();
    CreatorBirth = time();
}

int is_living(){ return 1; }

int inventory_accessible(){ return 1; }

int inventory_visible(){ return 1; }

mixed direct_verb_rule(string verb){
    return SOUL_D->CanTarget(this_player(), verb, this_object());
}

mixed direct_give_liv_obj(){
    if( this_player() == this_object() ) return "Are you confused?";
    return 1;
}

mixed direct_give_liv_obs(){
    return direct_give_liv_obj();
}

mixed indirect_give_obj_to_liv(object item){
    if( !item ) return 0;
    if( this_player() == this_object() ) return "Are you confused?";
    if( environment(item) != this_player() ) return "You don't have that!";
    return CanCarry(item->GetMass());
}

mixed indirect_give_obs_to_liv(object *item){
    return 1;
}

mixed direct_look_obj(){ return 1; }

mixed direct_look_at_obj(){ return 1; }

mixed direct_marry_liv_to_liv(){
    return 1;
}

mixed indirect_marry_liv_to_liv(){
    return 1;
}

static void net_dead(){
    player::net_dead();
    CreatorAge += time() - LastCreatorAge;
    LastCreatorAge = time();}

    void eventReconnect(){
        string tmp;

        player::eventReconnect();
        LastCreatorAge = time();
        if( file_exists(tmp = user_path(GetKeyName()) + "dead.edit") )
            message("system", "\nYour edit file was saved as: "+tmp, this_object());
    }

varargs int eventShow(object who, string str, string on_id){
    player::eventShow(who, str);
    return 1;
}

/* *****************  /lib/creator.c events  ***************** */

void eventDescribeEnvironment(int verbose){
    object env;

    if( !(env = environment()) ){
        message("room_description", "No environment.", this_object());
        return;
    }
    message("system", file_name(env), this_object());
    if(this_object()->GetVisibleGrid() && environment(this_object())){
        string grid = ROOMS_D->GetCoordinates(environment(this_object()));
        if(sizeof(grid) > 2){
            grid = "Global coordinates: "+grid;
        }
        else {
            grid = "Global coordinates unavailable.";
        }
        message("system", grid, this_object());
    }
    player::eventDescribeEnvironment(verbose);
}

static int Destruct(){
    int x;

    if( !(x = player::Destruct()) ) return 0;
#ifdef __PACKAGE_DATABASE_DB__
    isql::Destruct();
#endif /* __PACKAGE_DATABASE_DB__ */
    return x;
}

/* *****************  /lib/creator.c  local functions ***************** */

int Setup(){
    string tmp;
    int bugs, laston;

    laston = GetLoginTime();
    if( !player::Setup() ) return 0;
    if( archp() ) AddChannel( ({ "admin", "error" }) );
    AddSearchPath( ({ DIR_CREATOR_CMDS, DIR_SECURE_CREATOR_CMDS }) );
    if( file_size(tmp = user_path(GetKeyName()) + "cmds") == -2 )
        AddSearchPath( ({ tmp }) );
    if( archp() ) AddSearchPath( ({ DIR_ADMIN_CMDS, DIR_SECURE_ADMIN_CMDS }) );
    if( bugs = BUGS_D->GetAssignedBugs(GetKeyName()) )
        message("system", "\n        >>>  You have " +
                consolidate(bugs, "an incomplete bug") +
                " assigned to you!!!!  <<<\n", this_object());
    NOTIFY_D->eventPrintNotices(this_object(), laston);

    /* Check for new customdefs location */
    if(!file_exists(tmp = REALMS_DIRS+"/"+GetKeyName()+"/area/customdefs.h")){
        string tmp2 = REALMS_DIRS+"/"+GetKeyName()+"/customdefs.h";
        if(file_exists(tmp2) && directory_exists(REALMS_DIRS+"/"+GetKeyName()))
            cp(tmp2, tmp);
    }

    return 1;
}

int eventForce(string cmd){
    if( !(master()->valid_apply( ({ GetKeyName() }) )) ) return 0;
    else return player::eventForce(cmd);
}

/*  ***************  /lib/creator.c modal functions  *************** */

int CanCarry(int amount){ return 1; }

/* ************  /lib/creator.c data manipulation functions  ************ */

int GetCreatorAge(){
    int x;

    if( !interactive(this_object()) ) return CreatorAge;
    x = time() - LastCreatorAge;
    CreatorAge += x;
    LastCreatorAge = time();
    return CreatorAge;
}

varargs string GetLong(string str){
    str = player::GetLong() + "\n";
    return str;
}

int GetCreatorBirth(){ return CreatorBirth; }

string GetName(){ 
    if( !GetInvis() || previous_object()->GetWizVision() ) return ::GetName();
    else return "A shadow";
}

mapping GetSpellBook(){
    mapping ret = ::GetSpellBook();
    foreach(string spell in keys(SPELLS_D->GetSpells())){
        ret[spell] = 100;
    }
    foreach(string spell in keys(SPELLS_D->GetPrayers())){
        ret[spell] = 100;
    }
    return ret;
}

varargs mixed CanCast(mixed spell...){
    return 1;
}

varargs mixed GetEffectiveVision(mixed location, int raw_score){
    if(wizvision) return VISION_CLEAR;
    return player::GetEffectiveVision(location, raw_score);
}

int SetWizVision(int i){
    if(!this_player()) return 0;
    if(archp(this_object()) && !archp(this_player())) return 0;
    if(!archp(this_object()) && this_player() != this_object()) return 0;
    if(i) wizvision = 1;
    else wizvision = 0;
    return wizvision;
}

int GetWizVision(){
    return wizvision;
}

int SetVisibleGrid(int i){
    if(!this_player()) return 0;
    if(archp(this_object()) && !archp(this_player())) return 0;
    if(!archp(this_object()) && this_player() != this_object()) return 0;
    if(i) showgrid = 1;
    else showgrid = 0;
    return showgrid;
}

int GetVisibleGrid(){
    return showgrid;
}

string GetLivingShort(){
    return LivingShort;
}

int eventDie(mixed agent){
    string tmpshort = this_object()->GetShort();
    if(this_object()->GetGodMode()) return 0;
    if(!grepp(tmpshort,"the ghost") && !grepp(tmpshort,"the reborn")){
        LivingShort = replace_string(tmpshort,this_object()->GetName(),
                "$N");
    }
    return ::eventDie(agent);
}

int eventReleaseObject(object foo2){
    int ret = ::eventReleaseObject(foo2);
    if(ret && GetParanoia("inventory_monitoring"))
        eventPrint("%^YELLOW%^NOTICE:%^RESET%^ "+identify(foo2)+
                " leaves your inventory.");
    flush_messages(this_object());
    return ret;
}

int eventMove(mixed dest){
    if(GetParanoia("move_monitoring")){
        eventPrint("%^RED%^B_BLACK%^You are about to move to: "+
                identify(dest)+"!%^RESET%^\nstack:\n"+get_stack(1));
        flush_messages(this_object());
    }
    return ::eventMove(dest);
}
