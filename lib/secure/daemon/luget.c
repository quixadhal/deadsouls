#include <lib.h>
#include NETWORK_H
#include <commands.h>

inherit LIB_DAEMON;
string gfile, ghost;
int upgrading = 0;
int max_outbound = 5;
int max_retries = 200;
mapping FilesMap = ([]);
mapping FileQueue = ([]);
string lucmd = CMD_LIVEUPGRADE;

void validate(){
    if(!master()->valid_apply(({ "SECURE" })) ){
        string offender = identify(previous_object(-1));
        debug("LUGET_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" LUGET_D breach: "+offender+" "+get_stack());
        error("LUGET_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

static void create() {
    daemon::create();
    set_heart_beat(1);
}

varargs string convname(string raw, int rev){
    string ret = replace_string(raw,"/","0^0");
    if(rev){
        ret = replace_string(raw,"0^0","/");
        ret = replace_string(ret,"/secure/upgrades/files/","");
    }
    else ret = "/secure/upgrades/files/"+ret;
    return ret;
}

static void LUReport(string str){
    if(upgrading) lucmd->eventReceiveReport(str);
}

static int ProcessData(int fd){
    int i;
    int nullify = 0;
    string where;
    string tmp="";
    string data= FilesMap[fd]["contents"];
    mixed tmp2 = "";
    string *nullifiers = ({ "HTTP/", "HTTP0^0", "Date:", "Server:", "Last-Modified:", "ETag:",
            "Accept-Ranges:", "Content-Length:", "Connection:", "Content-Type:", "X-Pad:" });
    string *tmparr = ({});
    FilesMap[fd]["last_char"] = last(data,1);
    FilesMap[fd]["first_char"] = first(data,1);
    tmparr = explode(data,"\n");
    i = sizeof(tmparr);

    foreach(string element in tmparr){
        if(!strsrch(element,"<title>404 Not Found</title>")) nullify = 1;
        foreach(string nully in nullifiers){
            if(!strsrch(element,nully)){
                tmparr -= ({ element });
            }
        }
    }

    if(!strsrch(tmparr[0],"Location: ")) nullify = 1;

    if(!nullify && sizeof(tmparr)){
        while(!sizeof(tmparr[0]) || sizeof(tmparr[0]) < 2){
            tmparr = tmparr[1..];
        }
    }
    data = implode(tmparr,"\n");

    if(!FilesMap[fd]["started"]) FilesMap[fd]["started"] = 1;

    where = FilesMap[fd]["where"];
    FilesMap[fd]["where"] = replace_string(where,"0^0code0^0upgrades0^0"+mudlib_version(),"");
    foreach(string nully in nullifiers){
        if(grepp(where, nully)) nullify = 1;
    }
    FilesMap[fd]["normalname"] = convname(FilesMap[fd]["where"],1);
    if(directory_exists(FilesMap[fd]["normalname"])) nullify = 1;
    if(!nullify){
        if(data && (data[0] == 10 || data[0] == 13)) data = data[1..];
        if(last(data,1) != "\n") data += "\n";
        FilesMap[fd]["contents"] = data;
    }
    else FilesMap[fd]["contents"] = 0;
}

void close_callback(int fd){
    validate();

    if(FilesMap[fd]){
        LUReport("Received "+FilesMap[fd]["file"]);
        ProcessData(fd);
        if(FilesMap[fd]["contents"] && sizeof(FilesMap[fd]["contents"])){
            write_file(FilesMap[fd]["where"], FilesMap[fd]["contents"],1);
        }
        FilesMap[fd] = 0;
        map_delete(FilesMap,fd);
    }
}

varargs int GetFile(string source, string file, string host, string where, int port){
    int fd, status;

    validate();

    if(sizeof(FilesMap) > max_outbound){
        if(!FileQueue) FileQueue = ([]);
        if(!FileQueue[file]) FileQueue[file] = ([ "source" : source, "file" : file, "host" : host,
                "where" : where, "port" : (port || 80), "creation" : time(), "started" : 0,
                "user" : (this_player() ? this_player()->GetName() : identify(this_object())),
                "first_char" : "", "last_char" : "" ]);
        return 0;
    }

    fd = socket_create(STREAM, "read_callback", "close_callback");
    if( fd < 0 ){
        return 0;
    }

    FilesMap[fd] = ([ "source" : source, "file" : file, "host" : host,
            "where" : where, "port" : (port || 80), "creation" : time(), "started" : 0,
            "user" : (this_player() ? this_player()->GetName() : identify(this_object())) ]);

    status = socket_connect(fd, source +" "+port, "read_callback", "write_callback");
    if( status < 0 ){
        return 0;
    }

    return fd;
}

static varargs void RetryGet(int i){
    mixed *queue = keys(FileQueue);
    if(!i) i = 2;
    if(i > max_outbound) i = max_outbound;
    if(!FileQueue || !sizeof(FileQueue)){
        return 0;
    }
    foreach(mixed element in queue[0..i]){
        mapping  TmpMap = FileQueue[element];
        mixed foo = GetFile(TmpMap["source"],TmpMap["file"],TmpMap["host"],TmpMap["where"],TmpMap["port"]);
        if(foo){
            LUReport("%^CYAN%^Requesting: file: "+TmpMap["file"]+"%^RESET%^");
            map_delete(FileQueue, element);
        }
    }
}

static void CleanFD(int fd){
    if(!FilesMap[fd]) socket_close(fd);
    else if(!socket_status(fd)) map_delete(FilesMap,fd);
    else if(socket_status(fd)[1] != "DATA_XFER"){
        map_delete(FilesMap,fd);
        socket_close(fd);
    }
}

void heart_beat(){
    if((!FileQueue || !sizeof(FileQueue)) &&
            (!FilesMap || !sizeof(FilesMap))){
        if(upgrading){
            LUReport("\nFile download complete. After backing up your mud, issue the command: liveupgrade apply");
            upgrading = 0;
        }
        return;
    }
    RetryGet();
}

void write_callback(int fd){
    string str;
    int result = 0;
    validate();
    if(!FilesMap[fd] || !socket_status(fd)){
        CleanFD(fd);
        return; 
    }
    str ="GET "+FilesMap[fd]["file"]+" HTTP/1.0"+CARRIAGE_RETURN+"\n"+
        "Host: "+FilesMap[fd]["host"]+CARRIAGE_RETURN+"\n" +
        "User-Agent: "+ FilesMap[fd]["user"] + "@" + mud_name() + " " +
        mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
        version() + " "+
        CARRIAGE_RETURN+"\n"+CARRIAGE_RETURN+"\n";
    result = socket_write(fd, str);
}

void read_callback(int fd, mixed data){
    validate();

    if(!FilesMap[fd]){
        return;
    }
    if(!FilesMap[fd]["contents"]) FilesMap[fd]["contents"] = data;
    else FilesMap[fd]["contents"] += data;
}

varargs int eventMajorUpgrade(string source, string *files, string host, int port){
    string *allfiles = sort_array(files,1);
    validate();
    if(upgrading){
        LUReport("There is a liveupgrade already in progress.");
        return 0;
    }
    upgrading = 1;
    foreach(string element in allfiles){
        //if(last_string_element(element,1) == "/") continue;
        GetFile(source, element, host, convname(element), (port || 80));
    }
    return 1;
}

int eventDestruct(){
    int *fds = keys(FilesMap);
    validate();
    if(sizeof(fds)){
        foreach(int fd in fds){
            socket_close(fd);
        }
    }
    return ::eventDestruct();
}

int GetUpgrading(){
    return upgrading;
}
