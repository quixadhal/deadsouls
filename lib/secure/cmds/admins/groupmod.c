#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(mixed args) {
    string config_file, new_config_file = "", str;
    string groupname, dudename;
    string *line_array;
    string *top_array;
    string *bottom_array;
    int action = 0;
    mapping GroupsMap = ([]);

    if(strsrch(args,"-d ") != -1){
	action = -1;
	args = replace_string(args,"-d ","");
    }
    if(strsrch(args,"-a ") != -1){
	action = 1;
	args = replace_string(args,"-a ","");
    }
    if(!this_player()) return 0;
    if( !this_player() || !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access groupmod: "+get_stack()+" "+identify(previous_object(-1)));

    sscanf(args,"%s %s",groupname,dudename);

    top_array = ({});
    bottom_array = ({});

    config_file = unguarded( (: read_file("/secure/cfg/groups.cfg") :) );
    line_array = explode(config_file, "\n");
    str = upper_case(groupname);
    dudename=(lower_case(dudename));
    line_array = explode(config_file,"\n");
    foreach(string line in line_array){
	string g,m;
	if(sizeof(line)) line = trim(line);
	if(sscanf(line,"(%s)%s",g,m) != 2) 
	    if(sscanf(line,"(%s)",g) != 1) continue;
	if(m){
	    GroupsMap[g] = explode(trim(m),":");
	}
	else GroupsMap[g] = ({});
    }

    if(str == "ASSIST" || str == "SECURE" ) {
	if(!securep(this_player())){
	    write("Only full admins may do this.");
	    return 1;
	}
    }

    if(!GroupsMap[str]){
	write("No such group.");
	return 1;
    }

    if(!action){
	write("I don't understand what you want. Please specify ");
	write("which action you want, -d  or -a");
	return 1;
    }

    if(action == 1){
	if(member_array(dudename,GroupsMap[str]) != -1){
	    write("That person is already a member of that group.");
	    return 1;
	}
	GroupsMap[str] += ({ dudename });
	write("The group "+str+" now contains the following:\n "+
	  implode(GroupsMap[str],":"));
    }

    else {
	if(member_array(dudename,GroupsMap[str]) == -1){
	    write("That person is already not a member of that group.");
	    return 1;
	}
	GroupsMap[str] -= ({ dudename });
	write("The group "+str+" now contains the following:\n "+
	  implode(GroupsMap[str],":"));
    }

    foreach(string key, mixed val in GroupsMap){
	new_config_file += "("+key+") "+implode(GroupsMap[key],":")+"\n";
    }
    write_file("/secure/cfg/groups.cfg",new_config_file,1);
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    return 1;
}

void help() {
    message("help", "Syntax: groupmod [-a|-d] GROUP NAME \n\n"
      "", this_player());
}
