#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <network.h>
#include <message_class.h>

inherit LIB_SOCKET;
inherit LIB_CLIENT;

string *file_chunks = ({});
string file = "";
mapping FilesMap = ([]);
string mud,whoami;
mixed globalvar;
int global_lock = 0;
string mcolor = "magenta";
int client = 0;
int counter = 0;

int eventDumpFiles();

static private void validate() {
    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access LIB_OOB: "+get_stack()+" "+identify(previous_object(-1)));
}

void eventID(string str){
    //add security stuff here
    mud = str;
}

static void create(mixed alpha, mixed beta, mixed gamma, mixed delta){
    if(alpha) trr("LIB_OOB.create alpha: "+identify(alpha),"green",MSG_OOB);
    if(beta) trr("LIB_OOB.create beta: "+identify(beta),"green",MSG_OOB);
    if(gamma) trr("LIB_OOB.create gamma: "+identify(gamma),"green",MSG_OOB);
    if(delta) trr("LIB_OOB.create delta: "+identify(delta),"green",MSG_OOB);
    set_heart_beat(1);
    //SetDestructOnClose(1);
    if(clonep()){
	if(intp(alpha)){
	    socket::create(alpha, beta);
	}

	else if(beta && intp(beta)){
	    client = 1;
	    if( eventCreateSocket(alpha, gamma) < 0 ){
		trr("LIB_OOB.create: Couldn't create outbound socket.",mcolor,MSG_OOB);
		client::eventDestruct();
		return;
	    }
	    else {
		trr("LIB_OOB.create: Apparently I opened a socket.",mcolor,MSG_OOB);
		if( delta && arrayp(delta)) {
		    trr("LIB_OOB.create Sending the oob-begin and setting globalvar.",mcolor,MSG_OOB);
		    client::eventWrite( ({ "oob-begin", mud_name(), 1, beta }) );
		    globalvar = delta;
		}
		else {
		    trr("LIB_OOB.create Sending the oob-begin.",mcolor,MSG_OOB);
		    client::eventWrite( ({ "oob-begin", mud_name(), 1, beta }) );
		}
	    }
	}
    }
    if(client) whoami = "CLIENT";
    else whoami = "SOCKET";
}

void heart_beat(){
    counter++;
    if(counter == 600){
	//tc("i am "+(client ? "client " : "socket ") +identify(this_object())+" and i'm trying to self destruct.","white");
	//tc("socket_close("+this_object()->GetDescriptor()+"): "+
	//socket_close(this_object()->GetDescriptor()), "white");
	if(client) client::eventDestruct();
	else {
	    eventClose(this_object());
	}
    }
    if(counter == 610){
	//tc("i am "+(client ? "client " : "socket ") +identify(this_object())+" and i'm trying to self destruct.","yellow");
	// socket::eventDestruct();
	eventCloseSocket();
    }
    if(counter == 620){
	//tc("i am "+(client ? "client " : "socket ") +identify(this_object())+" and i'm trying to self destruct.","red");
	// destruct();
	eventSocketClosed();
    }
}

