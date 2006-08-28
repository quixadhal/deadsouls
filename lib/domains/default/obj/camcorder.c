#include <lib.h>

inherit LIB_ITEM;
inherit LIB_TURN;
inherit LIB_COMMAND;
inherit LIB_MESSAGES;
inherit LIB_CHAT;
inherit LIB_GENETICS;
inherit LIB_LANGUAGE;

string baseshort;
int recording;
string recfile;

void create() {
    item::create();
    command::create();
    messages::create();
    chat::create();
    genetics::create();
    SetKeyName("camcorder");
    SetId( ({ "camera", "videocam", "cam" }) );
    SetAdjectives( ({ "video", "small","portable" }) );
    SetShort("a camcorder");
    SetLong("It is a small camera that records moving images and sound.");
    baseshort=GetShort();
}

void init(){
    ::init();
}

void catch_tell( string message ){
    tc("catch_tell: "+message);
}

void receive_message(string s1, string s2){
    tc("receive_message: "+s2,"yellow");
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
  string msg, string lang) {
    tc("eventHearTalk");
    this_object()->receive_message("me",who->GetName()+" "+verb+"s: "+msg) ;
    return;
}

varargs int doPrint(string msg, string msg_class){
    tc("doPrint");
    this_object()->receive_message("me again",msg) ;
    return 1;
}

varargs int eventPrint(string msg, string msg_class){
    tc("eventPrint");
    doPrint( msg, msg_class) ;
    return 1;
}

varargs mixed eventTurn(string str){
    if( this_player() != environment() ) { write("You don't have that."); return 0; }
    write("You turn the camcorder over in your hands.");
    say(this_player()->GetName()+" turns over a camcorder in "+possessive(this_player())+" hands.");
    return 1;
}

int eventTurnOn(object ob){
    if(this_player() != environment()  && environment(this_player()) !=environment()) {
	write("It isn't within reach.");
	return 1;
    }
    if(!recording){
	write("You turn on the camcorder.");
	say(this_player()->GetName()+" turns on a camcorder.");
	SetShort(baseshort+" (%^BOLD%^RED%^%^FLASH%^recording%^RESET%^)");
	recording = 1;
	return 1;
    }
    if(recording){
	write("It is already on.");
	return 1;
    }
}

varargs mixed eventTurnOff(string str){
    if(this_player() != environment()  && environment(this_player()) !=environment()) { write("It isn't within reach."); return 1; }
    if(recording){
	write("You turn off the camcorder.");
	say(this_player()->GetName()+" turns off a camcorder.");
	SetShort(baseshort);
	return 1;
    }
    if(!recording){
	write("It is already off.");
	return 1;
    }
}

