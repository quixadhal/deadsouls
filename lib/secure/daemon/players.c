#include <objects.h>
#include <save.h>
#include <dirs.h>

mapping PlayerDataMap = ([]);
string *PendingEncres = ({});
string *PendingDecres = ({});
string *players = ({});
string *creators = ({});
string *user_list = ({});

string player_save_file;

void create() {
    if(unguarded((: file_exists, SAVE_PLAYER_LIST+__SAVE_EXTENSION__ :)))
	unguarded((: restore_object, SAVE_PLAYER_LIST :));
}

string *eventCre(string str){
    str = lower_case(str);
    if(member_array(str,creators) == -1) creators += ({ str });
    if(member_array(str,players) != -1) players -= ({ str });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return creators;
}

string *eventDecre(string str){
    str = lower_case(str);
    if(member_array(str,creators) != -1) creators -= ({ str });
    if(member_array(str,players) == -1) players += ({ str });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return players;
}

void AddPlayerInfo(mixed arg) {

    if(!objectp(arg) && !stringp(arg)) return ;

    if(objectp(arg)){
	if(!interactive(arg)) return;
	else player_save_file = base_name(arg)+".o";
    }

    else {
	arg = lower_case(arg);
	if(last(arg,2) != ".o") arg += ".o";
	if(!grepp(arg,"/")){
	    string initial = arg[0..0];
	    player_save_file = DIR_CRES +"/"+initial+"/"+arg;
	    if(!file_exists(player_save_file))
		player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
	    if(!file_exists(player_save_file)) return;
	}
	else player_save_file = arg;
	if(grepp(DIR_CRES, player_save_file)) creators += ({ replace_string(last_string_element(player_save_file,"/"),".o","") }) ; 
	else players += ({ replace_string(last_string_element(player_save_file,"/"),".o","") }) ; 
	if(!file_exists(player_save_file)) return;
    }

    tc("player_save_file: "+player_save_file);
    user_list += ({ replace_string(last_string_element(player_save_file,"/"),".o","") }) ; 
    unguarded((: save_object, SAVE_PLAYER_LIST :));
}

string *GetPlayerList(){
    return players;
}

string *GetCreatorList(){
    return creators;
}

string *GetUserList(){
    return user_list;
}

string *AddPendingEncre(string str){
    if(catch((int)master()->valid_apply()) ||
      !(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){ 
	string offender = identify(previous_object(-1));
	debug("SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
	log_file("security", "\n"+timestamp()+" AddPendingEncre breach: "+offender+" "+get_stack());
	return ({});
    }
    if(str && str != "") PendingEncres += ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingEncres;
}

string *RemovePendingEncre(string str){
    if(catch((int)master()->valid_apply()) ||
      !(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
	string offender = identify(previous_object(-1));
	debug("SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
	log_file("security","\n"+ timestamp()+" RemovePendingEncre breach: "+offender+" "+get_stack());
	return ({});
    }
    if(!str || str == "") return PendingEncres;
    str = lower_case(str);
    if(member_array(str, PendingEncres) != -1) PendingEncres -= ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingEncres;
}

string *GetPendingEncres(){
    return PendingEncres;
}

string *AddPendingDecre(string str){
    if(catch((int)master()->valid_apply()) ||
      !(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
	string offender = identify(previous_object(-1));
	debug("SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
	log_file("security", "\n"+timestamp()+" AddPendingDecre breach: "+offender+" "+get_stack());
	return ({});
    }
    if(str && str != "") PendingDecres += ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingDecres;
}

string *RemovePendingDecre(string str){
    if(catch((int)master()->valid_apply()) ||
      !(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
	string offender = identify(previous_object(-1));
	debug("SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
	log_file("security", "\n"+timestamp()+" RemovePendingDecre breach: "+offender+" "+get_stack());
	return ({});
    }
    if(!str || str == "") return PendingDecres;
    str = lower_case(str);
    if(member_array(str, PendingDecres) != -1) PendingDecres -= ({ lower_case(str) });
    unguarded((: save_object, SAVE_PLAYER_LIST :));
    return PendingDecres;
}

string *GetPendingDecres(){
    return PendingDecres;
}

