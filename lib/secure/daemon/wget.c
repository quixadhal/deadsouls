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
	tn("WGET_D: socket error: "+socket_error(fd),"red");
	return 0;
    }
    status = socket_connect(fd, where +" "+port, "read_callback", "write_callback");
    if( status < 0 ){
	tn("WGET_D: socket error: "+socket_error(status),"red");
	return 0;
    }
    return 1;
}

varargs int GetFile(string source, string file, string host, string where, int port){
    gfile = file;
    ghost = host;
    gwhere = where;
    if(where) rm(where);
    open_connection(source, (port || 80));
}

void write_callback(int fd){
    string str ="GET "+gfile+" HTTP/1.0"+CARRIAGE_RETURN+"\n"+
    "Host: "+ghost+CARRIAGE_RETURN+"\n" +
    "User-Agent: "+ "foo" + "@" + mud_name() + " " +
    mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
    version() + " "+
    CARRIAGE_RETURN+"\n"+CARRIAGE_RETURN+"\n";
    int result = 0;
    tn("str: "+str);
    result = socket_write(fd, (string)str);
    if(result < 0) tn("result: "+result+", aka: "+socket_error(result));
    else tn("WGET_D: request successfully sent from client "+identify(this_object()));
}

void read_callback(int fd, mixed data){
    int i;
    int nullify = 0;
    string tmp="";
    mixed tmp2 = "";
    string last_char = last(data,1);
    string *tmparr = explode(data,"\n");
    string *nullifiers = ({ "HTTP/", "HTTP0^0", "Date:", "Server:", "Last-Modified:", "ETag:",
      "Accept-Ranges:", "Content-Length:", "Connection:", "Content-Type:", "X-Pad:" });
    //tmp2 = implode(tmparr,"\n");
    //tmparr = explode(tmp2,"\n");
    data = replace_string(data,"\n"+CARRIAGE_RETURN,"\n");
    data = replace_string(data,CARRIAGE_RETURN+"\n","\n");
    data = replace_string(data,CARRIAGE_RETURN,"\n");
    tmparr = explode(data,"\n");
    //if(last(data,1) != last_char) tc("AQUI GUAPO: "+gwhere,"white");
    //if(last_char == "\n") tc("last_char is \\n, guapo. file: "+gwhere,"red");
    //if(last_char == "\r") tc("last_char is \\r, guapo. file: "+gwhere,"green");
    //if(last_char == "\t") tc("last_char is \\t, guapo. file: "+gwhere,"blue");
    //tc("last data char: \""+last_char+"\", size: "+sizeof(last_char)+" N?: "+(last_char == "\n" ? "yes" : "no"));
    //tc("last data char: \""+last_char+"\", size: "+sizeof(last_char)+" R?: "+(last_char == "\r" ? "yes" : "no"));
    tn("fd is: "+fd);
    tn("data: "+data,"red");
    if(!started){
	write_file("/tmp/foo.txt",data,1);
	foreach(string element in tmparr){
	    foreach(string nully in nullifiers){
		if(!strsrch(element,nully)){
		    tn("deleting: "+element,"green");
		    tmparr -= ({ element });
		}
	    }
	}
	//tc("tmparr[0]: \""+tmparr[0]+"\", size: "+sizeof(tmparr[0])+" R?: "+(tmparr[0] == "\r" ? "yes" : "no"));
	if(!strsrch(tmparr[0],"Location: ")) nullify = 1;

	while(sizeof(tmparr[0]) < 2){
	    if(sizeof(tmparr) > 1){
		tmparr = tmparr[1..];
		//tc("tmparr[0]: "+tmparr[0],"green");
	    }
	    else break;
	}
	started = 1;
    }

    else {
	write_file("/tmp/foo.txt",data);
    }
    //foreach(string line in tmparr){
    //if(grepp(line, " has evaded you.")){
    //int memb = member_array(line,tmparr);
    // tc("line: "+line,"cyan");
    //tc("size of tmparr: "+sizeof(tmparr),"cyan");
    //tc("member: "+memb,"cyan");
    //tc("memb["+(memb-1)+"]: "+tmparr[memb-1],"cyan");
    //tc("memb["+(memb+1)+"]: "+tmparr[memb+1],"cyan");
    //}
    //}
    data = implode(tmparr,"\n");
    if(last(data,1) != last_char){
	//tc("AQUI GUAPO: "+gwhere,"yellow");
	if(last_char == CARRIAGE_RETURN ) data += "\n";
	else data += last_char;
    }

    tn("where: "+gwhere);
    gwhere = replace_string(gwhere,"0^0code0^0upgrades0^0"+mudlib_version(),"");
    foreach(string nully in nullifiers){
	if(grepp(gwhere, nully)) nullify = 1;
    }
    tn("where: "+gwhere);
    tn("data: "+data,"white");
    if(!nullify) write_file(gwhere,data);
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
