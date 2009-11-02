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

string gateway(string args) {
    object ob = previous_object();
    mapping cookie = ob->GetCookie();
    string current_page = ob->GetReferer();
    string ret = "<html>";
    string operand;
    mixed tmp;

    sscanf(args,"%s=%s",args, operand);

    if(ENABLE_CREWEB){
        validate();

        operand = web_translate(operand);

        if(!operand || strsrch(operand,REALMS_DIRS)) operand = REALMS_DIRS + "/" + cookie["name"];
        if(last(operand,1) == "/") operand = truncate(operand,1); 

        tmp =  WEB_SESSIONS_D->eventWebCreate(operand, args, cookie["name"], cookie["shib"]);

        if(intp(tmp)){
            ret += "Success!<br>";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL="+current_page+"\">";
        }

        else {
            //ret += "<br>args: "+args+"<br>";
            //ret += "referer: "+current_page+"<br>";
            //ret += "cookie: "+identify(cookie)+"<br>";
            //ret += "<META http-equiv=\"refresh\" content=\"5;URL="+current_page+"\">";
            ret += tmp + "<br>";
            ret += "<a href=\"/index.html\">Home</a><br>";
            ret += "<a href=\""+current_page+"\">Back</a><br<br>";
            ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
        }
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
        ret += "<a href=\"/index.html\">Home</a><br>";
        ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
    }

    ret += "</html>";
    return ret;
}
