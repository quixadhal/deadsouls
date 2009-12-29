#include <lib.h>
#include <cfg.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd() {
    string *classes = get_dir(CFG_CLASSES+"/");
    foreach(string element in classes){
        string str = CFG_CLASSES+"/"+element;
        if(file_exists(str)) {
            catch( CLASSES_D->AddClass(str) );
        }
    }
    write("Done.");
    return 1;
}

string GetHelp() {
    return ("Syntax: addclasses\n\n"
            "Imports all the class files in CFG_CLASSES into  "
            "CLASSES_D. It may fail if all classes have not been "
            "removed prior to its use.");
}
