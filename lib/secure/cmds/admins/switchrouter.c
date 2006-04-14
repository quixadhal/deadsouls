#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string file;

mixed cmd(string args) {
    string ip, name;
    int port, octet1, octet2, octet3, octet4;

    if(!archp(this_player())) return "No.";

    find_object(INTERMUD_D)->eventDestruct();
    rm("/save/intermud.o");

    if(!args || args == ""){
	write("No argument provided. Reloading intermud daemon.");
	if( load_object(INTERMUD_D) ) write("Intermud daemon reloaded.");
	else write("Failed to reload intermud daemon.");
	return 1;
    }

    if(sscanf(args,"%s %s %d", name, ip, port) != 3 || port > 65534 ||
      sscanf(ip,"%d.%d.%d.%d", octet1, octet2, octet3, octet4) != 4){
	write("Invalid router reference. \nTry: switchrouter <ROUTERNAME> <NUMERICAL IP> <PORT>");
	write("For example: switchrouter yatmim 149.152.218.102 23");
	if( load_object(INTERMUD_D) ) write("Intermud daemon reloaded.");
	else write("Failed to reload intermud daemon.");
	return 1;
    }

    file = read_file(INTERMUD_D+".c");
    //write_file("/tmp/1.txt",file,1);

    file = replace_matching_line(file, "//Nameservers", "");

    //write_file("/tmp/2.txt",file,1);
    if(first(name,1) != "*") name = "*"+name;

    file = replace_matching_line(file, "Nameservers = ({",
      "    Nameservers = ({ ({ \""+name+"\", \""+ip+" "+port+"\" }) });", 1);

    //write_file("/tmp/3.txt",file,1);

    unguarded( (: write_file(INTERMUD_D+".c",file,1) :) );

    if( load_object(INTERMUD_D) ) write("Intermud daemon reloaded.");
    else write("Failed to reload intermud daemon.");

    write("Done.");
    return 1;
}


string GetHelp(string args) {
    return ("Syntax: switchrouter <NAME> <IP> <PORT>\n\n"
      "Stops the intermud daemon, changes the default intermud "
      "router to the ip and port you specify, wipes the previous "
      "router data, and reloads INTERMUD_D. Without arguments, "
      "this command will clear your intermud cache and reload "
      "the daemon. Known routers are:\n"
      "*gjs 198.144.203.194 9000 (The \"official\" intermud.org router)\n"
      "*yatmim 149.152.218.102 23 (The \"official\" Dead Souls router)\n"
      "The official current Dead Souls router's IP will always be "
      "available at:\n"
      "http://dead-souls.net/router.html"
      "\n\n"
      "See also: router, mudlist");
}
