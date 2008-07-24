#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    object env = environment(this_player());
    int err;
    mixed dest;
    if(!env){
        write("There are no exits here.");
        return 1;
    }
    if(!str){
        write("Please specify an exit.");
        return 1;
    } 
    dest = env->GetExit(str);
    if(!dest) dest = env->GetEnter(str);
    if(!dest || !stringp(dest)){
        write("It appears that you can't noclip in that direction.");
        return 1;
    }
    err = catch( dest = load_object(dest) );
    if(err){
        write("There seems to be a problem with that direction.");
        write("You remain where you are.");
        return 1;
    }
    this_player()->eventMoveLiving(dest);
    if(!environment(this_player()) || environment(this_player()) != dest){
        write("It appears that you are unable to noclip there.");
    }
    return 1;
}

void help()
{
    write("Syntax: noclip <direction>\n\nAllows you to walk through doors and other obstacles, under most circumstances.\n\nSee also: go, swim, fly, crawl");
}
