#include <daemons.h>

mapping Tries = ([]);

mapping Specials = ([
        "%21" : "\!",
        "%40" : "\@",
        "%23" : "\#",
        "%24" : "\$",
        "%25" : "\%",
        "%5E" : "\^",
        "%26" : "\&",
        "%28" : "\(",
        "%29" : "\)",
        "%2B" : "\+",
        "%60" : "\`",
        "%7E" : "\~",
        "%3D" : "\=",
        "%7B" : "\{",
        "%7D" : "\}",
        "%7C" : "\|",
        "%3A" : "\:",
        "%22" : "\"",
        "%3C" : "\<",
        "%3E" : "\>",
        "%3F" : "\?",
        "%5B" : "\[",
        "%5D" : "\]",
        "%5C" : "\\",
        "%3B" : "\;",
        "%27" : "\'",
        "%2C" : "\,",
        "%2F" : "\/",
        ]);

varargs string gateway(mixed args){
    string pass_hash, who, password, ip;
    string ret = "";
    int max_tries = 3;
    int no_user = 0;

    if(ENABLE_CREWEB){ 

        ip = previous_object()->GetIp();

        if(!args || !stringp(args)) args = "123456789101112";
        if(sscanf(args,"%s&%s",who, password) != 2){
            ret = "<br>";
        }

        else {
            who = lower_case(who);

            if(!strsrch(who,"username=")){
                who = replace_string(who,"username=","",1);
            }

            if(!strsrch(password,"password=")){
                password = replace_string(password,"password=","",1);
                if(grepp(password, "%")){
                    foreach(string key, string val in Specials){
                        password = replace_string(password, key, val);
                    }
                }
            }

            if(!ret && !user_exists(who)){
                no_user = 1;
            }

            else {
                if(!Tries) Tries = ([]);
                if(!Tries[ip]) Tries[ip] = 0;
                if(no_user){
                    pass_hash = alpha_crypt(32);
                }
                else {
                    pass_hash = PLAYERS_D->GetPlayerData(who,"Password");
                }
                if(pass_hash != crypt(password, pass_hash) ||
                        PLAYERS_D->GetPaused(who)){
                    Tries[ip]++;
                    ret = "Fail! Tries left: "+(max_tries - Tries[ip])+"<br>";
                    if(Tries[ip] >= max_tries){
                        previous_object()->eventBlockIp();
                        Tries[ip] = 0;
                    }
                }
                else {
                    string shibboleth = alpha_crypt(32);
                    string packet=who+"."+shibboleth;
                    WEB_SESSIONS_D->StartSession(ip,who,shibboleth);
                    ret = "Login successful<br>";
                    Tries[ip] = 0;
                    ret += "<meta http-equiv=\"Set-Cookie\" content=\"creweb="+packet+";path=/\">";
                    ret += "<META http-equiv=\"refresh\" content=\"1;URL=creweb.html\">";
                    return ret;
                }
            }
        }

        ret += "Log in to CreWeb using your mud username and mud password.<br><br>";
        ret += "<FORM ACTION=\"login.html\" METHOD=POST>";
        ret += "username: <INPUT TYPE=TEXT NAME=\"username\" MAXLENGTH=32><BR>";
        ret += "password:   <INPUT TYPE=PASSWORD NAME=\"password\" MAXLENGTH=32>";
        ret += "<P><INPUT TYPE=SUBMIT VALUE=\"submit\">";
        ret += "</FORM>";
        ret += "<br>";
    }
    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
    }
    ret += "<br><br><a href=\"/index.html\">Home</a><br><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a>";

    return ret;
}
