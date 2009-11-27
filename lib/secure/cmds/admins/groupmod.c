#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string new_config_file = "";
string dudename;

mixed cmd(mixed args) {
    string config_file, str;
    string groupname, s1, s2;
    string *line_array;
    string *top_array;
    string *bottom_array;
    string *cles = ({});
    int reload_player, action = 0;
    mapping GroupsMap = ([]);

    if( !this_player() || !(master()->valid_apply(({ "ASSIST" }))) )
        error("Illegal attempt to access groupmod: "+get_stack()+" "+identify(previous_object(-1)));


    if(!args){
        write(read_file("/secure/cfg/groups.cfg"));
        return 1;
    }

    if(sscanf(args,"%s %s",s1,s2) != 2){
        write("Insufficient number of arguments. Try: help groupmod");
        return 1;
    }

    if(strsrch(args,"-r ") != -1){
        action = -1;
        args = replace_string(args,"-r ","");
    }
    if(strsrch(args,"-a ") != -1){
        action = 1;
        args = replace_string(args,"-a ","");
    }

    if(strsrch(args,"-d ") != -1){
        action = -2;
        args = replace_string(args,"-d ","");
    }
    if(strsrch(args,"-c ") != -1){
        action = 2;
        args = replace_string(args,"-c ","");
    }

    if(sscanf(args,"%s %s",groupname,dudename) == 2 && (abs(action) > 1) ){
        write("Too many arguments for this operation. Please try: help groupmod.");
        return 1;
    }

    if(sscanf(args,"%s %s",groupname,dudename) != 2 && (abs(action) < 2) ){
        write("Insufficient arguments for this operation. Please try: help groupmod.");
        return 1;
    }

    args = upper_case(args);

    top_array = ({});
    bottom_array = ({});

    unguarded( (: cp("/secure/cfg/groups.cfg","/secure/save/backup/groups."+time()) :) );
    config_file = read_file("/secure/cfg/groups.cfg");
    line_array = explode(config_file, "\n");
    if(groupname) str = upper_case(groupname);
    else str = args;
    if(dudename) dudename=(lower_case(dudename));
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
    cles = keys(GroupsMap);
    if(str == "ASSIST" || str == "SECURE" ) {
        if(!securep(this_player())){
            write("Only full admins may do this.");
            return 1;
        }
        reload_player = 1;
    }

    if(!GroupsMap[str] && (abs(action) < 2)){
        write("No such group.");
        return 1;
    }

    if(!action){
        write("I don't understand what you want. Please specify ");
        write("which action you want, -r, -a, -d, or -c");
        return 1;
    }

    if(action == 1){
        if(member_array(dudename,GroupsMap[str]) != -1){
            write("That person is already a member of that group.");
            return 1;
        }
        if(member_array(str,cles) != -1 && !sizeof(GroupsMap[str])) GroupsMap[str] = ({});
        GroupsMap[str] += ({ dudename });
        write("The group "+str+" now contains the following:\n "+
                implode(GroupsMap[str],":"));
    }

    if(action == -1){
        if(member_array(dudename,GroupsMap[str]) == -1){
            write("That person is already not a member of that group.");
            return 1;
        }
        if((str == "ASSIST" || str == "SECURE") && dudename == this_player()->GetKeyName() ) {
            write("This command will not let you remove yourself from the SECURE or ASSIST groups.");
            return 1;
        }
        GroupsMap[str] -= ({ dudename });
        write("The group "+str+" now contains the following:\n "+
                implode(GroupsMap[str],":"));
    }

    if(action == -2){
        if(member_array(args,cles) == -1){
            write("That group doesn't exist.");
            return 1;
        }
        if(args == "ASSIST" || args == "SECURE" ) {
            write("I'm not doing that. You'll have to hose your mud manually.");
            return 1;
        }
        map_delete(GroupsMap,args);
        write("Group deleted.");
    }

    if(action == 2){
        if(member_array(args,cles) != -1){
            write("That group already exists.");
            return 1;
        }
        GroupsMap[args] = ({});
        write("Group added.");
    }

    foreach(string key, mixed val in GroupsMap){
        new_config_file += "("+key+") "+implode(GroupsMap[key],":")+"\n";
    }
    unguarded( (: write_file("/secure/cfg/groups.cfg",new_config_file,1) :) );
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    if(str == "SNOOPER") SNOOP_D->SnoopClean();
    new_config_file = "";
    if(reload_player){
        object player = unguarded((: find_player(dudename) :));
        if(player){
            tell_player(player, "You've had your group membership changed "+
                    "in an important way.\n\nYour user object will be reloaded in "+
                    "a few moments.\n\n");
            RELOAD_D->eventReload(player, 3);
        }
    }
    dudename = "";
    return 1;
}

string GetHelp() {
    return ("Syntax: groupmod [-a|-r] <GROUP> <NAME> \n"
            "        groupmod [-c|-d] <GROUP>\n\n"
            "Modifies /secure/cfg/groups.cfg with the desired information.\n"
            "To create a group called MUDKIPZ: groupmod -c mudkipz\n"
            "To add Yotsuba as member of that group: "
            "groupmod -a mudkipz yotsuba");
}
