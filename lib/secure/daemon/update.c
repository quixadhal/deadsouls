#include <lib.h>
#include <daemons.h>
#include <save.h>

inherit LIB_DAEMON;
mapping EventsMap = ([]);
mixed *event_funs = ({});

int check_function(string str){
    if(member_array(str,MASTER_D->GetEfuns()) != -1) return 1;
    return 0;
}

int make_empties(){
    string *empties = ({
      "/cmds/hm",
      "/cmds/builders",
      "/secure/cmds/common",
      "/secure/save/postal",
      "/secure/save/binaries",
      "/secure/save/decre",
      "/secure/save/players",
      "/secure/save/letters",
      "/secure/save/backup",
      "/secure/save/creators",
      "/secure/save/suicide",
      "/secure/save/rid",
      "/secure/save/votes",
      "/secure/upgrades/txt",
      "/www/doc",
      "/www/logs",
      "/doc/lpc/advanced",
      "/doc/tmp",
      "/doc/help/avatars",
      "/doc/help/hm",
      "/doc/help/religion",
      "/doc/help/law",
      "/doc/faq",
      "/log/author_stats",
      "/realms/template/tmp",
      "/realms/template/log",
      "/realms/template/area/etc",
      "/realms/template/area/meals",
      "/realms/template/area/doors",
      "/domains/town/save",
      "/domains/town/virtual/forest",
      "/domains/town/virtual/sub",
      "/domains/town/virtual/surface",
      "/domains/default/save",
      "/domains/default/virtual/sky",
      "/domains/default/virtual/arena",
      "/domains/Ylsrim/save",
      "/domains/Ylsrim/virtual/desert",
      "/verbs/spells",
      "/verbs/undead",
      "/secure/log/intermud",
      "/secure/log/network",
      "/domains/default/save",
      "/domains/town/save",
      "/open",
      "/powers",
      "/powers/spells",
      "/powers/prayers",
      "/powers/feats",
      "/powers/psionics",
      "/powers/trades",
      "/secure/log",
      "/secure/log/adm",
      "/secure/log/bak",
      "/secure/log/intermud",
      "/secure/log/network",
    });

    foreach(string dir in empties){
        catch( mkdir(dir) );
    }
    return 1;
}