int eventRead(mixed data) {
    validate();
    trr("--\nOOB "+whoami+" READ i am: "+identify(this_object()),mcolor,MSG_OOB);
    trr("OOB "+whoami+" READ i read: "+identify(data)+"\n--",mcolor,MSG_OOB);
    //tc("it is a: "+typeof(data),"yellow");
    counter = 0;
    if(data[0] == "oob-begin"){
	//check for token here?
	socket::eventWrite( ({ "oob-reply","Welcome to oob object "+identify(this_object()) }));
	OOB_D->RequestToken(data[1]);
	OOB_D->RegisterNewIncoming(data[1]);
    }

    if(data[0] == "oob-end"){
	//tc("hmmmmmmmmmm");
	if(sizeof(FilesMap)) eventDumpFiles();
	OOB_D->RemoveIncoming( OOB_D->FindMud(this_object()) );
	if(client) this_object()->eventDestruct();
	else socket::eventCloseSocket();
    }

    //if(sizeof(globalvar)) tc("i'd like to write this: "+identify(globalvar),"white");

    if(data[0] == "oob-reply" && !global_lock && globalvar && arrayp(globalvar)){
	global_lock = 1;
	client::eventWrite(globalvar);
    }

    if(data[0] == "oob-test"){
	socket::eventWrite( ({ "oob-reply","This is a test" }));
    }

    if(data[0] == "oob-file"){
	//tc("me: "+identify(OOB_D->FindMud(this_object()))+" "+data[1],"green");
	if(!OOB_D->AuthenticateFile(OOB_D->FindMud(this_object()), data[1])){
	    client::eventWrite( ({ "oob-file-error","CLIENT I have not asked for this file from you." }) );
	    socket::eventWrite( ({ "oob-file-error","SOCKET I have not asked for this file from you." }) );
	    return 0;
	}
	if(!FilesMap[data[1]]) FilesMap[data[1]] = ([]);
	if(data[3]) FilesMap[data[1]][data[2]] = data[3];
    }

    if(data[0] == "oob-file-req"){
	string *ok_files = explode(read_file("/secure/upgrades/txt/upgrades.txt"),"\n");
	ok_files += ({ "/secure/upgrades/txt/upgrades.txt" });
	if(member_array(data[1],ok_files) == -1 && strsrch(data[1],"/open/")){
	    client::eventWrite( ({ "oob-file-error","CLIENT File access denied." }) );
	    socket::eventWrite( ({ "oob-file-error","SOCKET File access denied." }) );
	    return 0;
	}
	if(directory_exists(data[1])){
	    socket::eventWrite( ({ "oob-file-error","That's a directory.",data[1]}) );
	    return 0;
	}
	if(!OOB_D->AuthenticateReceivedToken(OOB_D->FindMud(this_object()))){
	    socket::eventWrite( ({ "oob-file-error","No token found for your mud." }) );
	    return 0;
	}
	OOB_D->SendFile(data[1]);
    }

    if(data[0] == "oob-file-error"){
	if(data[1] == "No token found for your mud."){
	    call_out( (: eventWrite(globalvar) :), 5);
	} 
	if(data[1] == "That's a directory."){
	    mkdir(data[2]);
	    client::eventDestruct();
	}
    }

    return 1;
}

void write_data(mixed arg){
    validate();
    trr("--/nLIB_OOB.write_data i am "+whoami+" "+identify(this_object()),mcolor,MSG_OOB);
    trr("LIB_OOB.write_data i am being asked to write: "+identify(arg)+"\n--",mcolor,MSG_OOB);
    //tc("LIB_OOB.write_data prev: "+base_name(previous_object())+" "+OOB_D);
    if(base_name(previous_object()) == OOB_D)
	client::eventWrite(arg);
    if(arg && arg[0] && arg[0] == "oob-end"){
	//client::eventDestruct();
	return;
    }
}

int eventDumpFiles(){
    validate();
    //trr("LIB_OOB.eventDumpFiles DUMPING FILES:",mcolor,MSG_OOB);
    foreach(mixed key, mixed val in FilesMap){
	string fname = DIR_UPGRADES_FILES+"/"+replace_string(key,"/","0^0");
	if(key == DIR_UPGRADES_TXT+"/upgrades.txt") fname = DIR_UPGRADES_TXT+"/list.txt";
	rm(fname);
	OOB_D->RemoveRequestedFile(OOB_D->FindMud(this_object()), key);
	trr("LIB_OOB.eventDumpFiles file: "+fname,mcolor,MSG_OOB);
	for(int i = 1;i < sizeof(FilesMap[key])+1; i++){
	    if(FilesMap[key][i]) write_file(fname,FilesMap[key][i]+"\n");
	}
    }
}
