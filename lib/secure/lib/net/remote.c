/*    /secure/daemon/remote.c
 *    from the Foundation II LPC Library
 *    Remote Creator Protocol daemon
 *    created by Descartes of Borg 950504
 */

#include <lib.h>
#include <network.h>

inherit LIB_SOCKET;

private string Password;
private static mapping Connections;
private static int FD = -1;

static void Setup();

void eventRead(int fd, string str);
static void eventProcess(int fd, string str);

static void create(int fd, object owner){
    socket::create(fd, owner);
    FD = fd;
    //trr("this object: "+identify(this_object())+" created","blue");
    //trr("fd: "+fd+", owner: "+identify(owner),"blue");
    Connections = ([]);
    //Connections[FD]["init"] = "crikey!";
    //call_out( (: Setup :), 1);
}

//static void Setup() {
//    if( eventCreateSocket(PORT_RCP) < 0 ) {
//	if( this_object() ) Destruct();
//	return;
//    }
//}

static void eventSocketClosed(int fd) {
    //tc("hit eventSocketClosed, arg: "+fd);
    map_delete(Connections, fd);
}

void eventRead(string str) {
    //tc("hit eventRead, arg: "+str,"blue");
    if( !str ) {
	eventWrite("50 Invalid command.\n", 1);
	if( Connections[FD] ) map_delete(Connections, FD);
	return;
    }
    eventProcess(FD, str);
}

static private void eventProcess(int fd, string str) {
    string tmp, cmd, arg, file, val;
    int x;
    //tc("hit eventProcess, args: "+fd+" "+str,"blue");
    if( Connections[fd] && Connections[fd]["in edit"] > 0 ) {
	int len;

	if( !Connections[fd]["object"] ) {
	    eventWrite("50 Object destructed.\n", 1);
	    map_delete(Connections, fd);
	    return;
	}
	Connections[fd]["buffer"] += str;
	if( !(len = strlen(Connections[fd]["buffer"])) ) return;
	if( len == Connections[fd]["in edit"] ) {
	    tmp =  Connections[fd]["object"]->eventWriteFile(Connections[fd]["file"],
	      Connections[fd]["buffer"]);
	    eventWrite(tmp + "\n");
	    Connections[fd]["in edit"] = 0;
	    Connections[fd]["file"] = "";
	    Connections[fd]["buffer"] = "";
	    return;
	}
	else if( len > Connections[fd]["in edit"] ) {
	    string tmp1, tmp2;

	    tmp1 =Connections[fd]["buffer"][0..(Connections[fd]["in edit"]-1)];
	    tmp2 =Connections[fd]["buffer"][Connections[fd]["in edit"]..];
	    Connections[fd]["buffer"] = "";
	    eventProcess(fd, tmp1);
	    eventProcess(fd, tmp2);
	    return;
	}
	return;
    }
    //trr("remote: not busy","blue");
    if( !Connections[fd] ) Connections[fd] = ([ "buffer" : "" ]);
    if( (Connections[fd]["buffer"] += str) == "" ) return;
    if( (x = strsrch(Connections[fd]["buffer"], "\n")) == -1 ) return;
    str = Connections[fd]["buffer"][0..(x-1)];
    if( x != strlen(Connections[fd]["buffer"]) - 1 ) 
	Connections[fd]["buffer"] = Connections[fd]["buffer"][(x+1)..];
    else Connections[fd]["buffer"] = "";
    if( sscanf(str, "%s %s", cmd, arg) != 2) {
	cmd = replace_string(str, CARRIAGE_RETURN, "");
	arg = "";
    }
    else arg = replace_string(arg, CARRIAGE_RETURN, "");
    //trr("Connections: "+identify(Connections),"blue");
    if( !Connections[fd]["object"] ) {
	string username, password;
	//trr("!Connections[fd][\"object\"] )","blue");
	if( cmd != "login" ) {
	    eventWrite("50 Must login with user name and password.\n", 1);
	    map_delete(Connections, fd);
	    return;
	}
	if( sscanf(arg, "%s %s", username, password) != 2 ) {
	    eventWrite("50 Login failed.\n", 1);
	    map_delete(Connections, fd);
	    return;
	}
	username = convert_name(username);
	if( !user_exists(username) ) {
	    eventWrite("50 Login failed.\n", 1);
	    map_delete(Connections, fd);
	    return;
	}
	unguarded( (: restore_object, DIR_CRES "/" + username[0..0] + "/" +
	    username :) );
	if( Password != crypt(password, Password) ) {
	    log_file("remote", "Failed attempt to login as " + username 
	      + "\n");
	    eventWrite("50 Login failed.\n", 1);
	    map_delete(Connections, fd);	     
	    return;
	}
	if( !(Connections[fd]["object"] = 
	    load_object(user_path(username) + "adm/remote")) ) {
	    eventWrite("50 Failed to load remote object.\n", 1);
	    map_delete(Connections, fd);
	    return;
	}
	eventWrite("60 Connection to " + mud_name() + ".\n");
    }
    else if( cmd == "100" ) {
	int sz;

	if( sscanf(arg, "%d %s", sz, Connections[fd]["file"]) != 2 ) 
	    eventWrite("50 Bad file send command.\n");
	else {
	    if( !sz ) eventWrite("110 No changes sent orwritten.\n");
	    else Connections[fd]["in edit"] = sz;
	}
    }
    else switch( cmd ) {
    case "edit":
	file = (string)Connections[fd]["object"]->eventReadFile(arg);
	if( file[<1] != '\n' ) file += "\n";
	tmp = sprintf( "%-14s\n", "100 " + strlen(file));
	eventWrite(tmp[0..15]);
	eventWrite(file);
	break;
    case "ls":
	val = (string)Connections[fd]["object"]->eventCommand(cmd, arg);
	if( val ) eventWrite("500 " + val + "\n");
	else eventWrite("50 " +cmd+ " " +arg+ ": Permission denied.\n");
	break;
    case "update":
	val = (string)Connections[fd]["object"]->eventCommand(cmd, arg);
	if( val ) eventWrite("510 " + val + "\n");
	else eventWrite("50 Update attempt went off into nowhere.\n");
	break;
    default:
	val = (string)Connections[fd]["object"]->eventCommand(cmd, arg);
	if( val ) eventWrite("400 " + val + "\n");
	else eventWrite("50 "+cmd+" "+arg+": Command not supported.\n");
	break;
    }
    eventProcess(fd, "");
}
