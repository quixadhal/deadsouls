/*    /secure/cmds/creator/update.c
 *    from the Dead Souls LPC Library
 *    destroys an object then reloads it
 *    created by Descartes of Borg 950330
 *    - added recursive update option (blitz 960711)
 *    Version: @(#) update.c 1.2@(#)
 *    Last modified: 96/12/17
 */
 
#include <lib.h>
#include <rooms.h>
 
inherit LIB_DAEMON;
inherit LIB_HELP;

#define U_RECURSIVE       (1 << 1)
#define U_INTERACTIVE     (1 << 2)
 
static void eventUpdate(string args, int flags);

static void create() {
    SetHelp("Syntax: <update [-r] [file list]>\n"
            "Destructs the master copy of the file named "
            "and then attempts to reload a new version "
            "of it.\n"
            "The -r flag attempts to update all "
            "files in the target's inheritance tree.\n");
}
 
mixed cmd(string args) {
    object *obs, ob;
    string *files, *tmpfiles;
    mixed tmp;
    string file;
    int i, flags;
 
    if( sizeof(args) ) {
        string str = args;
        args = "";
        foreach(string foo in explode(str, " ")) {
            switch(foo) {
                case "-r" : flags |= U_RECURSIVE; break;
                case "-e" : flags |= U_INTERACTIVE; break;
                default: args += " " + foo;
            }   
        }
    }
    if( args == "" || !args ) {
        ob = environment(this_player());
        if( !ob ) return "You have no environment.";
        file = base_name(ob);
        this_player()->eventPrint("Updating environment");
        obs = filter(all_inventory(ob), (: userp :));
        if( sizeof(obs) ) obs->eventMove(ROOM_VOID);
        if( !eventUpdate(base_name(ob), flags) ) {
            obs->eventPrint("You are thrown into the void as your "
                            "surroundings violently destruct.");
            return "Error in reloading environment.";
        }
        obs = filter(obs, (: $1 :));
        if( sizeof(obs) ) obs->eventMove(file);
        return 1;
    }
    tmpfiles = map(explode(args, " "),
                   function(string x) {
                     string tmp = (string)this_player()->query_cwd();
                     if( x[<2..] != ".c" ) x = x + ".c";
                     return absolute_path(tmp, x);
                   });
    tmpfiles = map(tmpfiles,
                   (: ((file_size($1) == -2) ?
                      (($1[<1] == '/') ? ($1 + "*.c") : ($1 + "/*.c")) : $1)
                   :));
    i = sizeof(tmpfiles);
    files = ({});
    while(i--) {
      if( sizeof(tmp = (string *)this_player()->wild_card(tmpfiles[i])) )
        files += tmp;
        else this_player()->eventPrint(tmpfiles[i] + ": File not found.");
    }
    i = sizeof(files);
    while(i--) eventUpdate(files[i], flags);
    return 1;
}
 
static int eventUpdate(string args, int flags) {
    object ob;
    string tmp;
 
    if( flags & U_RECURSIVE ) {
        string *ancestors;
        int i;

        if( !eventUpdate(args, flags ^ U_RECURSIVE) ) return 0;
        if( !(ob = find_object(args)) ) return 0;
        ancestors = deep_inherit_list(ob);
        this_player()->eventPrint("(%^CYAN%^Recursive "
                                  "update: " + args + "%^RESET%^)\n");
        i = sizeof(ancestors);
        while(i--) if( !eventUpdate(ancestors[i], flags ^ U_RECURSIVE) ) {
            this_player()->eventPrint("Recursive update failed.");
            return 0;
        }        
    }
    if( args[<2..] == ".c" ) args = args[0..<3];
    if( ob = find_object(args) ) {
        if( tmp = catch( ob->eventDestruct()) )
          this_player()->eventPrint(args + ": error in eventDestruct()");
        if( ob ) destruct(ob);
        if( ob )
          this_player()->eventPrint(args + ": Failed to destruct old object.");
    }
    if( args == base_name(this_object()) ) {
        this_player()->eventPrint("Cannot reload update after destruct.\n"
                                  "It will be reloaded at next reference.");
        return 0;
    }
    tmp = catch(call_other(args, "???"));
    if( !tmp ) {
         this_player()->eventPrint(args + ": Ok");
         return 1;
    } else this_player()->eventPrint(args + ": Error in update\n" + tmp);
    return 0;
}

