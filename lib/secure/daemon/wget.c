#include <lib.h>
#include <config.h>
#include <network.h>

inherit LIB_DAEMON;
string gwhere, gfile, ghost;
int started = 0;

static void create() {
    daemon::create();
}

int open_connection(string where, int port){
    int status, fd;

    fd = socket_create(STREAM, "read_callback", "close_callback");
    if( fd < 0 ){
	tn("socket error: "+socket_error(fd));
	return 0;
    }
    status = socket_connect(fd, where +" "+port, "read_callback", "write_callback");
    if( status < 0 ){
	tn("socket error: "+socket_error(status));
	return 0;
    }
    return 1;
}

varargs int GetFile(string source, string file, string host, string where){
    gfile = file;
    ghost = host;
    gwhere = where;
    if(where) rm(where);
    open_connection(source, 80);
}

void write_callback(int fd){
    string str ="GET "+gfile+" HTTP/1.0\r\n"+
    "Host: "+ghost+"\r\n" +
    "User-Agent: "+ "foo" + "@" + mud_name() + " " +
    mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
    version() + " "+
    "\r\n\r\n";
    int result = 0;
    tn("str: "+str);
    result = socket_write(fd, (string)str);
    tn("result: "+result+", aka: "+socket_error(result));
}

void read_callback(int fd, mixed data){
    int i;
    int nullify = 0;
    string tmp="";
    mixed tmp2 = "";
    string *tmparr = explode(data,"\n");
    string *nullifiers = ({ "HTTP/", "HTTP0^0", "Date:", "Server:", "Last-Modified:", "ETag:",
      "Accept-Ranges:", "Content-Length:", "Connection:", "Content-Type:", "X-Pad:" });

    tn("fd is: "+fd);
    tn("data: "+data,"red");

    if(!started){
	foreach(string element in tmparr){
	    foreach(string nully in nullifiers){
		if(!strsrch(element,nully)){
		    tn("deleting: "+element,"green");
		    tmparr -= ({ element });
		}
	    }
	}
	started = 1;
    }
    if(tmparr[0] == "" || !tmparr[0] || !sizeof(tmparr[0]) ||
      (sizeof(tmparr[0]) == 1 && tmparr[0][0..0] == "\r")){
	tmparr = tmparr[1..];
    }

    data = implode(tmparr,"\n");
    //if(last(data,1) != "\n") data += "\n";
    tn("data: "+data,"blue");    for(i=0;i<sizeof(data);i++){
	if(data[i] == 13) continue;
	tmp = sprintf("%c",data[i]);
	tmp2 += tmp;
	//tn("tmp: "+tmp,"white");
    }
    tn("where: "+gwhere);
    gwhere = replace_string(gwhere,"0^0code0^0upgrades0^0"+mudlib_version(),"");
    foreach(string nully in nullifiers){
	if(grepp(gwhere, nully)) nullify = 1;
    }
    tn("where: "+gwhere);
    tn("tmp2: "+tmp2,"white");
    if(!nullify) write_file(gwhere,tmp2);
}

string convname(string raw){
    string ret = replace_string(raw,"/","0^0");
    ret = "/secure/upgrades/files/"+ret;
    tn("file: "+ret);
    return ret;
}

int eventMajorUpgrade(string source, string *files, string host){
    string *allfiles = sort_array(files,1);
    foreach(string element in allfiles){
	object new_client;
	tn("element: "+element);
	tn("source: "+source);
	if(last_string_element(element,1) == "/") continue;
	new_client = new(LIB_WEB_CLIENT);
	tn("new_client: "+identify(new_client));
	new_client->GetFile(source, element, host, convname(element));
    }
    return 1;
}
