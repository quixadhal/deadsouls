#include <daemons.h>

varargs string gateway(mixed args){
    string ret = "";
    string *ret_array;
    mixed cookie, session;

    if(!args || !stringp(args)) args = "123456789101112";

    //tc("args: "+args);

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

    //tc("cookie: "+identify(cookie), "white");

    ret += "Welcome to CreWeb.<br><br>";
    ret += "CreWeb is a very simple CGI web application that allows creators to<br>";
    ret += "log in, browse their home directories, and upload files. <br>";
    ret += "<br>";
    ret += "To mitigate potential security hazards, some limitations are ";
    ret += "in place:<br>";
    ret += "* Files may not be overwritten.<br>";
    ret += "* Directories may not be created.<br>";
    ret += "* Nothing outside home directories may be browsed.<br>";
    ret += "<br>";
    ret += "<br>";
    ret += "<a href=\"/realms/"+cookie["name"]+"\">Click here to begin.</a><br>";
    ret += "<a href=\"logout.html\">Click here to logout.</a><br>";
    ret += "<br><br><a href=\"/index.html\">Home</a><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a>";

    return ret;
}
