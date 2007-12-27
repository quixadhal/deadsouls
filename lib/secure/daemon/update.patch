#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
mapping EventsMap = ([]);
mixed *event_funs = ({});

static void eventUpdate(){
    object remote;
    string a,b;
    string cpw,spw;
    string x = read_file("/secure/daemon/imc2.c");
    int y = sscanf(x,"%s#define CLIENT_PW \"%s\"%s",a,cpw,b);
    string config_file = read_file("/secure/include/config.h");
    string newfile = "#include <lib.h>\n";
    y = sscanf(x,"%s#define SERVER_PW \"%s\"%s",a,spw,b);
    newfile += "\n";
    newfile += "inherit LIB_DAEMON;\n";
    newfile += "\n";
    newfile += "static void eventUpdate(){}\n";
    newfile += "\n";
    newfile += "static void create(){\n";
    newfile += "    daemon::create();\n";
    newfile += "    call_out((: eventUpdate :), 60);\n";
    newfile += "}\n";

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

        if(!grepp(config_file, "IRN_PASSWORD2"))
            config_file = append_line(config_file,"#define IRN_PASSWORD",
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

        tc("config_file: "+config_file);

        write_file("/secure/include/config.h", config_file+"\n", 1);
    }

    rm("/cmds/players/where.c");
    rm("/domains/Praxis/obj/mon/execution.c");
    rm("/domains/campus/txt/moochers.txt");
    rm("/secure/save/players/g/guest.o");

    remote = load_object("/secure/cmds/admins/removeemote");
    if(remote) remote->cmd("roll");

    reload(EVENTS_D);
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

    load_object("/secure/cmds/admins/removeraces")->cmd();
    load_object("/secure/cmds/admins/addraces")->cmd();

    write_file("/secure/daemon/update.c",newfile,1);
    if(file_exists("/secure/daemon/imc2_new.c")){
        rename("/secure/daemon/imc2.c","/secure/save/backup/imc2_old.c");
        rename("/secure/daemon/imc2_new.c", "/secure/daemon/imc2.c");
    }

    CLASSES_D->RemoveClass("thief");
    CLASSES_D->AddClass("/secure/cfg/classes/thief");

    write_file("/secure/daemon/update.c",newfile,1);
}

static void create() {
    daemon::create();
    call_out((: eventUpdate :), 1);
}
