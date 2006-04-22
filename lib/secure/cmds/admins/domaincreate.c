#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string domain_path;
    string *subdirs = ({"room","weap","armor","npc","etc","adm","doors","meals","obj"});
    string *alphabet = ({",", "#", ";", "^", "&", "(", ")","\"","\'","\`","?", " " });
    subdirs += ({"txt","virtual"});
    alphabet += ({ "@", "!", "$", "%", "=", "{", "}", "[", "]", ":", "<", ">", "\*" });
    alphabet += ({ "\\", "\/", "\|" });

    if(!archp(this_player()) || this_player()->GetForced()){
	return "No.";
    }

    if(!args || args == "" || args == "/domains" || args == "/domains/" ||
      args == "domain"){
	write(this_object()->GetHelp());
	return 1;
    }

    if(first(args, 9) == "/domains/"){
	domain_path = args;
    }
    else domain_path = "/domains/"+args;

    if(sscanf(domain_path,"/domains/%s",args) != 1) return 0;
    if(last(args,1) == "/") args = truncate(args,1);

    foreach(string booboo in alphabet){
	args = replace_string(args, booboo, "_");
    }

    domain_path = "/domains/"+args;

    write("Domain path is: "+domain_path);

    if(directory_exists(domain_path)){
	write("That domain already exists.");
	return 1;
    }

    mkdir(domain_path);

    foreach(string dir in subdirs){
	string newdir = domain_path + "/" + dir;
	write("Creating: "+newdir);
	mkdir(newdir);
    }

    cp("/obj/room.c",domain_path+"/room/start.c");

    write("Ok.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: domaincreate DOMAIN\n\n"
      "This command creates a new directory in the /domains dir, "
      "and adds the appropriate subdirs needed by a domain."
      "\n\n"
      "See also: domainadmin, admintool");
}
