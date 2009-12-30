/*    /lib/sentient.c
 *    from the Dead Souls LPC Library
 *    a sentient npc
 *    created by Descartes of Borg 950331
 *    Version: @(#) sentient.c 1.5@(#)
 *    Last modified: 96/12/15
 */

#include <lib.h>
#include <function.h>
#include <talk_type.h>
#include <message_class.h>


inherit LIB_NPC;

private mapping     CommandResponses = ([]);
private mapping     EmoteResponses   = ([]);
private mapping     RequestResponses = ([]);
private mapping     TalkResponses    = ([]);
private mapping     ConsultResponses = ([]);
static private int  WanderCount      = 0;
static private int  WanderMarker     = 0;
private mixed array WanderPath       = ({});
private int         WanderRecurse    = 0;
private int         WanderSpeed      = 0;
private int         permit_load      = 0;
private int      SpellChance      = 50;

/* ******************  /lib/sentient.c data manipulation  **************** */
/**
 * This method is called to see if anything is going on requiring
 * the NPC to keep its heart beat going.
 * @return true if the heart beat should continue
 */
static int ContinueHeart(){
    if( WanderSpeed ){
        return 1;
    }
    else {
        return npc::ContinueHeart();
    }
}

mixed AddConsultResponse(string str, mixed val){
    return (ConsultResponses[str] = val);
}

mixed GetConsultResponse(string str){
    return ConsultResponses[str];
}

mapping GetConsultResponses(){
    return ConsultResponses;
}

int RemoveConsultResponse(string str){
    if( !ConsultResponses[str] ){
        return 0;
    }
    map_delete(ConsultResponses, str);
    return 1;
}

mapping SetConsultResponses(mapping mp){
    return (ConsultResponses = expand_keys(mp));
}

mixed AddCommandResponse(string str, mixed val){
    return (CommandResponses[str] = val);
}

mixed GetCommandResponse(string str){
    return CommandResponses[str];
}

mapping GetCommandResponses(){
    return CommandResponses;
}

int RemoveCommandResponse(string str){
    if( !CommandResponses[str] ){
        return 0;
    }
    map_delete(CommandResponses, str);
    return 1;
}

mapping SetCommandResponses(mapping mp){
    return (CommandResponses = expand_keys(mp));
}

mapping AddCommandResponses(mapping mp){
    mapping ret = add_maps(CommandResponses, mp, -1);
    CommandResponses = ret;
    return ret;
}

mixed AddEmoteResponse(string verb, mixed val){
    return (EmoteResponses[verb] = val);
}

mixed GetEmoteResponse(string verb){
    return EmoteResponses[verb];
}

mapping GetEmoteResponses(){
    return EmoteResponses;
}

int RemoveEmoteResponse(string verb){
    if( !EmoteResponses[verb] ){
        return 0;
    }
    map_delete(EmoteResponses, verb);
    return 1;
}

mapping SetEmoteResponses(mapping mp){
    return (EmoteResponses = expand_keys(mp));
}

mixed AddRequestResponse(string str, mixed val){
    return (RequestResponses[str] = val);
}

mixed GetRequestResponse(string str){
    return RequestResponses[str];
}

mapping GetRequestResponses(){
    return RequestResponses;
}

int RemoveRequestResponse(string str){
    if( !RequestResponses[str] ){
        return 0;
    }
    map_delete(RequestResponses, str);
    return 1;
}

mapping SetRequestResponses(mapping mp){
    return (RequestResponses = expand_keys(mp));
}

mapping AddRequestResponses(mapping mp){
    mapping ret = add_maps(RequestResponses, mp, -1);
    RequestResponses = ret;
    return RequestResponses;
}

mixed AddTalkResponse(string str, mixed val){
    return (TalkResponses[str] = val);
}

int RemoveTalkResponse(string str){
    if( !TalkResponses[str] ){
        return 0;
    }
    map_delete(TalkResponses, str);
    return 1;
}

mixed GetTalkResponse(string str){
    return TalkResponses[str];
}

mapping GetTalkResponses(){
    return TalkResponses;
}

