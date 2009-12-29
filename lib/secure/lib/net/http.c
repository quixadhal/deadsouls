/*    /daemon/http.c
 *    From the Dead Souls LPC Library
 *    An http socket for the WWW (based on the Dead Souls http daemon)
 *    Created by Descartes of Borg 950429
 *    Version: @(#) http.c 1.1@(#)
 *    Last modified: 96/12/19
 */

/* Note on how this works:
 * On Dead Souls, this http program is in fact not so much a "daemon"
 * as an "applet". The inet daemon starts a server program that 
 * watches the http port. When a request comes in, that server program
 * fires up a *clone* of this here file. Therefore, you will not see
 * multiple-connection handling here. This http applet services a
 * single connection, and dies happy when its job is done.
 *
 * The CGI stuff is basically just sending the post and get requests to
 * LPC objects in the cgi/ directory. The key thing to remember if you're 
 * going to play around here is that this object closes and dies on write. So
 * if you're going to receive post data in multiple chunks (as is common)
 * you will want to ensure it is either queued here until sent to the
 * cgi object, or that the cgi object somehow prevents this object from
 * writing its data to the client til all chunks are received and processed.
 * 
 * -Crat 09Feb2008
 */

#include <lib.h>
#include <dirs.h>
#include <logs.h>
#include <daemons.h>

#define FILE_BAD_CMD     DIR_WWW_ERRORS "/badcmd.html"
#define FILE_BAD_GATE    DIR_WWW_ERRORS "/badgate.html"
#define FILE_NOT_FOUND   DIR_WWW_ERRORS "/notfound.html"
#define ACCESS_DENIED    DIR_WWW_ERRORS "/denied.html"
#define DIRECTORY_LISTINGS

#ifndef ENABLE_CGI
#define ENABLE_CGI 1
#endif
#ifndef WWW_DIR_LIST
#define WWW_DIR_LIST 1
#endif

#define TAB "&nbsp&nbsp&nbsp&nbsp"

inherit LIB_SOCKET;

