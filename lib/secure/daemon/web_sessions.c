#include <lib.h>
#include <save.h>
#include <sockets.h>

inherit LIB_DAEMON;

mapping Sessions = ([]);
static string LastError = "";

int authenticate(string name, string shibboleth){
    if(!Sessions || !Sessions[name] || !Sessions[name]["shibboleth"]) return 0;
    if(Sessions[name]["shibboleth"] == shibboleth) return 1;
    return 0;
}

varargs void validate(string name, string shibboleth){
    int auth = 1;
    if(name && shibboleth) auth = authenticate(name, shibboleth);
    if(!auth || (!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
                strsrch(base_name(previous_object()), DIR_WWW_GATEWAYS) &&
                strsrch(base_name(previous_object()), SOCKET_HTTP))){
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
    mixed ret = "Odd Fail.";
    validate(name, shibboleth);
    //if(unguarded( (: file_exists($(file)) || directory_exists($(file)) :) ) ){
    //    return "A file or directory by that name already exists. Please rename your file.";
    //}
    if(unguarded( (: !directory_exists(path_prefix($(file))) :))){
        return "Invalid target path selected.<br>";
    }

    if(unguarded( (: directory_exists($(file)) :))){
        return "Refusing to overwrite directory.<br>";
    }
    if(!strsrch(file,"/realms/"+name+"/")){
        ret = unguarded( (: write_file($(file), $(content),1) :) ); 
    }
    return ret;
}

mixed eventReadFile(string file, string name, string shibboleth){
    mixed ret;
    validate(name, shibboleth);
    if(unguarded( (: !file_exists($(file)) || directory_exists($(file)) :) ) ){
        return 0;
    }
    if(!strsrch(file,"/realms/"+name+"/")){
        if(unguarded( (: !file_size($(file)) :) ) ) ret = ""; 
        else ret = unpinkfish(unguarded( (: (read_file($(file)) || "") :) ));
    }
    return ret;
}

mixed eventSaveFile(string file, string content, string name, string shibboleth ){
    mapping  ret = ([]);
    validate(name, shibboleth);
    if(!file || !content || unguarded( (: directory_exists($(file)) :) )) return (["write" : 0]);
    if(!strsrch(file,"/realms/"+name+"/")){
        ret["write"] =  unguarded( (: write_file($(file), $(content), 1) :) );
    }
    if(ret["write"] && last(file,2) == ".c"){
        ret["error"] = unguarded( (: !update($(file)) :) );
    }
    if(ret["error"]){
        ret["report"] = LastError;
        LastError = "";
    }
    return ret;
}

void ReceiveErrorReport(string report){
    LastError += report;
}

mixed eventWebCreate(string operand, string args, string name, string shibboleth){
    mixed ret = "";
    int exists;
    validate(name, shibboleth);
    if(!operand || !args) return "Invalid arguments.";
    exists = unguarded( (: (file_exists($(operand)) || directory_exists($(operand))) :) );
    if(exists) return "Refusing to overwrite "+
        ( unguarded( (: file_exists($(operand)) :) ) ? "file." : "directory.");
    if(strsrch(operand,"/realms/"+name+"/")) return "Access to "+ path_prefix(operand)+" denied.";
    if(!unguarded( (: directory_exists(path_prefix($(operand))) :) )){     
        return path_prefix(operand)+" doesn't exist.";
    }
    if(args == "file"){
        if(!unguarded( (: write_file($(operand), "", 1) :) )){
            return "Unknown error writing file.";
        }
        else return 1;
    }

    else if(args == "dir"){
        if(!unguarded( (: mkdir($(operand)) :) )){
            return "Unknown error creating directory.";
        }
        else return 1;
    }

    else return "Invalid operation.";
}
