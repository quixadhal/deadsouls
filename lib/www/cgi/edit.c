#include <lib.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
mapping UploadsMap = ([]);

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
            strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("edit.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" edit.c breach: "+offender+" "+get_stack());
        error("edit.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string eventGenerateEditor(string file, string name, string shib){
    mixed tmp;
    string ret = "File not found.";
    validate();
    tmp = WEB_SESSIONS_D->eventReadFile(file, name, shib);
    if(tmp){
        ret = read_file(DIR_WWW_GATEWAYS+"/edit_script.js")+"<br>\n";
        ret += "Editing "+file+"";
        ret += "<FORM ACTION=\"/cgi/save.html\" METHOD=POST ENCTYPE=\"multipart/form-data\"><br>";
        ret += "<TEXTAREA NAME=\"EditFile\" ID=\"EditFile\" style=\"width:600px;height:400px;\" wrap=\"off\">";
        ret += tmp;
        ret += "</TEXTAREA><SCRIPT TYPE=\"text/javascript\">eventCreateTextArea('EditFile');</script>";
        ret += "<INPUT TYPE=SUBMIT VALUE=\"submit\"><br><br>";
        ret += "Write to a different file: <INPUT NAME=\"copy\" VALUE=\""+path_prefix(file)+"/\"";
        ret += "SIZE=60";
        ret += "</FORM><br>"; 
        ret += "</body>";
    }
    return ret;
}

string gateway(string args) {
    object ob = previous_object();
    mapping cookie = ob->GetCookie();
    string current_page = ob->GetReferer();
    string ret = "";

    if(ENABLE_CREWEB){
        validate();

        ret += eventGenerateEditor(args, cookie["name"], cookie["shib"]);
    }
    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
    }
    ret += "<a href=\"/index.html\">Home</a><br>";
    ret += (ENABLE_CREWEB ? "<a href=\""+path_prefix(args)+"\">Back</a><br<br>": "<br>");
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";

    return ret;
}
