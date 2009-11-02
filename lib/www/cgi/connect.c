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

varargs string gateway(mixed args, mixed args2) {
    object ob = previous_object();
    string host = ob->GetHost();
    string port = ob->GetPort();
    string ret = "";
    if(args2){
        host = args;
        port = args2;
    }

    if(ENABLE_CREWEB){
        //validate();

        ret += "<html><center>"+mud_name()+" Connection Page<br><br>";
        ret += "<applet codebase=\"http://dead-souls.net/javaclient/\" ";
        ret += "archive=\"jta25.jar\" code=\"de.mud.jta.Applet\"";
        ret += " height=\"20\" width=\"200\">";
        ret += "<param name=\"config\" value=\"applet.conf\">";
        ret += "<PARAM NAME=\"Socket.host\" VALUE=\""+host+"\">";
        ret += "<PARAM NAME=\"Socket.port\" VALUE=\""+query_host_port()+"\">";
        ret += "<br><b>Your Browser seems to have no ";
        ret += "<a href=\"http://java.sun.com/\">Java</a> ";
        ret += "support. Please get a new browser or enable Java to ";
        ret += "see this applet!</b><br></applet></center>";
        ret += "<br> <br><a href=\"/index.html\">Home</a><br>";
        ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
        ret += "<a href=\"/index.html\">Home</a><br>";
        ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
    }

    return ret;
}
