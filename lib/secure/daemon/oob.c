#include NETWORK_H
#include <message_class.h>
#include <daemons.h>

inherit LIB_DAEMON;
inherit LIB_CLIENT;

string globalmud;
string array allfiles;
string mcolor = "magenta";

mapping RequestedFiles = ([]);

mapping SentMudTokens = ([]);
mapping ReceivedMudTokens = ([]);

mapping OutgoingSessions = ([]);
mapping IncomingSessions = ([]);

string array waiting_auth = ({});
object array Clients = ({});

mixed globalvar, g1, g2;

void StartServer();
void StopServer();

static private void validate() {
    if( !(master()->valid_apply(({ "SECURE" }))) )
        error("Illegal attempt to access OOB_D: "+get_stack()+" "+identify(previous_object(-1)));
}

void create(){
    daemon::create();
}

varargs mixed eventBeginOOB(string mud, int token, mixed *data){
    object ob;
    mapping MudList = INTERMUD_D->GetMudList();
    int port = MudList[mud][11]["oob"];
    string ip = MudList[mud][1];
    if(sizeof(MudList[mud][12]) &&
            MudList[mud][12]["ip"] &&
            MudList[mud][12]["ip"] != "127.0.0.1")
        ip = MudList[mud][12]["ip"];
    validate();
    trr("OOB_D.eventBeginOOB, mud: "+mud+", token: "+token,"yellow",MSG_OOB);
    if(!port || !ip) return;
    if(!OutgoingSessions[mud]) OutgoingSessions[mud] = ({});
    if(data && sizeof(data)) ob = new(LIB_OOB, ip, token, port, data);
    else ob = new(LIB_OOB, ip, token, port);
    if(ob){
        trr("OOB_D.eventBeginOOB, ob: "+identify(ob),"yellow",MSG_OOB);
        OutgoingSessions[mud] += ({ ob });
        Clients += ({ ob });
        return ob;
    }
    else {
        trr("OOB_D.eventBeginOOB: there is no new ob created. Weird.","red",MSG_OOB);
        return 0;
    }
}

string clean_fd(string fd){
    if(grepp(fd," ")){
        string *ip_split=explode(fd," ");
        fd = ip_split[0];
    }
    return fd;
}

void Report(){
    validate();
    trr("SentMudTokens: "+identify(SentMudTokens),"yellow");
    trr("ReceivedMudTokens: "+identify(ReceivedMudTokens),"cyan");
    trr("IncomingSessions: "+identify(IncomingSessions),"white");
    trr("OutgoingSessions: "+identify(OutgoingSessions),"green");
    trr("RequestedFiles: "+identify(RequestedFiles),"blue");
}

void SentToken(string mud, int token){
    if(base_name(previous_object()) != SERVICES_D) return 0;
    if(!SentMudTokens[mud]) SentMudTokens[mud] = ([]);
    SentMudTokens[mud]["token"] = token;
    SentMudTokens[mud]["token_time"] = time();
}

void ReceivedToken(string mud, int token){
    if(base_name(previous_object()) != SERVICES_D) return 0;
    if(!ReceivedMudTokens[mud]) ReceivedMudTokens[mud] = ([]);
    ReceivedMudTokens[mud]["token"] = token;
    ReceivedMudTokens[mud]["token_time"] = time();
    if(member_array(mud, waiting_auth) != -1){
        eventBeginOOB(mud, token);
        waiting_auth -= ({ mud });
    }
}

int AuthenticateReceivedToken(string mud){
    if( ReceivedMudTokens[mud] &&  ReceivedMudTokens[mud]["token"] ) return 1;
    else return 0;
}

mapping GetTokens(string arg){
    if(base_name(previous_object()) != SERVICES_D) return 0;
    if(arg) return ReceivedMudTokens[arg];
    else return copy(ReceivedMudTokens);
}

int RequestToken(string target){
    if(!target || !(target = INTERMUD_D->GetMudName(target))) return 0;
    if(!ReceivedMudTokens[target] || !ReceivedMudTokens[target]["token"])
        INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0,target, 0 }));
    return 1;
}