varargs static void eventUpdate(object whom){
    int err;
    object remote;
    string a,b;
    string cpw,spw;
    string *file_arr;
    string x = read_file("/secure/daemon/imc2.c");
    int y = sscanf(x,"%s#define IMC2_CLIENT_PW \"%s\"%s",a,cpw,b);
    string config_file = read_file("/secure/include/config.h");
    string newfile = "#include <lib.h>\n";
    y = sscanf(x,"%s#define IMC2_SERVER_PW \"%s\"%s",a,spw,b);
    newfile += "\n";
    newfile += "inherit LIB_DAEMON;\n";
    newfile += "\n";
    newfile += "static void eventUpdate(){}\n";
    newfile += "\n";
    newfile += "static void create(){\n";
    newfile += "    daemon::create();\n";
    newfile += "    call_out((: eventUpdate :), 60);\n";
    newfile += "}\n";
    write_file("/secure/daemon/update.c",newfile,1);

    if(find_object(RACES_D))
        find_object(RACES_D)->eventDestruct();
    rename(SAVE_RACES __SAVE_EXTENSION__, SAVE_RACES+"."+time());

    reload("/secure/sefun/pointers",0,1);
    reload("/secure/sefun/sefun",0,1);
    reload("/secure/daemon/master",0,1);

    if(sizeof(config_file)){

        if(!grepp(config_file, "DISABLE_IMC2"))
            config_file = append_line(config_file,"#define DISABLE_INTERMUD",
              "#define DISABLE_IMC2             0");

        if(!grepp(config_file, "PING_INTERVAL"))
            config_file = append_line(config_file,"#define PINGING_MUDS",
              "#define PING_INTERVAL            290");

        if(!grepp(config_file, "DESTRUCT_LOGGING"))
            config_file = append_line(config_file,"#define WEB_SOURCE",
              "#define DESTRUCT_LOGGING         0");

        if(!grepp(config_file, "ROUTER_NAME"))
            config_file = append_line(config_file,"#define DESTRUCT_LOGGING",
              "#define ROUTER_NAME              \"*"+mud_name()+"\"");

        if(!grepp(config_file, "ROUTER_TESTING"))
            config_file = append_line(config_file,"#define DESTRUCT_LOGGING",
              "#define ROUTER_TESTING           1");

        if(!grepp(config_file, "SEFUN_PLURALIZE"))
            config_file = append_line(config_file,"#define OLD_STYLE_PLURALS",
              "#define SEFUN_PLURALIZE          0");

        if(!grepp(config_file, "LOG_REMOTE_CHANS"))
            config_file = append_line(config_file,"#define DESTRUCT_LOGGING",
              "#define LOG_REMOTE_CHANS         0");

        if(!grepp(config_file, "LOG_LOCAL_CHANS"))
            config_file = append_line(config_file,"#define LOG_REMOTE_CHANS",
              "#define LOG_LOCAL_CHANS          1");

        if(!grepp(config_file, "IRN_PASSWORD"))
            config_file = append_line(config_file,"#define ROUTER_NAME",
              "#define IRN_PASSWORD             \""+mud_name()+"\"");

        if(!grepp(config_file, "IRN_PASSWORD1"))
            config_file = append_line(config_file,"#define IRN_PASSWORD",
              "#define IRN_PASSWORD1            \"bet\"");

        if(!grepp(config_file, "IRN_PASSWORD2"))
            config_file = append_line(config_file,"#define IRN_PASSWORD1",
              "#define IRN_PASSWORD2            \"bet\"");

        if(!grepp(config_file, "IRN_PASSWORD3"))
            config_file = append_line(config_file,"#define IRN_PASSWORD2",
              "#define IRN_PASSWORD3            \"gimel\"");

        if(!grepp(config_file, "IMC2_CLIENT_PW") && cpw)
            config_file = append_line(config_file,"#define LOG_LOCAL_CHANS",
              "#define IMC2_CLIENT_PW           \""+cpw+"\"");

        if(!grepp(config_file, "IMC2_SERVER_PW") && spw)
            config_file = append_line(config_file,"#define IMC2_CLIENT_PW",
              "#define IMC2_SERVER_PW           \""+spw+"\"");

        if(!grepp(config_file, "FAST_COMBAT"))
            config_file = append_line(config_file,"#define RETAIN_ON_QUIT",
              "#define FAST_COMBAT              0");

        if(!grepp(config_file, "ENABLE_CGI"))
            config_file = append_line(config_file,"#define IMC2_SERVER_PW",
              "#define ENABLE_CGI               0");

        if(!grepp(config_file, "WWW_DIR_LIST"))
            config_file = append_line(config_file,"#define ENABLE_CGI",
              "#define WWW_DIR_LIST             0");

        if(!grepp(config_file, "ENABLE_CREWEB"))
            config_file = append_line(config_file,"#define WWW_DIR_LIST",
              "#define ENABLE_CREWEB            0");

        if(!grepp(config_file, "CHANNEL_PIPES"))
            config_file = append_line(config_file,"#define RESTRICTED_INTERMUD",
              "#define CHANNEL_PIPES            0");

        if(!grepp(config_file, "F_TERMINAL_COLOR")){
            if(check_function("terminal_color"))
                config_file = append_line(config_file,"#define ENABLE_CREWEB",
                  "#define F_TERMINAL_COLOR         1");
            else
                config_file = append_line(config_file,"#define ENABLE_CREWEB",
                  "#define F_TERMINAL_COLOR         0");
        }

        if(!grepp(config_file, "CLASS_SELECTION"))
            config_file = append_line(config_file,"#define HUMANS_ONLY",
              "#define CLASS_SELECTION          0");

        if(!grepp(config_file, "SEVERABLE_LIMBS"))
            config_file = append_line(config_file,"#define HUMANS_ONLY",
              "#define SEVERABLE_LIMBS          1");

        write_file("/secure/include/config.h", config_file+"\n", 1);
    }

    rm("/secure/cmds/admins/addemote.c");
    rm("/secure/cmds/admins/removeemote.c");
    rm("/secure/cmds/admins/stupidemote.c");
    rm("/daemon/class.c");
    rm("/cmds/players/where.c");
    rm("/domains/Praxis/obj/mon/execution.c");
    rm("/domains/campus/txt/moochers.txt");
    rm("/secure/cfg/classes/priest");
    rm("/secure/sefun/distinct_array.c");
    rm("/secure/sefun/query_carrying.c");
    rm("/secure/sefun/singular_array.c");
    rm("/verbs/creators/add.c");
    rm("/verbs/creators/copy.c");
    rm("/verbs/creators/createfix.c");
    rm("/verbs/creators/delete.c");
    rm("/verbs/creators/dest.c");
    rm("/verbs/creators/initfix.c");
    rm("/verbs/creators/modify.c");
    rm("/verbs/creators/pulsecheck.c");
    rm("/verbs/creators/reload.c");
    rm("/secure/cmds/creators/create.c");
    rm("/secure/cmds/creators/home.c");
    rm("/secure/cmds/creators/grant.c");
    rm("/daemon/include/races.h");
    rm("/lib/verb.c");
    rm("/lib/include/verb.h");

    call_out("make_empties",0);

    err = catch( remote = load_object("/secure/cmds/admins/removeemote") );
    if(err || !remote) 
        catch( remote = load_object("/secure/cmds/creators/removeemote") );
    if(remote) remote->cmd("roll");

    reload(EVENTS_D,0,1);
    EventsMap = EVENTS_D->GetEvents();
    foreach(mixed key, mixed val in EventsMap){
        event_funs += ({ val["function"] });
    }
    if(member_array("ReadDir",event_funs) == -1)
        EVENTS_D->AddEvent("/secure/daemon/file","/secure/daemon/file","ReadDir",({ }),90000,1);
    if(member_array("ReadFuns",event_funs) == -1)
        EVENTS_D->AddEvent("/secure/daemon/function","/secure/daemon/function","ReadFuns",({ }),100000,1);
    if(member_array("RotateLogs",event_funs) == -1)
        EVENTS_D->AddEvent("/secure/daemon/log","/secure/daemon/log","RotateLogs",({ }),3600,1);

    if(file_exists("/secure/scripts/qcs_check.scr"))
        rename("/secure/scripts/qcs_check.scr", "/secure/scripts/qcs_check.txt");

    catch( load_object("/secure/cmds/admins/removeraces")->cmd());
    catch( load_object("/secure/cmds/admins/addraces")->cmd());

    newfile = read_file("/secure/cfg/read.cfg");
    newfile = replace_string(newfile,"(/log/secure)","(/log/secure/)");
    newfile = replace_string(newfile,"(/secure/log)","(/secure/log/)");
    newfile = replace_string(newfile,"(/log/router)","(/log/router/)");
    write_file("/secure/cfg/read.cfg",newfile,1);
    newfile = read_file("/secure/cfg/write.cfg");
    newfile = replace_string(newfile,"(/log/router)","(/log/router/)");
    newfile = replace_string(newfile,"(/log/secure)","(/log/secure/)");
    write_file("/secure/cfg/write.cfg",newfile,1); 
    newfile = read_file("/secure/cfg/groups.cfg");
    file_arr = explode(newfile,"\n");
    if(!grepp(newfile,"(BUILDER)")) file_arr += ({"(BUILDER) "});
    if(!grepp(newfile,"(TELNET)")) file_arr += ({"(TELNET) "});
    if(!grepp(newfile,"(EMOTES)")) file_arr += ({"(EMOTES) "});
    newfile = implode(file_arr,"\n");
    write_file("/secure/cfg/groups.cfg", newfile, 1);

    if(file_exists("/secure/daemon/imc2_new.c")){
        rename("/secure/daemon/imc2.c","/secure/save/backup/imc2_old.c");
        rename("/secure/daemon/imc2_new.c", "/secure/daemon/imc2.c");
    }

    catch( CLASSES_D->RemoveClass("thief") );
    catch( CLASSES_D->AddClass("/secure/cfg/classes/thief") );
    catch( CLASSES_D->RemoveClass("priest") );
    catch( CLASSES_D->AddClass("/secure/cfg/classes/cleric") );
    //tc("Done with classes...");

    catch( reload("/secure/daemon/master",0,1) );
    catch( reload("/secure/sefun/arrays",0,1) );
    catch( reload("/secure/sefun/sefun",0,1) );

    catch( reload("/domains/default/room/stargate_lab.c",0,1));
    catch( reload("/domains/town/virtual/space/1,1,1",0,1));
    catch( reload("/domains/town/virtual/bottom/33,100000",0,1));
    catch( reload("/domains/Praxis/square.c",0,1));
    catch( reload("/domains/Ylsrim/room/tower",0,1));
    catch( reload("/domains/campus/room/slab",0,1));
    update(RELOAD_D,0,1);

    if(whom){
        tell_player(whom,"Update daemon finished.");
        if(query_os_type() == "windows") tell_player(whom,"Rebooting now is a good idea.");
        else tell_player(whom,"Initiating warm boot.");
    }

    if(query_os_type() != "windows"){
        RELOAD_D->WarmBoot();
    }
}

static void create() {
    object whom;
    daemon::create();
    if(this_player()) whom = this_player();
    call_out((: eventUpdate :), 1, whom);
    if(whom){
        tell_player(whom,"Please stand by until you see the \"Update daemon finished.\" message.");
        tell_player(whom,"If you do not see it after a few seconds, you may need to restore "
          "your mud from backup.");
    }
}
