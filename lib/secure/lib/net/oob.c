#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <network.h>

//inherit LIB_CLIENT;
inherit LIB_SOCKET;
inherit LIB_CLIENT;

string *file_chunks = ({});
string file = "";
mapping FilesMap = ([]);
string mud;
mixed globalvar;
int global_lock = 0;

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
    if(clonep()){
	if(intp(alpha)){
	    socket::create(alpha, beta);
	}

	else if(beta && intp(beta)){
	    tc("alpha: "+identify(alpha));
	    tc("beta: "+identify(beta));
	    tc("gamma: "+identify(gamma));
	    if( eventCreateSocket(alpha, gamma) < 0 ){
		tc("couldn't create outbound socket");
		this_object()->eventDestruct();
		return;
	    }
	    else {
		if( delta && arrayp(delta)) {
		    client::eventWrite( ({ "oob-begin", mud_name(), 1, beta }) );
		    globalvar = delta;
		}
		else {
		    client::eventWrite( ({ "oob-begin", mud_name(), 1, beta }) );
		}
	    }
	}
    }
}

int eventRead(mixed data) {
    validate();
    tc("i am: "+identify(this_object()),"yellow");
    tc("i read: "+identify(data),"yellow");
    tc("it is a: "+typeof(data),"yellow");
    if(data[0] == "oob-begin"){
	//check for token here?
	socket::eventWrite( ({ "oob-reply","Welcome to oob object "+identify(this_object()) }));
	OOB_D->RequestToken(data[1]);
	OOB_D->RegisterNewIncoming(data[1]);
    }

    if(data[0] == "oob-end"){
	tc("hmmmmmmmmmm");
	tc("FilesMap: "+sizeof(FilesMap));
	//print_long_string("FilesMap: "+identify(FilesMap),find_player("cratylus"));
	tc("a");
	if(sizeof(FilesMap)) eventDumpFiles();
	tc("b");
	OOB_D->RemoveIncoming( data[1] );
	tc("c");
	eventCloseSocket();
	call_out( (: this_object()->eventDestruct() :), 10);
    }

    if(data[0] == "oob-reply" && !global_lock && globalvar && arrayp(globalvar)){
	global_lock = 1;
	client::eventWrite(globalvar);
    }

    if(data[0] == "oob-test"){
	socket::eventWrite( ({ "oob-reply","This is a test" }));
    }

    if(data[0] == "oob-file"){
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
	if(member_array(data[1],ok_files) == -1){
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
	    this_object()->eventDestruct();
	}
    }

    return 1;
}

void write_data(mixed arg){
    validate();
    tc("LIB_OOB.write_data i am: "+identify(this_object()),"green");
    tc("LIB_OOB.write_data i am being asked to write: "+identify(arg),"green");
    tc("LIB_OOB.write_data prev: "+base_name(previous_object())+" "+OOB_D);
    if(base_name(previous_object()) == OOB_D)
	client::eventWrite(arg);
    if(arg && arg[0] && arg[0] == "oob-end")
	call_out( (: this_object()->eventDestruct() :), 10);
}

int eventDumpFiles(){
    validate();
    tc("LIB_OOB.eventDumpFiles DUMPING FILES:","cyan");
    foreach(mixed key, mixed val in FilesMap){
	string fname = DIR_UPGRADES_FILES+"/"+replace_string(key,"/","\\");
	if(key == DIR_UPGRADES_TXT+"/upgrades.txt") fname = DIR_UPGRADES_TXT+"/list.txt";
	rm(fname);
	OOB_D->RemoveRequestedFile(OOB_D->FindMud(this_object()), key);
	tc("LIB_OOB.eventDumpFiles file: "+fname,"cyan");
	for(int i = 1;i < sizeof(FilesMap[key])+1; i++){
	    if(FilesMap[key][i]) write_file(fname,FilesMap[key][i]+"\n");
	}
    }
}
