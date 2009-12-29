#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    foreach(string str in CLASSES_D->GetClasses()){
        CLASSES_D->RemoveClass(str);
    }
    write("Done.");
    return 1;
}

string GetHelp(){
    return ("Syntax: removeclasses\n\n"
            "Gets rid of ALL classes known by the classes daemon. "
            "Unless you want to be very sad later, you should "
            "add your classes with the addclasses command immediately. ");
}
