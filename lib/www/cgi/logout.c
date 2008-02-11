#include <daemons.h>

varargs string gateway(mixed args){
    object ob = previous_object();
    mixed cookie = ob->GetCookie();
    string ret = "";

    if(!ENABLE_CREWEB){
        ret += "CreWeb is disabled.";
        ret += "<META http-equiv=\"refresh\" content=\"2;URL=login.html\">";
        return ret;
    }

    if(!cookie || !cookie["shib"] || !cookie["name"]){
        ret += "Not logged in.";
        ret += "<META http-equiv=\"refresh\" content=\"1;URL=login.html\">";
        return ret;
    }

    WEB_SESSIONS_D->EndSession(cookie["name"]);
    ret += "Logging out...";
    ret += "<META http-equiv=\"refresh\" content=\"2;URL=login.html\">";
    return ret;
}