mapping SetTalkResponses(mapping mp){
    return TalkResponses = expand_keys(mp);
}

int SetSpellChance(int i){
    SpellChance = i;
    return SpellChance;
}

int GetSpellChance(){
    return SpellChance;
}

varargs int SetWander(int speed, mixed *path, int recurse){
    WanderSpeed = speed;
    if( path ){
        WanderPath = path;
    }
    else {
        WanderPath = ({});
    }
    WanderRecurse = recurse;
}

mixed array GetWanderPath(){
    return WanderPath;
}

mixed array SetWanderPath(mixed array path){
    return (WanderPath = path);
}

int GetWanderRecurse(){
    return WanderRecurse;
}

int SetWanderRecurse(int x){
    return (WanderRecurse = x);
}

int GetWanderSpeed(){
    return WanderSpeed;
}

int SetWanderSpeed(int x){
    return (WanderSpeed = x);
}

int SetPermitLoad(mixed i){
    permit_load = i;
    return permit_load;
}

int GetPermitLoad(){
    return permit_load;
}

/* ******************    /lib/sentient.c events      **************** */
mixed eventAsk(object who, string str){
    string cmd, args, lang;
    int prof;

    if( !str || str == "" ) return 0;
    lang = who->GetDefaultLanguage();
    prof = who->GetLanguageLevel(lang);
    str = translate(str, prof);
    prof = this_object()->GetLanguageLevel(lang);
    str = translate(str, prof);
    if(!prof){
        eventForce("speak I don't even know what language that is.");
        return 1;
    }
    if(prof < 100){
        eventForce("speak I don't really understand "+capitalize(lang)+".");
        return 1;
    }
    if( sscanf(str, "%s %s", cmd, args) != 2 ){
        cmd = str;
        args = 0;
    }
    if( !CommandResponses[cmd] ){
        if( !CommandResponses["default"] ) return 0;
        else if( stringp(CommandResponses["default"]) ){
            eventForce("speak " + CommandResponses["default"]);
            return 1;
        }
        else return evaluate(CommandResponses["default"], who, cmd, args);
    }
    if( stringp(CommandResponses[cmd]) ){
        eventForce("speak " + CommandResponses[cmd]);
        return 1;
    }
    return evaluate(CommandResponses[cmd], who, cmd, args);
}

varargs mixed eventReceiveEmote(object who, string verb, string info){
    mixed val = EmoteResponses[verb];
    if( !val ){
        return 0;
    }
    if( stringp(val) ){
        eventSpeak(who,TALK_LOCAL,val);
        return 1;
    }
    return evaluate(val, this_player(), verb, info);
}

mixed eventConsult(object who, string str){
    string lang;
    int prof;
    if( !str || str == "" || !ConsultResponses) return 0;
    lang = who->GetDefaultLanguage();
    prof = who->GetLanguageLevel(lang);
    str = translate(str, prof);
    prof = this_object()->GetLanguageLevel(lang);
    str = translate(str, prof);

    if(!prof){
        eventForce("speak I don't even know what language that is.");
        return 1;
    }
    if(prof < 100){
        eventForce("speak I don't really understand "+capitalize(lang)+".");
        return 1;
    }

    if( !str || str == "" || !ConsultResponses) return 0;
    str = remove_article(str);
    if( !ConsultResponses[str] ){
        if( !ConsultResponses["default"] ) return 0;
        else if( stringp(ConsultResponses["default"]) ){
            eventForce("speak " + ConsultResponses["default"]);
            return 1;
        }
        else return evaluate(ConsultResponses["default"], who, str);
    }
    if( stringp(ConsultResponses[str]) ){
        eventForce("speak " + ConsultResponses[str]);
        return 1;
    }
    return evaluate(ConsultResponses[str], who, str);
}