varargs mixed RequestBegin(string target, mixed *data){
    string ip;
    int port;
    int token;
    mixed ret = 1;
    validate();
    if(!target || !(target = INTERMUD_D->GetMudName(target))) return 0;
    if(!ReceivedMudTokens[target] || !ReceivedMudTokens[target]["token"])
        RequestToken(target);
    ip = INTERMUD_D->GetMudList()[target][1];
    if(sizeof(INTERMUD_D->GetMudList()[target][12]) &&
            INTERMUD_D->GetMudList()[target][12]["ip"] &&
            INTERMUD_D->GetMudList()[target][12]["ip"] != "127.0.0.1")
        ip = INTERMUD_D->GetMudList()[target][12]["ip"];
    port = INTERMUD_D->GetMudList()[target][11]["oob"];
    if( eventCreateSocket(ip, port) < 0 ){
        trr("OOB_D.RequestBegin: couldn't create outbound socket",mcolor,MSG_OOB);
        return 0;
    }
    else trr("OOB_D.RequestBegin socket created",mcolor,MSG_OOB);
    if(member_array(target, waiting_auth) == -1 && (!ReceivedMudTokens[target] ||
                !ReceivedMudTokens[target]["token"]) ){
        RequestToken(target);
        if(data) call_out( (: RequestBegin( $(target), $(data) ) :), 5);
        else call_out( (: RequestBegin( $(target) ) :), 5);
        return 0;
    }
    else if( ReceivedMudTokens[target] && ReceivedMudTokens[target]["token"] ){
        if(data && sizeof(data)) 
            ret = eventBeginOOB(target, ReceivedMudTokens[target]["token"], data);
        else ret = eventBeginOOB(target, ReceivedMudTokens[target]["token"]);
    }
    return ret;
}

int RequestEnd(int fd){
    validate();
    eventWrite( ({ "oob-end", mud_name() }) );
    return 1;
}

varargs void write_data(mixed arg, object oob){
    validate();
    if(sizeof(Clients) && !oob){
        foreach(object ob in Clients){
            if(ob) ob->write_data(arg);
        }
        return;
    }
    oob->write_data(arg);
}

void eventRead(mixed foo){
    validate();
}

varargs mixed send_file(string str, object oob){
    string *arr;
    string tmp;
    int i = 0;
    validate();
    trr("OOB_D.send_file str: "+identify(str),"cyan",MSG_OOB);
    if(oob) trr("OOB_D.send_file oob: "+identify(oob),"cyan",MSG_OOB);

    if( !str ) trr("You must specify a file to cat.","cyan",MSG_OOB);
    str = trim(str);
    if( !file_exists(str) ) trr("File " + str + " not found.","cyan",MSG_OOB);
    else if( !(tmp = read_file(str)) )
        trr("Unable to read file " + str + ".","cyan",MSG_OOB);
    if( !(tmp = read_file(str)) ) {
        trr("File " + str + " not readable","cyan",MSG_OOB);
        return 0;
    }
    arr = explode(tmp,"\n");
    if(file_size(str) > 60000){
        trr("OOB_D.send_file: file too large. Send of "+str+" %^WHITE%^FAILED%^RESET%^.");
        return 1;
    }
    if(!strsrch(str,"/secure/upgrades/txt/upgrades."))
        str = "/secure/upgrades/txt/upgrades.txt";
    if(!strsrch(str,"/secure/upgrades/txt/mud_info."))
        str = "/secure/sefun/mud_info.c";
    trr("OOB_D.send_file str: "+identify(str),"cyan",MSG_OOB);
    foreach(string line in arr){
        i++;
        if(oob) write_data( ({ "oob-file", str, i, line }), oob );
        else write_data( ({ "oob-file", str, i, line }) );
    }
    trr("OOB_D.send_file: sent.","cyan",MSG_OOB);
    return 1;
}

varargs void call_send_file(string str, object oob){
    g1 = str;
    g2 = oob;
    call_out( (:send_file, g1, g2 :), 2);
}

