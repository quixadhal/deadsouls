#include <network.h>
#include <daemons.h>

inherit LIB_DAEMON;
inherit LIB_CLIENT;

int port = PORT_OOB;
int sock, i;

string globalmud;
string array allfiles;

mapping RequestedFiles = ([]);

mapping SentMudTokens = ([]);
mapping ReceivedMudTokens = ([]);

mapping OutgoingSessions = ([]);
mapping IncomingSessions = ([]);

string array waiting_auth = ({});
object array Clients = ({});

mixed globalvar;

void StartServer();
void StopServer();

static private void validate() {
    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access OOB_D: "+get_stack()+" "+identify(previous_object(-1)));
}

void create(){
    daemon::create();
}

varargs mixed eventBeginOOB(string mud, int token, mixed *data){
    object ob;
    int port = INTERMUD_D->GetMudList()[mud][11]["oob"];
    string ip = INTERMUD_D->GetMudList()[mud][1];
    validate();
    if(!port || !ip) return;
    if(!OutgoingSessions[mud]) OutgoingSessions[mud] = ({});
    if(data && sizeof(data)) ob = new(LIB_OOB, ip, token, port, data);
    else ob = new(LIB_OOB, ip, token, port);
    if(ob){
	OutgoingSessions[mud] += ({ ob });
	Clients += ({ ob });
	return ob;
    }
    else return 0;
}

string clean_fd(string fd){
    if(grepp(fd," ")){
	string *ip_split=explode(fd," ");
	fd = ip_split[0];
    }
    return fd;
}

void Report(){
    tc("SentMudTokens: "+identify(SentMudTokens),"yellow");
    tc("ReceivedMudTokens: "+identify(ReceivedMudTokens),"cyan");
    tc("IncomingSessions: "+identify(IncomingSessions),"white");
    tc("OutgoingSessions: "+identify(OutgoingSessions),"green");
    tc("RequestedFiles: "+identify(RequestedFiles),"blue");
}

//security hole
void SentToken(string mud, int token){
    if(!SentMudTokens[mud]) SentMudTokens[mud] = ([]);
    SentMudTokens[mud]["token"] = token;
    SentMudTokens[mud]["token_time"] = time();
}

//security hole
void ReceivedToken(string mud, int token){
    if(!ReceivedMudTokens[mud]) ReceivedMudTokens[mud] = ([]);
    ReceivedMudTokens[mud]["token"] = token;
    ReceivedMudTokens[mud]["token_time"] = time();
    if(member_array(mud, waiting_auth) != -1){
	//eventWrite( ({ "oob-begin", mud_name(), 1, token }) );
	eventBeginOOB(mud, token);
	waiting_auth -= ({ mud });
    }
}

int AuthenticateReceivedToken(string mud){
    //validate();
    if( ReceivedMudTokens[mud] &&  ReceivedMudTokens[mud]["token"] ) return 1;
    else return 0;
}

//security hole
mapping GetTokens(string arg){
    //validate();
    if(arg) return ReceivedMudTokens[arg];
    else return copy(ReceivedMudTokens);
}

int RequestToken(string target){
    //validate();
    if(!target || !(target = INTERMUD_D->GetMudName(target))) return 0;
    if(!ReceivedMudTokens[target] || !ReceivedMudTokens[target]["token"])
	INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0,target, 0 }));
    return 1;
}

