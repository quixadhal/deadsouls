#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
inherit LIB_CGI;
string gfile;

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
            strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("showlog.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" showlog.c breach: "+offender+" "+get_stack());
        error("showlog.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string *eventLinkify(string *arg){
    string junk2;
    int i;
    string *ret = ({});
    if(!sizeof(arg)) return ret;
    if(!grepp(implode(arg,"\n"),"ttp://")) return arg;
    foreach(string str in arg){
        if(grepp(lower_case(str),"ttp://")){
            string junk1, url;
            i = sscanf(str,"%sttp://%s %s",junk1,url,junk2);
            if(i != 3) i = sscanf(str,"%sttp://%s)%s",junk1,url,junk2);
            else if(i != 3) i = sscanf(str,"%sttp://%s)",junk1,url);
            if(i < 3) i = sscanf(str,"%sttp://%s",junk1,url);

            if(junk1){
                str = replace_string(str,url,"1010011101001110100111010011",1);
                ret += ({ replace_string(str,"1010011101001110100111010011",
                            "<a href=\"http://"+url+"\">"+url+"</a>",1) });
            }
            else ret += ({ str });
        }
        else ret += ({ str });
    } 
    return ret;
}

string gateway(mixed args) {
    string line_str, log_contents, log_ret;
    string *log_arr = ({});
    int i, lines;
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

        i = sscanf(args,"%s+%s", gfile, line_str); 
        if(i != 2 || !(lines = atoi(line_str)) || 
                !(log_contents = read_file(DIR_CHANNEL_LOGS +"/"+ gfile))){
            ret += "Bad request.<br>"; 
            ret += "<a href=\"/index.html\">Home</a><br>";        
            ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
            return ret;
        }
        if(sizeof(explode(log_contents,"\n")) < lines){
            mapping Archives = ([]);
            string *archive_array = get_dir(DIR_CHANNEL_LOGS +"/archive/");
            archive_array = filter(archive_array, (: !strsrch($1,gfile) :) );

            foreach(string element in archive_array){
                string junk1;
                if(sscanf(element,"%s%*d.%*d.%*d-%*d.%*d", junk1) != 6)
                    continue;
                if(truncate(junk1,1) != gfile) continue;
                Archives[stat(DIR_CHANNEL_LOGS+"/archive/"+element)[1]] = element;
            }
            if(sizeof(Archives)){
                mixed *order = sort_array(keys(Archives),-1);
                log_contents = (read_file(DIR_CHANNEL_LOGS+"/archive/"+
                            Archives[order[0]]) || "") + log_contents;
            }
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

