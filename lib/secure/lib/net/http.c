/*    /daemon/http.c
 *    From the Dead Souls LPC Library
 *    An http socket for the WWW (based on the Dead Souls http daemon)
 *    Created by Descartes of Borg 950429
 *    Version: @(#) http.c 1.1@(#)
 *    Last modified: 96/12/19
 */

#include <lib.h>
#include <dirs.h>

#define FILE_BAD_CMD     DIR_WWW_ERRORS "/badcmd.html"
#define FILE_BAD_GATE    DIR_WWW_ERRORS "/badgate.html"
#define FILE_NOT_FOUND   DIR_WWW_ERRORS "/notfound.html"

inherit LIB_SOCKET;

private static void eventError(string name) {
    object file = new(LIB_FILE, name);

    eventWrite(file->GetBuffer(), 1);
}

private static mixed eventGetFile(string name) {
    string array parts;
    object file;
    
    name = explode(name, " ")[0];
    if( name[0] != '/' ) {
	name = "/" + name;
    }
    parts = explode(name = absolute_path("/", name), "/");
    if( !sizeof(parts) ) {
	name = DIR_WWW "/index.html";
    }
    else if( parts[0][0] == '~' ) {
	parts[0] = user_path(parts[0][1..]) + "/public_html";
	name = implode(parts, "/");
    }
    if( strsrch(name, DIR_WWW) && strsrch(name, REALMS_DIRS) ) {
	name = DIR_WWW + name;
    }
    file = new(LIB_FILE, name);
    if( file->isDirectory() ) {
	file = new(LIB_FILE, name = name + "/index.html");
    }
    if( !strsrch(name, DIR_WWW_GATEWAYS) ) {
	string id, args, str;
	buffer b;
	
	if( sscanf(name, DIR_WWW_GATEWAYS "/%s?%s", id, args) != 2 ) {
	    args = 0;
	    sscanf(name, DIR_WWW_GATEWAYS "/%s", id);
	}
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
    else if( !file->isFile() ) {
	eventError(FILE_NOT_FOUND);
	return 1;
    }
    else {
	eventWrite(file->GetBuffer(), 1);
    }
}

int eventRead(buffer data) {
    string cmd, args;
    string str = read_buffer(data);
    
    if( !socket::eventRead(str) ) {
	return 0;
    }
    if( !str || str == "" ) {
	eventError(FILE_BAD_CMD);
	return 1;
    }
    args = explode(replace_string(str, "\r", ""), "\n")[0];
    sscanf(args, "%s %s", cmd, args);
    switch(lower_case(cmd)) {
        case "get":
	    eventGetFile(args);
	    return 1;
	    
	default:
	    eventError(FILE_BAD_CMD);
	    return 1;
    }
}
