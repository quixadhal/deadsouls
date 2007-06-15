#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

static void eventUpdate(){
    int ok = 0;
    string config_file = read_file("/secure/include/config.h");
    string newfile = "#include <lib.h>\n";
    newfile += "\n";
    newfile += "inherit LIB_DAEMON;\n";
    newfile += "\n";
    newfile += "static void eventUpdate(){}\n";
    newfile += "\n";
    newfile += "static void create(){\n";
    newfile += "    daemon::create();\n";
    newfile += "    call_out((: eventUpdate :), 60);\n";
    newfile += "}\n";

    tc("Performing update tasks...");

    reload(EVENTS_D);
    foreach(mixed key, mixed val in EVENTS_D->GetEvents()){
	if(val["function"] == "ReadDir") ok = 1;
    }
    if(!ok)
	EVENTS_D->AddEvent("/secure/daemon/file","/secure/daemon/file","ReadDir",({ }),90000,1);
    rm("/secure/sefun/dump_socket_status.c");
    reload("/secure/sefun/sockets");
    reload("/secure/sefun/sefun");
    reload("/secure/daemon/master");

    tc("Doing race stuff...");

    load_object("/secure/cmds/admins/removeraces")->cmd();
    load_object("/secure/cmds/admins/addraces")->cmd();

    tc("Almost done...");
    write_file("/secure/daemon/update.c",newfile,1);
    tc("Update tasks complete.");
}

static void create() {
    daemon::create();
    call_out((: eventUpdate :), 60);
}