varargs mixed RequestBegin(string target, mixed *data){
    string ip;
    int port = PORT_OOB;
    int token;
    mixed ret = 1;
    validate();
    tc("OOB_D.RequestBegin hit.");
    tc("OOB_D.RequestBegin Report:"+this_object()->Report());
    if(!target || !(target = INTERMUD_D->GetMudName(target))) return 0;
    if(!ReceivedMudTokens[target] || !ReceivedMudTokens[target]["token"])
	RequestToken(target);
    ip = INTERMUD_D->GetMudList()[target][1];
    if( eventCreateSocket(ip, port) < 0 ){
	tc("OOB_D.RequestBegin: couldn't create outbound socket");
	return 0;
    }
    //token = ReceivedMudTokens[target]["token"];
    //eventWrite( ({ "oob-begin", mud_name(), 1, ReceivedMudTokens[target]["token"] }) );
    //eventWrite( ({ "oob-begin", mud_name(), 1, token }) );
    if(member_array(target, waiting_auth) == -1 && (!ReceivedMudTokens[target] ||
	!ReceivedMudTokens[target]["token"]) ){
	//waiting_auth += ({ target });
	RequestToken(target);
	if(data) call_out( (: RequestBegin( $(target), $(data) ) :), 5);
	else call_out( (: RequestBegin( $(target) ) :), 5);
	return 0;
    }
    else if( ReceivedMudTokens[target] && ReceivedMudTokens[target]["token"] ){
	//eventWrite( ({ "oob-begin", mud_name(), 1, token }) );
	if(data && sizeof(data)) 
	    ret = eventBeginOOB(target, ReceivedMudTokens[target]["token"], data);
	else ret = eventBeginOOB(target, ReceivedMudTokens[target]["token"]);
    }
    tc("OOB_D.RequestBegin: ret: "+identify(ret));
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
    tc("OOB_D.eventRead: "+identify(foo));
}

varargs mixed send_file(string str, object oob){
    string *arr;
    string tmp;
    int i = 0;
    validate();
    tc("OOB_D.send_file: 1");

    if( !str ) tc("You must specify a file to cat.");
    //else str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) tc("File " + str + " not found.");
    else if( !(tmp = read_file(str)) )
	tc("Unable to read file " + str + ".");
    arr = explode(tmp,"\n");
    tc("OOB_D.send_file: 2");
    foreach(string line in arr){
	i++;
	if(oob) write_data( ({ "oob-file", str, i, line }), oob );
	else write_data( ({ "oob-file", str, i, line }) );
    }
    tc("OOB_D.send_file: 3");
    return 1;
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
    tc("OOB_D.RemoveIncoming: "+identify(previous_object())+" is asking to be removed from "+mud);
    if(!IncomingSessions[mud]) IncomingSessions[mud] = ({});
    IncomingSessions[mud] -= ({ previous_object() });
    Clients -= ({ previous_object() });
}

void AddRequestedFile(string mud, string file){
    validate();
    tc("OOB_D.AddRequestedFile prev: "+identify(previous_object(-1)));
    if((this_player() && securep(this_player())) || base_name(previous_object()) == LIB_OOB){
	if(!RequestedFiles[mud]) RequestedFiles[mud] = ({});
	RequestedFiles[mud] += ({ file });
    }
    else tc("OOB_D.AddRequestedFile bad thing.");
}

void RemoveRequestedFile(string mud, string file){
    validate();
    if(base_name(previous_object()) == LIB_OOB){
	if(!RequestedFiles[mud]) RequestedFiles[mud] = ({});
	RequestedFiles[mud] -= ({ file });
    }
}

int AuthenticateFile(string mud, string file){
    validate();
    if(!RequestedFiles[mud] || member_array(file, RequestedFiles[mud]) == -1) return 0;
    else return 1;
}

mixed SendFile(string str){
    mixed target;
    object which = previous_object();
    string mud = FindMud(which);
    validate();
    tc("OOB_D.SendFile: which: "+file_name(which),"red");
    if(base_name(which) != LIB_OOB) return 0;
    target = RequestBegin(mud);
    tc("OOB_D.SendFile: target: "+identify(target),"red");
    tc("OOB_D.SendFile: str: "+identify(str),"red");
    tc("OOB_D.SendFile: mud: "+identify(mud),"red");
    if(target && objectp(target)){
	tc("OOB_D.SendFile: target found!","cyan");
	send_file(str, target);
	target->write_data(({"oob-end", "oob-file send complete." }) );
    }
    else {
	tc("OOB_D.SendFile: no target");
	send_file(str);
	write_data(({"oob-end", "oob-file send complete." }) );
    }
    return 1;
}

mixed GetFile(string mud, string file){
    //security hole
    validate();
    AddRequestedFile(mud, file);
    RequestBegin(mud, ({ "oob-file-req", file }) );
    return 1;
}

int eventMajorUpgrade(string mud, string *files){
    int interval = 0;
    validate();
    allfiles = sort_array(files,1);
    globalmud = mud;
    foreach(string element in allfiles){
	interval += 1;
	globalvar = element;
	call_out( (: GetFile, globalmud, globalvar :), interval );
    }

    return 1;
}

