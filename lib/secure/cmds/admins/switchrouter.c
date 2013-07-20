#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string file;

mixed cmd(string args) {
    string ip, name;
    int port, octet1, octet2, octet3, octet4;

    if(!archp(previous_object())) return "No.";

    if(!args || args == ""){
        object omud = find_object(INTERMUD_D);
        if(!omud){
            write("The intermud daemon is not loaded. Try: help switchrouter");
            write("It may also be disabled. Try: help mudconfig");
            return 1;
        }
        name = omud->GetNameservers()[0][0];
        ip = omud->GetNameservers()[0][1];
        if(!name || !ip){
            write("There appears to be something wrong with your intermud daemon config.");
            return 1;
        }
        write("Your current direct router connection is to "+name+" "+ip);
        if(sizeof(omud->GetNameservers()) > 1){
            write("NOTE: You have multiple routers configured. The array "+
                    "of routers you are connected to is: "+identify(omud->GetNameservers()));
        }
        return 1;
    }

    if(find_object(INTERMUD_D))find_object(INTERMUD_D)->eventClearVars();
    if(find_object(INTERMUD_D))find_object(INTERMUD_D)->eventDestruct();


    if(args == "reload"){
        write("Reloading intermud daemon.");
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

    file = replace_matching_line(file, "//Nameservers", "");

    if(first(name,1) != "*") name = "*"+name;

    file = replace_matching_line(file, "Nameservers = ({",
         "    Nameservers = ({ ({ \""+name+"\", \""+ip+" "+port+"\" }) });", 1);

            unguarded( (: write_file(INTERMUD_D+".c",file,1) :) );

            if( load_object(INTERMUD_D) ) write("Intermud daemon reloaded.");
            else write("Failed to reload intermud daemon.");

            write("Done.");
            return 1;
            }

            string GetHelp(){
            return ("Syntax: switchrouter <NAME> <IP> <PORT>\n\n"
                "Stops the intermud daemon, changes the default intermud "
                "router to the ip and port you specify, wipes the previous "
                "router data, and reloads INTERMUD_D. Without arguments, "
                "this command will clear your intermud cache and reload "
                "the daemon. Known routers are:\n"
                //"*gjs 198.144.203.194 9000 (The \"official\" intermud.org router)\n"
                //"*adsr 62.49.9.82 9000 (Arren's unmoderated router)\n"
                //"*yatmim 149.152.218.102 23 (The backup/alternate router for *dalet)\n"
                "*dalet 97.107.133.86 8787 (The \"official\" LPMuds.net router)\n"
                "*i4 204.209.44.3 8080 (The backup/alternate router for *dalet)\n"
                //"*wpr 195.242.99.94 8080 (An alternate run in Europe)\n"
                "The official current Dead Souls router's IP will always be "
                "available at:\n"
                "http://dead-souls.net/router.html"
                "\n\n"
                "See also: mudlist, mudconfig, http://intermud.org");
            }