string ip = "";
string out = "";
string host;
int port;
mapping Cookie = ([]);
string gateway, login_data, current_page, boundary, cookie, cmd, read_args, filename, user_agent;
int ok_to_send, boundary_count;
int logging = 1;

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" })) &&
            strsrch(base_name(previous_object()), DIR_WWW_GATEWAYS)){
        string offender = identify(previous_object(-1));
        debug("HTTPD SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("/secure/security", "\n"+timestamp()+" HTTPD breach: "+offender+" "+get_stack());
        error("HTTPD SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

void eventSendData(string str){
    buffer b;
    int i;
    validate();
    if(!str || !sizeof(str)) return;
    str = strip_colours(str);
    b = allocate_buffer(strlen(str));
    for(i=0; i<strlen(str); i++) {
        b[i] = str[i];
    }
    eventWrite(b, 1);
}

void eventLogConnection(){
    if(logging){
        log_file(LOG_HTTP,ip+" "+timestamp()+" "+identify(read_args)+" "+current_page+"\n");
    }
}

int authenticate(string path){
    validate();
    if(!Cookie || !sizeof(Cookie) || !Cookie["name"] || !Cookie["shib"]) return 0;
    if(!strsrch(path,REALMS_DIRS)){
        if(!strsrch(path,REALMS_DIRS+"/"+Cookie["name"])){
            if(WEB_SESSIONS_D->GetShibboleth(Cookie["name"]) == Cookie["shib"]){
                return 1;
            }
        }
    }
    return 0;
}

string GetReferer(){
    return current_page;
}

string GetBoundary(){
    return boundary;
}

mapping GetCookie(){
    validate();
    return copy(Cookie);
}

string GetHost(){
    return host;
}

int GetPort(){
    return port;
}

private static void eventError(string name) {
    object file = new(LIB_FILE, name);

    eventWrite(file->GetBuffer(), 1);
}

void eventBlockIp(){
    if(!strsrch(base_name(previous_object()), DIR_WWW_GATEWAYS) ){
        INET_D->eventBlockIp(ip);
    }
}

string GetIp(){
    return ip;
}

mixed GenerateIndex(string dir, string requested){
    string ret = "<html>\n";
    string prefix = "";
    mixed *listing;
    if(!dir || !directory_exists(dir)) return 0; 
    listing = get_dir(dir+"/");
    if(!strsrch(dir,DIR_WWW)) dir = replace_string(dir,DIR_WWW,"",1);
    prefix = path_prefix(requested);
    if(!sizeof(prefix)) prefix = "/";
    if(WEB_SESSIONS_D->GetSession(Cookie["name"])){
        if(authenticate(dir) && requested[1..1] != "~"){
            ret += "<FORM METHOD=POST ENCTYPE=\"multipart/form-data\" ACTION=\"/cgi/upload.html\">";
            ret += "Upload file (text files only): <INPUT TYPE=FILE NAME=\"upfile\">";
            ret += "<INPUT TYPE=SUBMIT VALUE=\"Submit\"></FORM>";
            ret += "<FORM ACTION=\"/cgi/new\">Create a folder <INPUT name=\"dir\" VALUE=\""+dir+"/\" SIZE=40></FORM>";
            ret += "<FORM ACTION=\"/cgi/new\">Create a file <INPUT name=\"file\" VALUE=\""+dir+"/\" SIZE=40></FORM>";
        }
    }
    ret += "<a href=\""+prefix+"\">Parent Directory   </a>\n";
    ret +="<hr style=\"width: 100%; height: 2px;\"><br>\n";
    foreach(string sub in listing){
        string ed_req = "<a href=\"/cgi/edit.html?"+dir+"/"+sub+"\">Edit</a>";
        ret +="<a href=\""+requested+"/"+sub+"\">"+sub+(directory_exists(dir+"/"+sub) ? "/" : "")+"</a>";
        ret += (directory_exists(dir+"/"+sub) ? "" : TAB + ed_req);
        ret += "<br>\n";
    }
    ret +="</html>";
    ret = replace_string(ret,"//","/");
    return ret;
}

void eventRemoveTmp(string file){
    rm(file);
}

varargs private static mixed eventGetFile(string name, string type, string payload) {
    string array parts;
    string tmpfile, orig, requested;
    object file;

    name = explode(name, " ")[0];
    if( name[0] != '/' ) {
        name = "/" + name;
    }
    requested = name;
    parts = explode(name = absolute_path("/", name), "/");
    if( !sizeof(parts) ) {
        name = DIR_WWW "/index.html";
    }
    else if( parts[0][0] == '~' ) {
        parts[0] = user_path(parts[0][1..]) + "/public_html";
        name = implode(parts, "/");
    }

    if(name == REALMS_DIRS || name == REALMS_DIRS +"/") name = DIR_WWW + "/index.html";

    if( strsrch(name, DIR_WWW) && strsrch(name, REALMS_DIRS) ) {
        name = DIR_WWW + name;
    }
    if(!strsrch(name,REALMS_DIRS) && !grepp(name,"/public_html") ){
        if(!authenticate(name)) eventError(FILE_NOT_FOUND);
    }
    orig = name;
    file = new(LIB_FILE, name);
    if( file->isDirectory() ) {
        file = new(LIB_FILE, name = name + "/index.html");
    }
    if( ENABLE_CGI && !strsrch(name, DIR_WWW_GATEWAYS) ) {
        string id, args, str, foo;
        buffer b;

        if( sscanf(name, DIR_WWW_GATEWAYS "/%s?%s", id, args) != 2 ) {
            args = 0;
            sscanf(name, DIR_WWW_GATEWAYS "/%s", id);
        }
        if( sscanf(id, "%s.%s", foo, str) == 2 ) {
            id = foo+".c";
        }
        if(payload) args = payload;
        if( catch(str = (DIR_WWW_GATEWAYS "/"+id)->gateway(args)) ) {
            eventError(FILE_BAD_GATE);
            return 1;
        }
        str = strip_colours(str);
        b = allocate_buffer(strlen(str));
        for(int i=0; i<strlen(str); i++) {
            b[i] = str[i];
        }
        eventWrite(b, 1);
        return 1;
    }
    else if( WWW_DIR_LIST && !file->isFile() ) {
        mixed ret = GenerateIndex(orig, requested);
        tmpfile = "/secure/tmp/webtemp.html";
        if(ret){
            write_file(tmpfile, ret,1);
            file = new(LIB_FILE, tmpfile);
            eventWrite(file->GetBuffer(),1);
        }
        else { 
            eventError(FILE_NOT_FOUND);
            return 1;
        }
    }
    else if( !WWW_DIR_LIST && !file->isFile() ) {
        eventError(FILE_NOT_FOUND);
        return 1;
    }
    else {
        eventWrite(file->GetBuffer(), 1);
    }
}

int eventRead(buffer data) {
    string *args_tmp, *posted_file;
    string browser, *sock_stat;
    string str = read_buffer(data);
    int argsize, i, j;
    buffer b;

    if(!Cookie || !sizeof(Cookie)){
        Cookie = ([]);
        Cookie["name"] = 0;
        Cookie["shib"] = 0;
    }

    ip = socket_ip(socket::GetDescriptor());

    if(member_array(ip,INET_D->GetBlockedIps()) != -1){
        eventError(ACCESS_DENIED);
        return 1;
    }

    if( !socket::eventRead(str) ) {
        return 0;
    }
    if( !str || str == "" ) {
        eventError(FILE_BAD_CMD);
        return 1;
    }
    if(!read_args) read_args = explode(replace_string(str, CARRIAGE_RETURN, ""), "\n")[0];
    args_tmp = explode(replace_string(str, CARRIAGE_RETURN, ""), "\n");
    foreach(mixed element in args_tmp){
        int int1, int2;
        string junk1, junk2;
        junk2 = reverse_string(element);
        int2 = sscanf(junk2,"%s---%*s",junk1);
        if(!strsrch(element,"Cookie:") && !cookie) cookie = element;
        if(!strsrch(element,"Host:") && !host){
            if(sscanf(element,"Host: %s:%s",junk1, junk2) != 2){
                sscanf(element,"Host: %s",junk1);
                port = 80;
            }
            else port = atoi(junk2);
            host = junk1;
        }
        if(boundary && grepp(element,boundary)){
            boundary_count++;
        }
        else if(boundary && int2 == 2){
            junk2 = reverse_string(junk1);
            junk1 = replace_string(boundary,"-","");
            if(boundary && sizeof(junk2) > 5
                    && first(junk1,sizeof(junk2)) == junk2){
                boundary_count++;
            }
        }

        if(grepp(element, "boundary=")){
            sscanf(element,"%sboundary=%s",junk1,boundary);
            args_tmp -= ({ element });
        }
        if(!strsrch(element, "Referer:") && !current_page){
            sscanf(element,"Referer: %s",current_page);
        }
        if(!strsrch(element, "User-Agent:") && !user_agent){
            sscanf(element,"User-Agent: %s",user_agent);
        }
        if(!strsrch(element, "Content-Disposition: form-data;") && !filename){
            if(sscanf(element,"%sfilename=\"%s\"",junk1, filename) != 2)
                sscanf(element,"%sfilename=\"%s\"%s",junk1, filename, junk2);
            if(filename && filename[1..2]==":\\"){
                //lol mircosoft
                filename=last_string_element(filename,"\\");
            }
        }
        if(!strsrch(element, "username=") && !login_data){
            login_data = element;
        }
    }

    out += implode(args_tmp,"\n");
    if(cookie){
        string name, shib, junk1, junk2;
        if(sscanf(cookie,"%screweb=%s.%s;%s",junk1,name,shib,junk2) == 4 ||
                sscanf(cookie,"%screweb=%s.%s",junk1,name,shib) == 3){
            Cookie["name"] = name;
            Cookie["shib"] = shib;
        }
    }
    if(!cmd) sscanf(read_args, "%s %s", cmd, read_args);
    if(!read_args) sscanf(read_args, "%s %s", cmd, read_args);
    eventLogConnection();
    switch(lower_case(cmd)) {
        string junk;
        case "get":
            eventGetFile(read_args);
        return 1;

        case "post":
            if(!ENABLE_CGI){
                eventError(FILE_BAD_CMD);
                return 1;
            }
        if(!gateway) sscanf(read_args,"%s HTTP%s",gateway,junk);

        if(boundary_count && boundary_count > 1){
            string junk1, junk2, tmp;
#if 0
            if(sscanf(out,"%s"+boundary+"%s--"+boundary+"%s",junk1,tmp,junk2) == 3){
                out = tmp;
                args_tmp=explode(out,"\n");
                if(filename) out = implode(args_tmp[2..],"\n");
                else out = implode(args_tmp[1..],"\n");
            }
#endif
            sscanf(out,"%s--%s",tmp,junk1);
            if(grepp(out,boundary+"--")) eventGetFile(read_args, "POST", out);
            return 1;

#if 0       
            if(!filename){
                if( catch(str = ((DIR_WWW_GATEWAYS +"/save")->gateway(out)) ) ){
                    eventError(FILE_BAD_GATE);
                    return 1;
                }
            }
            else if( catch(str = ((DIR_WWW_GATEWAYS +"/upload")->gateway(out, current_page, filename,
                                Cookie["name"], Cookie["shib"]))) ) {
                eventError(FILE_BAD_GATE);
                return 1;
            }
            str = strip_colours(str);
            b = allocate_buffer(strlen(str));
            for(j=0; j<strlen(str); j++) {
                b[j] = str[j];
            }
            eventWrite(b, 1);
#endif
        }
        else if(login_data){
            eventGetFile("cgi/login.html?"+login_data);
        }
        return 1;

        default:
        eventError(FILE_BAD_CMD);
        return 1;
    }
    return 1;
}