mixed eventRequest(object who, string str){
    string lang;
    int prof;

    if( !str || str == "" ) return 0;
    lang = (who->GetDefaultLanguage() || "that language");
    prof = who->GetLanguageLevel(lang);
    str = translate(str, prof);
    prof = this_object()->GetLanguageLevel(lang);
    str = translate(str, prof);

    if(!prof){
        eventForce("speak I don't even know what language that is.");
        return 1;
    }
    if(prof < 100){
        eventForce("speak I don't really understand "+capitalize(lang)+".");
        return 1;
    }

    if( !RequestResponses[str] ){
        if( !RequestResponses["default"] ) return 0;
        else if( stringp(RequestResponses["default"]) ){
            eventForce("speak " + RequestResponses["default"]);
            return 1;
        }
        else return evaluate(RequestResponses["default"], who, str);
    }
    if( stringp(RequestResponses[str]) ){
        eventForce("speak " + RequestResponses[str]);
        return 1;
    }
    return evaluate(RequestResponses[str], who, str);
}

mixed eventTalkRespond(object who, object targ, int cls, string msg, string lang){
    string resp;

    foreach(resp in keys(TalkResponses)){
        if( resp == "default" ) continue;
        if( strsrch(lower_case(msg), resp) > -1 ){
            if( stringp(TalkResponses[resp]) ){
                eventForce("speak " + TalkResponses[resp]);
                return 1;
            }
            else if( evaluate(TalkResponses[resp], who, targ, msg, lang, cls) )
                return 1;
        }
    }
    if( TalkResponses["default"] ){
        if( stringp(TalkResponses["default"]) )
            eventForce("speak " + TalkResponses["default"]);
        else evaluate(TalkResponses["default"], who, targ, msg, lang, cls);
        return 1;
    }
    return 0;
}

mixed eventWander(){
    int fp;
    object env = environment();
    mixed outs;

    if(!env || !(outs = env->GetExits())) return 0;

    if( !sizeof(WanderPath) ){
        string *sorties;
        string tmp;

        sorties = ({});
        foreach(tmp in outs){
            string dest, door;
            object ob;
            if(!permit_load) ob=find_object(dest = environment()->GetExit(tmp));
            else ob=load_object(dest = environment()->GetExit(tmp));

            if(!ob)
                continue;
            door = environment()->GetDoor(tmp);
            if( door  &&
                    door->GetClosed() ) continue;
            sorties += ({ "go " + tmp });
        }

        foreach(tmp in environment()->GetEnters(1)){
            string dest, door;
            object ob;
            if(!permit_load) ob=find_object(dest = environment()->GetEnter(tmp));
            else ob=load_object(dest = environment()->GetEnter(tmp));

            if(!ob)
                continue;
            door = environment()->GetDoor(tmp);
            if( door  &&
                    door->GetClosed() ) continue;
            sorties += ({ "enter " + tmp });
        }
        if( sizeof(sorties) ){
            eventForce(sorties[random(sizeof(sorties))]);
            return 1;
        }
        else return 0;
    }
    if( arrayp(WanderPath[WanderMarker]) ) 
        foreach(mixed cmd in WanderPath[WanderMarker]){
            if( fp = functionp(cmd) ){
                if( fp != FP_OWNER_DESTED ) evaluate(cmd);
            }
            else eventForce(cmd);
        }
    else if( fp = functionp(WanderPath[WanderMarker]) ){
        if( fp != FP_OWNER_DESTED ) evaluate(WanderPath[WanderMarker]);
    }
    else eventForce(WanderPath[WanderMarker]);
    WanderMarker++;
    if( WanderMarker >= sizeof(WanderPath) ){
        WanderMarker = 0;
        if( !WanderRecurse ) WanderPath = ({});
    }
}

/********************** sentient.c driver applies ************************/
static void heart_beat(){
    if( !this_object() || !environment() ){
        return;
    }
    npc::heart_beat();
    if( !this_object() || GetDying() || !environment() ){
        // no longer exist or in the middle of dying
        return;
    }
    if( !GetInCombat() ){ // Things to do when not in combat
        if( WanderSpeed ){ // Check if wandering
            if( WanderCount >= WanderSpeed ){ // Time to wander
                WanderCount = 0;
                eventWander();
            }
            else {
                WanderCount++;
            }
        }
    }
}
