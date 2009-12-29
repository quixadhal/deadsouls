#include <lib.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
inherit LIB_CGI;
mapping UploadsMap = ([]);

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
            strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("upload.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" upload.c breach: "+offender+" "+get_stack());
        error("upload.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string gateway(mixed args) {
    string err, full_name, path, junk1, junk2;
    int i;
    object ob = previous_object();
    string current_page = ob->GetReferer();
    mapping cookie = ob->GetCookie();
    string name = cookie["name"];
    string shib = cookie["shib"];
    mapping ProcessedPost = ParsePost(args);
    string posted_file = ProcessedPost["upfile"];
    string filename = ProcessedPost["filename"];
    string ret = "";

    if(ENABLE_CREWEB){
        validate();

        i = sscanf(current_page,"http://%s/%s",junk1, path);
        if(i != 2) i = sscanf(current_page,"/%s",path);
        if(i != 1) i = sscanf(current_page,"./%s",path);

        path = "/"+path;
        if(last(path,1) != "/") path += "/";
        full_name = path+filename;

        if(!filename || !sizeof(filename)){
            ret += "No file selected.<br>";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL="+current_page+"\">";
            return ret;
        }

        if(stringp(err = WEB_SESSIONS_D->eventWriteFile(full_name, posted_file, name, shib))){
            ret += err+"<br>";
            ret += "<a href=\""+current_page+"\">Return</a><br>";
            return ret;
        }

        ret += "Success!<br>";
        ret += "<META http-equiv=\"refresh\" content=\"1;URL="+current_page+"\">";
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
        ret += "<a href=\"/index.html\">Home</a><br>";
        ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
    }

    return ret;
}
