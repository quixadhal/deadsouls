#include <lib.h>
#include <save.h>
#include <sockets.h>
#include <config.h>

inherit LIB_DAEMON;

mapping Sessions = ([]);

void validate(){
    if(!(int)master()->valid_apply(({ "SECURE", "ASSIST" })) &&
      strsrch(base_name(previous_object()), DIR_WWW_GATEWAYS) &&
      strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("SESSIONS_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" SESSIONS_D breach: "+offender+" "+get_stack());
        error("SESSIONS_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

static void create() {
    if(!Sessions) Sessions = ([]);
    SetSaveFile(SAVE_SESSIONS);
    daemon::create();
}

mapping StartSession(string ip, string name, string shibboleth){
    validate();
    if(!Sessions) Sessions = ([]);
    if(!Sessions[name]) Sessions[name] = ([]);
    //tc("sessions: ip: "+ip,"cyan");
    //tc("sessions: name: "+name,"cyan");
    //tc("sessions: shibboleth: "+shibboleth,"cyan");
    Sessions[name]["ip"] = ip;
    Sessions[name]["shibboleth"] = shibboleth;
    Sessions[name]["start"] = time();
    eventSave();
    return copy(Sessions[name]);
}

string *EndSession(string name){
    validate();
    map_delete(Sessions, name);
    eventSave();
    return keys(Sessions);
}

mixed GetShibboleth(string name){
    validate();
    if(!Sessions[name] || !Sessions[name]["shibboleth"]) return 0;
    else return Sessions[name]["shibboleth"];
}

mapping GetSession(string name){
    validate();
    return copy(Sessions[name]);
}

mixed eventWriteFile(string file, string content, string name, string shibboleth ){
    mixed ret;
    validate();
    if(unguarded( (: file_exists($(file)) || directory_exists($(file)) :) ) ){
        return "A file or directory by that name already exists. Please rename your file.";
    }
    if(unguarded( (: !directory_exists(path_prefix($(file))) :))){
        return "Invalid target path selected.<br>";
    }
    if(!strsrch(file,"/realms/"+name)){
        ret = unguarded( (: write_file($(file), $(content)) :) ); 
    }
    return ret;
}



