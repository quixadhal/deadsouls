#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
inherit LIB_CGI;

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
            strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("chanlogs.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" chanlogs.c breach: "+offender+" "+get_stack());
        error("chanlogs.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string gateway(mixed args) {
    mapping Logs = ([]);
    int *times = ({});
    string err, full_name, path, junk1, junk2;
    int i;
    object ob = previous_object();
    string current_page = ob->GetReferer();
    mapping cookie = ob->GetCookie();
    string name = cookie["name"];
    string shib = cookie["shib"];
    string ret = "";

    if(ENABLE_CREWEB){
        string *logfiles = ({});
        validate();

        if(!name || !shib || !WEB_SESSIONS_D->authenticate(name, shib)){
            ret += "Bad session. <br>";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL=login.html\">";
            return ret;
        }

        foreach(string file in get_dir(DIR_CHANNEL_LOGS+"/")){
            if(!file_exists(DIR_CHANNEL_LOGS+"/"+file)) continue;
            Logs[file] = stat(DIR_CHANNEL_LOGS+"/"+file)[1];
        }
        times = sort_array(distinct_array(values(Logs)),-1);
        foreach(mixed element in times){
            foreach(mixed key, mixed val in Logs){
                string tmp_ret = "";//This shouldn't be needed, but it is.
                if(val == element){
                    tmp_ret += "<a href=\"/cgi/showlog.html?"+key+"+20\"";
                    tmp_ret += ">"+key+"    (last modified ";
                    tmp_ret += ctime(val);
                    tmp_ret += ")</a><br>";
                }
                ret += tmp_ret;
            }
        }

        ret += "<br>";
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
    }
    ret += "<a href=\"/index.html\">Home</a><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";

    return ret;
}

