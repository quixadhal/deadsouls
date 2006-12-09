#include <lib.h>
#include <daemons.h>
#include <message_class.h>
inherit LIB_ROOM;

int ftp, http, rcp, i3;

string LongDesc(){
    string ret = "This room is like the network room to its north, "
    "but it receives messages from the various network servers "
    "that this mud can run. If this mud does not serve as a "
    "network server, this room should be quiet. Otherwise "
    "this may be the noisiest damn room on your mud.\n";
    ret += "FTP server monitoring: "+(ftp?"%^GREEN%^online%^RESET%^":"%^RED%^OFFLINE%^RESET%^")+"\n";
    ret += "HTTP server monitoring: "+(http?"%^GREEN%^online%^RESET%^":"%^RED%^OFFLINE%^RESET%^")+"\n";
    ret += "RCP server monitoring: "+(rcp?"%^GREEN%^online%^RESET%^":"%^RED%^OFFLINE%^RESET%^")+"\n";
    ret += "I3 server monitoring: "+(i3?"%^GREEN%^online%^RESET%^":"%^RED%^OFFLINE%^RESET%^")+"\n";
    ret += "\nTo enable server monitoring, you may, for example, type:\n"
    "i3 on\n"
    "ftp on\n"
    "http on\n"
    "rcp on\n";
    return ret;
}

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Server monitoring room");
    SetLong( (: LongDesc :) );
    SetExits(([
	"north" : "/secure/room/network",
      ]));

}
int CanReceive(object ob) {
    if( !archp(ob)  ){
	message("info","The server room is available only to "+
	  "admins, sorry.",ob);
	return 0;
    }
    return 1;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3){
    //tc("event print hit. obs: "+identify(previous_object(-1)));
    //tc("msg: "+msg);
    //tc("arg2: "+arg2);
    //tc("arg3: "+identify(arg3));
    if(arg2 == MSG_FTP && !ftp) return 0;
    if(arg2 == MSG_HTTP && !http) return 0;
    if(arg2 == MSG_RCP && !rcp) return 0;
    if(arg2 == MSG_I3 && !i3) return 0;
    return ::eventPrint(msg, arg2, arg3);
}

void init(){
    ::init();
    add_action("ListenFTP","ftp");
    add_action("ListenHTTP","http");
    add_action("ListenRCP","rcp");
    add_action("ListenI3","i3");
}

int ListenI3(string str){
    if(!str || (str != "on" && str != "off")){
	write("Please specify whether you want it on or off.");
	return 1;
    }
    if(str == "on"){
	if(i3){
	    write("This room is already receiving i3 data.");
	    return 1;
	}
	write(capitalize(this_player()->GetKeyName())+" enables i3 data monitoring.");
	say("You enable i3 data monitoring.");
	i3 = 1;
	return 1;
    }
    if(!i3){
	write("This room is already blocking i3 data.");
	return 1;
    }
    write(capitalize(this_player()->GetKeyName())+" disables i3 data monitoring.");
    say("You disable i3 data monitoring.");
    i3 = 0;
    return 1;
}

int ListenFTP(string str){
    if(!str || (str != "on" && str != "off")){
	write("Please specify whether you want it on or off.");
	return 1;
    }
    if(str == "on"){
	if(ftp){
	    write("This room is already receiving ftp data.");
	    return 1;
	}
	write(capitalize(this_player()->GetKeyName())+" enables ftp data monitoring.");
	say("You enable ftp data monitoring.");
	ftp = 1;
	return 1;
    }
    if(!ftp){
	write("This room is already blocking ftp data.");
	return 1;
    }
    write(capitalize(this_player()->GetKeyName())+" disables ftp data monitoring.");
    say("You disable ftp data monitoring.");
    ftp = 0;
    return 1;
}

int ListenHTTP(string str){
    if(!str || (str != "on" && str != "off")){
	write("Please specify whether you want it on or off.");
	return 1;
    }
    if(str == "on"){
	if(http){
	    write("This room is already receiving http data.");
	    return 1;
	}
	write(capitalize(this_player()->GetKeyName())+" enables http data monitoring.");
	say("You enable http data monitoring.");
	http = 1;
	return 1;
    }
    if(!http){
	write("This room is already blocking http data.");
	return 1;
    }
    write(capitalize(this_player()->GetKeyName())+" disables http data monitoring.");
    say("You disable http data monitoring.");
    http = 0;
    return 1;
}

int ListenRCP(string str){
    if(!str || (str != "on" && str != "off")){
	write("Please specify whether you want it on or off.");
	return 1;
    }
    if(str == "on"){
	if(rcp){
	    write("This room is already receiving rcp data.");
	    return 1;
	}
	write(capitalize(this_player()->GetKeyName())+" enables rcp data monitoring.");
	say("You enable rcp data monitoring.");
	rcp = 1;
	return 1;
    }
    if(!rcp){
	write("This room is already blocking rcp data.");
	return 1;
    }
    write(capitalize(this_player()->GetKeyName())+" disables rcp data monitoring.");
    say("You disable rcp data monitoring.");
    rcp = 0;
    return 1;
}

