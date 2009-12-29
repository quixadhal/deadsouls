#include <lib.h>
#include <save.h>
#include <daemons.h>

inherit LIB_DAEMON;
mapping mudlist;
string banned, unbanned;
string blacklisted, unblacklisted;
string *allmuds, *banned_arr, *unbanned_arr;
string *blacklisted_arr, *unblacklisted_arr;

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

    if(!router && (args != "on" && args != "online")){
        write("Router is not loaded. First try: mudconfig router enable");
        return 1;
    }

    mudlist = router->query_mudinfo();
    allmuds = keys(mudlist);

    if(!args || args == ""){
        args = "show";
    }

    if(args == "show"){
        write("Router config:");
        router->get_info();
        return 1;
    }

    if(!router){
        write("Router is not loaded.");
        return 1;
    }

    scannedp = sscanf(args,"%s %s", arg1, arg2);

    if(args == "mudlist" || arg1 == "mudlist"){
        mapping info;
        string *list;
        mapping borg;
        string mud, tempy;
        int all = 0;
        int ipsort = 0;
        int fdsort = 0;
        if(sizeof(arg2)) tempy = match_mud_name(arg2);
        if(sizeof(tempy)) arg2 = tempy;
        if(arg2 && !strsrch(arg2,"-i")){
            ipsort = 1;
            arg2 = trim(arg2[2..]);
        }
        if(arg2 && !strsrch(arg2,"-f")){
            fdsort = 1;
            arg2 = trim(arg2[2..]);
        }

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
                if( lower_case(arg2) == lower_case(tmpstr) ) {
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
                if( all == 1 || (info["restart_delay"] && info["restart_delay"] == -1) ) 
                    borg[mud] = info;
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

        if(fdsort){
            mapping fds = ROUTER_D->query_connected_fds();
            string *ret = ({});
            int i, quant = sizeof(socket_status());
            for(i = 0; i < quant; i++){
                if(last_string_element(socket_status(i)[3],".") == ROUTER_D->GetRouterPort() ){
                    string mudnamen;
                    if(fds[i]) mudnamen = fds[i];
                    else mudnamen = "UNKNOWN";
                    ret += ({ i+" "+mudnamen+" "+explode(socket_address(i)," ")[0] });
                }
            }
            ret = sort_array(ret,1);
            this_player()->eventPage(ret);
            return 1;
        }

        foreach(mud, info in borg){
            if(ipsort)
                list += ({ sprintf("%:-15s %:-6s %:-15s %:-18s",
                            info["ip"], itoa(info["player_port"]), replace_string(mud,"%^","%%^^"), info["base_mudlib"]) });

            else
                list += ({ sprintf("%:-15s %:-6s %:-15s %:-18s %s %d",
                            replace_string(mud,"%^","%%^^"), info["mud_type"], info["driver"], info["base_mudlib"], info["ip"], info["player_port"]) });
        }
        list = sort_array(list, 1);
        list = ({ replace_string(mud_name(),"%^","%%^^") + " recognizes " + consolidate(sizeof(borg), "a mud")+
                " matching your query: ", "" }) + list;
        this_player()->eventPage(list);
        return 1;
    }

    if(args == "reload" || args == "restart" || args == "reset"){
        object rsocket = find_object(RSOCKET_D);
        if(router){
            if(args == "reset"){
                router->clear();
                router->irn_clear();
            }
            router->SetList();
            router->eventDestruct();
            router = find_object(ROUTER_D);
            if(router) write("Router unload failed.");
            else write("Router unloaded from memory.");
            flush_messages();
        }
        if(args == "restart" || args == "reset"){
            if(rsocket) rsocket->eventDestruct();
            if(rsocket) destruct(rsocket);
            if(rsocket) write("%^RED%^BOLD%^Router socker not destructed.%^RESET%^");
            rsocket = load_object(RSOCKET_D);
            if(!rsocket) write("Could not reload router socket daemon.");
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

        router->RemoveBannedMud(arg2);
        write(arg2+" has been removed from the banned list.");
        return 1;
    }

    if(arg1 == "blacklist" || arg1 == "blacklisted"){
        if(sizeof(router->GetBlacklistedMuds())) blacklisted_arr = router->GetBlacklistedMuds();
        else blacklisted_arr = ({});
        if(!sizeof(blacklisted_arr)) blacklisted = "No blacklisted muds.";
        else blacklisted = implode(blacklisted_arr,", ");
        if(!arg2){
            write("Blacklisted muds: "+blacklisted);
            return 1;
        }
        tmp = match_mud_name(arg2, allmuds);
        if(!sizeof(tmp)){
            write("There is no such mud, but I am adding the name to the list.");
        }
        else arg2 = tmp;

        router->AddBlacklistedMud(arg2);
        write(arg2+" has been added to the blacklisted list.");
        return 1;
    }

    if(arg1 == "unblacklist" || arg1 == "unblacklisted"){
        if(sizeof(router->GetBlacklistedMuds())) blacklisted_arr = router->GetBlacklistedMuds();
        else blacklisted_arr = ({});
        if(!sizeof(blacklisted_arr)) unblacklisted = "No blacklisted muds.";
        else {
            blacklisted = implode(blacklisted_arr,", ");
            unblacklisted_arr = filter(keys(mudlist), (: member_array($1, blacklisted_arr) == -1 :) ) || ({});
            unblacklisted = implode(unblacklisted_arr,", ");
        }
        if(!arg2){
            write("Unblacklisted muds: "+unblacklisted);
            return 1;
        }

        router->RemoveBlacklistedMud(arg2);
        write(arg2+" has been removed from the blacklisted list.");
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
        string s1, s2, s3;
        if(arg2 && sizeof(arg2)){
            arg2 = reverse_string(arg2);
            sscanf(arg2,"%s %s %s", s3, s2, s1);
        }
        if(s1){
            s1 = reverse_string(s1);
            if(!strsrch(s1,"*")) true();
            else s1 = "*"+s1;
        }
        if(!s3){
            write("Syntax: router config NAME IP PORT");
            return 1;
        }
        if(s2) s2 = reverse_string(s2);
        s3 = reverse_string(s3);
        router->SetRouterName(s1);
        router->SetRouterIP(s2);
        router->SetRouterPort(s3);
        router->SetRouterList();
        write("Config complete. To activate, type: router reset");
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

    if(arg1 == "name"){
        if(!arg2 || !sizeof(arg2)){
            write("Syntax: router name NAME");
            return 1;
        }
        router->SetRouterName(arg2);
        write("Router name set.");
        return 1;
    }


    if(arg1 == "ip"){
        if(!arg2 || !sizeof(arg2)){
            write("Syntax: router ip ADDRESS");
            return 1;
        }
        router->SetRouterIP(arg2);
        write("Router ip set.");
        return 1;
    }


    if(arg1 == "port"){
        if(!arg2 || !sizeof(arg2)){
            write("Syntax: router port NUMBER");
            return 1;
        }
        router->SetRouterPort(arg2);
        write("Router port set.");
        return 1;
    }

    if(arg1 == "irn"){
        if(!arg2){
            write("Please specify an irn subcommand: enable, disable, "
                    "check, force.");
            return 1;
        }
        if(arg2 == "check"){
            ROUTER_D->irn_checkstat();
            write("irn status check issued.");
            return 1;
        }
        if(arg2 == "enable"){
            if(ROUTER_D->query_irn_enabled()){
                write("irn already enabled.");
                return 1;
            }
            ROUTER_D->toggle_irn(1);
            ROUTER_D->irn_checkstat(); 
            write("irn enabled.");
            return 1;
        }
        if(arg2 == "disable"){
            if(!(ROUTER_D->query_irn_enabled())){
                write("irn already disabled.");
                return 1;
            }
            ROUTER_D->toggle_irn(0);
            ROUTER_D->irn_clear();
            write("irn disabled.");
            return 1;
        } 
        if(arg2 == "force"){
            ROUTER_D->toggle_irn(1);
            ROUTER_D->irn_setup(1);
            write("irn forcibly reloaded.");
            return 1;
        }
    }

    write("Router command completed.");
    return 1;
}

string GetHelp(){
    return ("Syntax: router [subcommand [arg]]\n\n"
            "With no arguments, router status is displayed.\n"
            "Examples:\n" 
            "router reload : bounces the router without dropping connections\n"
            "router restart : bounces the router dropping all connections\n"
            "router reset : like restart but also clears all saved mud info\n"
            "router irn [enable|disable|check|force] : manages IRN subsystem\n"
            "router ban : lists banned muds\n"
            "router ban <mudname> : bans the mud with the name <mudname>\n"
            "router unban <mudname> : the opposite of banning\n"
            "router blacklist : lists blacklisted names and IP's\n"
            "router blacklist [name | ip address] : a harsher kind of ban\n"
            "router unblacklist [name | ip address] : the opposite of blacklisting\n"
            "router config <name> <ip> <port> : config the router in one line\n"
            "router port <portnum> : sets the router port\n"
            "router ip <ip number> : sets the ip address, e.g. 11.22.33.44\n"
            "router name <routername> : sets the router name\n"
            "router mudlist : display information of known muds\n" 
            "router mudlist -i : display muds sorted by ip\n" 
            "router mudlist -f : display connected muds sorted by file descriptor\n" 
            "\n"
            "To bring the router online or offline, use the "
            "mudconfig command.");
}
