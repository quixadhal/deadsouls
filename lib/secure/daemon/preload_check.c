#include <lib.h>
#include <daemons.h>
#include <cfg.h>

inherit LIB_DAEMON;

string *preloads;

static void checkPreloads(){
    foreach(string daemon in preloads){
        if(!find_object(daemon) && daemon != INTERMUD_D && daemon != IMC2_D &&
                daemon != AUTOEXEC_D) {
            update(daemon);
        }
    }
    call_out( (: checkPreloads :) , 30);
}

static void create() {
    daemon::create();
    SetNoClean(1);
    preloads = filter(explode(read_file(CFG_PRELOAD),"\n"), (: !grepp($1,"#") :) );
    call_out( (: checkPreloads :), 30);
}
