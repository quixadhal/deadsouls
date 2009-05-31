#include <daemons.h>

varargs string gateway(mixed args){
    string ret = "";
    string *ret_array;
    mixed cookie, session;

    if(ENABLE_CREWEB){ 

        if(!args || !stringp(args)) args = "123456789101112";

        cookie = previous_object()->GetCookie();

        if(!cookie || !cookie["shib"] || !cookie["name"]){
            ret += "Not logged in.";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL=login.html\">";
            return ret;
        }

        session = WEB_SESSIONS_D->GetSession(cookie["name"]);

        if(!sizeof(session) || session["shibboleth"] != cookie["shib"]){
            WEB_SESSIONS_D->EndSession(cookie["name"]);
            ret += "Expired or conflicted session. Resetting.";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL=login.html\">";
            return ret;
        }

        ret += "Welcome to CreWeb.<br><br>";
        ret += "CreWeb is a very simple CGI web application that allows creators to<br>";
        ret += "log in, browse their home directories, upload and edit files. <br>";
        ret += "<br>";
        ret += "To mitigate potential security hazards, ";
        ret += "directories outside home dirs may not be browsed.<br>";
        ret += "<br>";
        ret += "<br>";
        ret += "<a href=\"/realms/"+cookie["name"]+"\">Your home directory is here</a><br>";
        ret += "<a href=\"chanlogs.html\">Click here to see channel logs.</a><br>";
        ret += "<a href=\"logout.html\">Click here to logout.</a><br>";
    }
    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
    }
    ret += "<br><br><a href=\"/index.html\">Home</a><br><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a>";
    ret += "<br><br>Your ip is: "+session["ip"];

    return ret;
}
