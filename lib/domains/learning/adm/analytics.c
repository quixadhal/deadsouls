#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;
static string SaveFile;

static void create() {
    daemon::create();
    SaveFile = save_file("/domains/learning/save/analytics");
    SetSaveFile(SaveFile);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    if(file_exists(SaveFile)) RestoreObject(SaveFile);
}

int eventDestruct(){
    SaveObject(SaveFile, 1);
    return ::eventDestruct();
}

