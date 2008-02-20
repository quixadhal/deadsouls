#include <lib.h>
#include <daemons.h>
#include <sockets.h>

inherit LIB_DAEMON;
inherit LIB_CGI;
mapping UploadsMap = ([]);

void validate(){
    if(!(int)master()->valid_apply(({ "SECURE", "ASSIST" })) &&
      strsrch(base_name(previous_object()), SOCKET_HTTP)){
        string offender = identify(previous_object(-1));
        debug("save.c SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" save.c breach: "+offender+" "+get_stack());
        error("save.c SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

string gateway(string args) {
    object ob = previous_object();
    mapping cookie = ob->GetCookie();
    string current_page = ob->GetReferer();
    string boundary = "--"+ ob->GetBoundary();
    string file, junk,outfile;
    string *tmp_arr;
    string ret = "";
    mapping Result = ([]);
    mapping ParsedPost = ([]);

    //tc("SAVE GATEWAY","red");

    if(ENABLE_CREWEB){

        ParsedPost = ParsePost(args);

        validate();

        sscanf(current_page,"%s?%s",junk, file);

        //tc("args: "+args,"white");
        //tc("file: "+file,"file");
        //tc("file prefix: "+path_prefix(file),"file");
        //tc("current_page: "+current_page,"white");
        //tc("cookie: "+identify(cookie),"white");

        //ret += args + "<br>";
        //ret += file + "<br>";
        //ret += current_page + "<br>";
        //ret += identify(cookie) + "<br>";

        //ret += "<br><br>"+identify(ParsedPost);

        if(ParsedPost["copy"] && ParsedPost["copy"] != path_prefix(file)+"/") outfile = ParsedPost["copy"];
        else outfile = file;

        //tc("outfile: "+outfile);

        Result = WEB_SESSIONS_D->eventSaveFile(outfile, ParsedPost["EditFile"], cookie["name"], cookie["shib"]);

        //tc("Result: "+identify(Result));

        if(Result["error"]){
            ret += "<br>ERROR LOADING FILE!<br><br>";
            ret += replace_string(Result["report"],"\n","<br>")+"<br><br>";
            //ret += "<br>args: "+args+"<br>";
            //ret += "referer: "+current_page+"<br>";
            //ret += "cookie: "+identify(cookie)+"<br>";
            //ret += "<META http-equiv=\"refresh\" content=\"5;URL="+current_page+"\">";
            ret += "<a href=\"/index.html\">Home</a><br>";
            ret += "<a href=\""+current_page+"\">Back</a><br><br>";
            ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
        }

        else if(!Result["write"]){
            ret += "Could not write to file!<br><br>";
            ret += "<a href=\"/index.html\">Home</a><br>";
            ret += "<a href=\""+current_page+"\">Back</a>";
        }

        else {
            ret += "Success!<br>";
            ret += "<META http-equiv=\"refresh\" content=\"1;URL="+current_page+"\">";
        }
    }

    else {
        ret += "CreWeb is disabled. To enable it: mudconfig creweb enable<br><br>";
        ret += "<a href=\"/index.html\">Home</a><br>";
        ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a><br>";
    }

    return ret;
}
