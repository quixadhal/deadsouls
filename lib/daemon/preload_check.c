#include <lib.h>
#include <cfg.h>

inherit LIB_DAEMON;

string *preloads;

static void checkPreloads(){
    foreach(string daemon in preloads){
	if(!find_object(daemon)) update(daemon);
	//if(!find_object(daemon)) tc("can't find: "+daemon,"red");
	//else tc("found: "+daemon,"green");
    }
    call_out((: checkPreloads :), 300);
}

static void create() {
    daemon::create();
    SetNoClean(1);
    preloads = filter(explode(read_file(CFG_PRELOAD),"\n"), (: !grepp($1,"#") :) );
    call_out((: checkPreloads :), 300);
}

