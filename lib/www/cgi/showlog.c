#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
inherit LIB_CGI;
mapping UploadsMap = ([]);

void validate(){
    if(!(int)master()->valid_apply(({ "SECURE", "ASSIST" })) &&
      strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("upload.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" upload.c breach: "+offender+" "+get_stack());
        error("upload.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string *eventLinkify(string *arg){
    string url, junk1, junk2;
    int i;
    string *ret = ({});
    if(!sizeof(arg)) return ret;
    if(!grepp(implode(arg,"\n"),"ttp://")) return arg;
    foreach(string str in arg){
        if(!grepp(lower_case(str),"ttp://")) continue;
        i = sscanf(str,"%sttp://%s %s",junk1,url,junk2);
        if(i != 3) i = sscanf(str,"%sttp://%s)%s",junk1,url,junk2);
        else if(i != 3) i = sscanf(str,"%sttp://%s)",junk1,url);
        else if(i != 2) i = sscanf(str,"%sttp://%s",junk1,url);
        else if(i != 2) continue;

        if(!str || !url) continue;

        str = replace_string(str,url,"1010011101001110100111010011",1);
        ret += ({ replace_string(str,"1010011101001110100111010011",
            "<a href=\"http://"+url+"\">"+url+"</a>",1) });
    } 
    return ret;
}

string gateway(mixed args) {
    string file, line_str, log_contents, log_ret;
    string *log_arr = ({});
    int i, lines;
    object ob = previous_object();
    string current_page = ob->GetReferer();
    mapping cookie = ob->GetCookie();
    string name = cookie["name"];
    string shib = cookie["shib"];
    string ret = "";

    //tc("SHOWLOG GATEWAY","red");

    if(ENABLE_CREWEB){
        string *logfiles = ({});
        validate();

        //tc("ob: "+identify(ob),"yellow");
        //tc("current_page: "+current_page,"white");
        //tc("args: "+args,"white");

        if(!name || !shib || !WEB_SESSIONS_D->authenticate(name, shib)){
            ret += "Bad session. <br>";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL=login.html\">";
            return ret;
        }

        i = sscanf(args,"%s+%s", file, line_str); 
        if(i != 2 || !(lines = atoi(line_str)) || 
          !(log_contents = read_file(DIR_CHANNEL_LOGS +"/"+ file))){
            ret += "Bad request.<br>"; 
            ret += "<a href=\"/index.html\">Home</a><br>";        
            ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
            return ret;
        }

        log_arr = eventLinkify(explode(log_contents,"\n")[<lines..]);
        log_ret = implode(log_arr,"<br>");
        ret += log_ret;
        ret += "<br>";
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
    }
    ret += "<a href=\"/index.html\">Home</a><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";

    return ret;
}
