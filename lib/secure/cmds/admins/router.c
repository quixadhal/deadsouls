#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
mapping mudlist;
string banned, unbanned;
string *allmuds, *banned_arr, *unbanned_arr;

varargs string match_mud_name(string mud, string *list){
    list = allmuds;
    if(member_array(mud, list) != -1) return mud;
    foreach(string element in list){
	if(lower_case(element) == lower_case(mud)) {
	    return element;
	}
    }
    foreach(string element in list){
	if(!strsrch(lower_case(element), lower_case(mud))){
	    return element;
	}
	if(mudlist[element]["ip"]+" "+mudlist[element]["player_port"] == mud)
	    return mudlist[element]["name"];
    }

    return "";
}

mixed cmd(string args) {
    string arg1, arg2, tmp;
    int scannedp;
    object router = find_object(ROUTER_D);

    if(!archp(previous_object())) return 0;

    if(!router){
	write("Router is not loaded.");
	return 1;
    }

    mudlist = router->query_mudinfo();
    allmuds = keys(mudlist);

    if(!args || args == ""){
	args = "show";
    }

    if(args == "show"){
	write("Router config:");
	//write("sample mud: "+identify(mudlist[allmuds[0]]));
	router->get_info();
	return 1;
    }

    if(!router){
	write("Router is not loaded.");
	return 1;
    }

    scannedp = sscanf(args,"%s %s", arg1, arg2);

    if(args == "mudlist" || arg1 == "mudlist"){
	//write("Mudlist: \n"+identify(mudlist));
	mapping info;
	string *list;
	mapping borg;
	string mud, tempy;
	int all = 0;

	if(sizeof(arg2)) tempy = match_mud_name(arg2);
	if(sizeof(tempy)) arg2 = tempy;

	if( arg2 && arg2 != "" && strlen(arg2) > 3 ) {
	    mapping tmp_map;
	    string opt, tmpstr;

	    tmp_map = mudlist;
	    if( arg2[0] != '-' ) arg2 = "-n " + arg2;
	    opt = arg2[1..1];
	    arg2 = arg2[3..];
	    borg = ([]);

	    foreach(mud, info in tmp_map) {
		int x, y, z;

		switch(opt) {
		case "a":
		    all = 1;
		    break;
		case "m":
		    x = 5;
		    break;
		case "d":
		    x = 7;
		    break;
		case "n":
		    x = 0;
		    break;
		}
		tmpstr = (x ? info[x] : mud);
		z = strlen(arg2 = replace_string(lower_case(arg2), " ", ""));
		y = strlen(tmpstr = replace_string(lower_case(tmpstr), " ", ""));
		if( arg2 == tmpstr ) {
		    borg = ([ mud : info ]);
		    break;
		}
		else if( y > z && tmpstr[0..z-1] == arg2 && info[0] == -1 ) 
		    borg[mud] = info;
	    }
	}
	else {
	    borg = ([ ]);
	    foreach( mud, info in mudlist ){
		if( all == 1 || info["restart_delay"] == -1 ) borg[mud] = info;
	    }
	}
	if( !sizeof(borg) ) {
	    message("system", "No MUDs match your query.", this_player());
	    return 1;
	}
	else if( sizeof(borg) == 1 ) {
	    string msg, svc;
	    int val, comma = 0;

	    mud = keys(borg)[0];
	    msg = "\nDetailed information on %^GREEN%^" + mud + "%^RESET%^:\n";
	    msg += sprintf("MUD Type: %:-6s Server: %:-20s Library: %s\n",
	      borg[mud]["mud_type"], borg[mud]["driver"], borg[mud]["base_mudlib"]);
	    msg += "Status: " + borg[mud]["open_status"] + "\nAdmin email: " +
	    borg[mud]["admin_email"] + "\n";
	    msg += "Services: ";
	    foreach(svc, val in borg[mud]["services"]) {
		if( val == 1 ) {
		    if( comma ) msg += ", " + svc;
		    else {
			msg += svc;
			comma = 1;
		    }
		}
	    }
	    msg += "\nHost: " + borg[mud]["ip"] + "\n";
	    msg += "Telnet port: " + borg[mud]["player_port"] + "\n";
	    if( borg[mud]["services"]["amcp"] )
		msg += "AMCP version: " + borg[mud]["services"]["amcp"] + "\n";
	    if( borg[mud]["services"]["http"] ) 
		msg += "HTTP port (World Wide Web): " + borg[mud]["services"]["http"]+"\n";
	    if( borg[mud]["services"]["ftp"] ) 
		msg += "FTP port (File Transfer): " + borg[mud]["services"]["ftp"] + "\n";
	    if( borg[mud]["services"]["rcp"] )
		msg += "RCP port (Remote Creator): " + borg[mud]["services"]["rcp"] + "\n";
	    message("info", msg, this_player());
	    return 1;
	}

	list = ({});
	foreach(mud, info in borg)
	list += ({ sprintf("%:-15s %:-6s %:-15s %:-18s %s %d",
	    //replace_string(mud,"%^","%%^^"), info[8], info[7], info[5], info[1], info[2]) });
	    replace_string(mud,"%^","%%^^"), info["mud_type"], info["driver"], info["base_mudlib"], info["ip"], info["player_port"]) });
	list = sort_array(list, 1);
	list = ({ replace_string(mud_name(),"%^","%%^^") + " recognizes " + consolidate(sizeof(borg), "a mud")+
	  " matching your query: ", "" }) + list;
	this_player()->eventPage(list);
	return 1;
    }

    if(args == "reload" || args == "restart"){
	if(router){
	    router->eventDestruct();
	    router = find_object(ROUTER_D);
	    if(router) write("Router unload failed.");
	    else write("Router unloaded from memory.");
	    flush_messages();
	}
	write("Loading router daemon...");
	flush_messages();
	router = load_object(ROUTER_D);
	if(!router) write("Router load failed.");
	else write("Router loaded.");
	return 1;
    }

    if(args == "on" || args == "online"){
	if(router){
	    write("The router daemon is already online.");
	    return 1;
	}
	else {
	    write("Loading router daemon...");
	    flush_messages();
	    router = load_object(ROUTER_D);
	    if(!router) write("Router load failed.");
	    else write("Router loaded.");
	    return 1;
	}
    }

    if(args == "off" || args == "offline"){
	if(!router){
	    write("The router daemon is already offline.");
	    return 1;
	}
	else {
	    write("Unloading router daemon...");
	    flush_messages();
	    router->eventDestruct();
	    router = find_object(ROUTER_D);
	    if(router) write("Router unload failed.");
	    else write("Router unloaded from memory.");
	    return 1;
	}
    }

    if(!router){
	write("The intermud router is not loaded.");
	return 1;
    }

    if(!arg2) arg1 = args;

    if(arg1 == "ban" || arg1 == "banned"){
	if(sizeof(router->GetBannedMuds())) banned_arr = router->GetBannedMuds();
	else banned_arr = ({});
	if(!sizeof(banned_arr)) banned = "No banned muds.";
	else banned = implode(banned_arr,", ");
	if(!arg2){
	    write("Banned muds: "+banned);
	    return 1;
	}
	tmp = match_mud_name(arg2, allmuds);
	if(!sizeof(tmp)){
	    write("There is no such mud, but I am adding the name to the list.");
	}
	else arg2 = tmp;

	router->AddBannedMud(arg2);
	write(arg2+" has been added to the banned list.");
	return 1;
    }

    if(arg1 == "unban" || arg1 == "unbanned"){
	if(sizeof(router->GetBannedMuds())) banned_arr = router->GetBannedMuds();
	else banned_arr = ({});
	if(!sizeof(banned_arr)) unbanned = "No banned muds.";
	else {
	    banned = implode(banned_arr,", ");
	    unbanned_arr = filter(keys(mudlist), (: member_array($1, banned_arr) == -1 :) ) || ({});
	    unbanned = implode(unbanned_arr,", ");
	}
	if(!arg2){
	    write("Unbanned muds: "+unbanned);
	    return 1;
	}
	//tmp = match_mud_name(arg2, allmuds);
	//if(!sizeof(tmp)){
	//   write("There is no such mud, but I am removing the name from the list.");
	//}
	//else arg2 = tmp;

	router->RemoveBannedMud(arg2);
	write(arg2+" has been removed from the banned list.");
	return 1;
    }


    if(arg1 == "id" || arg1 == "identify"){
	if(!arg2){
	    write("Identify which mud?");
	    return 1;
	}
	tmp = match_mud_name(arg2, allmuds);
	if(!sizeof(tmp)){
	    write("Mud not found.");
	    return 1;
	}
	else arg2 = tmp;

	write("The proper name is: "+arg2);
	return 1;
    }


    if(arg1 == "config"){
	string sendarg = ""; 
	string s1, s2, s3;
	if(arg2 && sizeof(arg2))
	    sscanf(arg2,"%s %s %s", s1, s2, s3);
	if(s1){
	    if(!strsrch(s1,"*")) true();
	    else s1 = "*"+s1;
	}
	if(!s3){
	    write("Syntax: router config NAME IP PORT");
	    return 1;
	}
	router->SetRouterName(s1);
	router->SetRouterIP(s2);
	router->SetRouterPort(s3);
	router->SetRouterList();
	write("Config complete. To activate, type: router reload");
	return 1;
    }

    if(arg1 == "deletemud"){
	if(!arg2 || !sizeof(arg2)){
	    write("Syntax: router deletemud MUDNAME");
	    return 1;
	}
	arg2 = match_mud_name(arg2, allmuds); 
	router->remove_mud(arg2, 1);
	write("Mud removed.");
	return 1;
    }

    write("Router command completed.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: \n\n"
      "\n\n"
      "");
}