string FindMud(object ob){
    validate();
    foreach(mixed key, mixed val in OutgoingSessions){
        foreach(mixed element in val){
            if(ob && ob == element) return key;
        }
    }
    foreach(mixed key, mixed val in IncomingSessions){
        foreach(mixed element in val){
            if(ob && ob == element) return key;
        }
    }
    return "";
}

void RegisterNewIncoming(string name){
    validate();
    if(!IncomingSessions[name]) IncomingSessions[name] = ({});
    IncomingSessions[name] += ({ previous_object() });
    Clients += ({ previous_object() });
}

void RemoveIncoming(string mud){
    validate();
    trr("OOB_D.RemoveIncoming: "+identify(previous_object())+" is asking to be removed from "+mud,mcolor,MSG_OOB);
    if(!IncomingSessions[mud]) IncomingSessions[mud] = ({});
    IncomingSessions[mud] -= ({ previous_object() });
    Clients -= ({ previous_object() });
}

void AddRequestedFile(string mud, string file){
    validate();
    if((this_player() && securep(this_player())) || base_name(previous_object()) == LIB_OOB){
        if(!RequestedFiles[mud]) RequestedFiles[mud] = ({});
        RequestedFiles[mud] += ({ file });
    }
}

void RemoveRequestedFile(string mud, string file){
    validate();
    if(base_name(previous_object()) == LIB_OOB){
        if(!RequestedFiles[mud]) RequestedFiles[mud] = ({});
        RequestedFiles[mud] -= ({ file });
    }
}

int AuthenticateFile(string mud, string file){
    if(!RequestedFiles[mud] || member_array(file, RequestedFiles[mud]) == -1) return 0;
    else return 1;
}

mixed SendFile(string str){
    mixed target;
    object which = previous_object();
    string mud = FindMud(which);
    validate();
    trr("OOB_D.SendFile: which: "+file_name(which),mcolor,MSG_OOB);
    if(base_name(which) != LIB_OOB) return 0;
    target = RequestBegin(mud);
    trr("OOB_D.SendFile: target: "+identify(target),mcolor,MSG_OOB);
    trr("OOB_D.SendFile: str: "+identify(str),mcolor,MSG_OOB);
    trr("OOB_D.SendFile: mud: "+identify(mud),mcolor,MSG_OOB);
    if(target && objectp(target)){
        trr("OOB_D.SendFile: target found!",mcolor,MSG_OOB);
        send_file(str, target);
        target->write_data(({"oob-end", "oob-file send complete." }) );
    }
    else {
        trr("OOB_D.SendFile: no target",mcolor,MSG_OOB);
        send_file(str);
        write_data(({"oob-end", "oob-file send complete." }) );
    }
    return 1;}

    mixed GetFile(string mud, mixed file){
        validate();
        if(stringp(file)) AddRequestedFile(mud, file);
        else if(arrayp(file))
            foreach(string element in file){
                AddRequestedFile(mud, element);
            }
        RequestBegin(mud, ({ "oob-file-req", file }) );
        return 1;
    }

int eventMajorUpgrade(string mud, string *files){
    int interval = 0;
    validate();
    allfiles = sort_array(files,1);
    globalmud = mud;
    GetFile(globalmud, allfiles);
    return 1;
}

mixed SendMail(mapping mail){
    mixed target;
    validate();
    trr("OOB_D.SendMail: mail: "+identify(mail),mcolor,MSG_OOB);
    foreach(mixed key, mixed val in mail){
        if(sizeof(val)){
            foreach(mixed id, mixed message in val){
                target = RequestBegin(key, message);
                trr("OOB_D.SendMail: target: "+identify(target),mcolor,MSG_OOB);
                trr("OOB_D.SendMail: mud: "+identify(key),mcolor,MSG_OOB);
                if(target && objectp(target)){
                    trr("OOB_D.SendMail: target found!",mcolor,MSG_OOB);
                    target->write_data(message);
                }
                else {
                    trr("OOB_D.SendMail: no target",mcolor,MSG_OOB);
                }
            }
        }
    }
    return 1;
}
