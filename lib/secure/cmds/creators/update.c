/*    /secure/cmds/creator/update.c
 *    from the Dead Souls LPC Library
 *    destroys an object then reloads it
 *    created by Descartes of Borg 950330
 *    - added recursive update option (blitz 960711)
 *    Version: @(#) update.c 1.2@(#)
 *    Last modified: 96/12/17
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_DAEMON;
inherit LIB_HELP;

#define U_RECURSIVE       (1 << 1)
#define U_INTERACTIVE     (1 << 2)
#define U_AUTOMATED       (1 << 3)

mapping LocationsMap = ([]);

varargs static int eventUpdate(string args, int flags, string virt);

static void CacheAndCarry(object *obs){
    if(!sizeof(obs)) return;
    foreach(object fellow in obs){
        string ubi = fellow->GetProperty("LastLocation");
        if(ubi) LocationsMap[fellow->GetKeyName()] = ubi;
        fellow->eventMove(ROOMS_D->GetVoid(fellow));
    }
}

static void ReturnAndRelease(object *dudes, string file){
    if(!sizeof(dudes)) return;
    if(!file) return;
    dudes->eventMove(file);
    foreach(object fellow in dudes){
        if(sizeof(LocationsMap[fellow->GetKeyName()])){
            fellow->SetProperty("LastLocation",LocationsMap[fellow->GetKeyName()]);
        }
    }
}

static void create() {
    SetHelp("Syntax: update [-r] [file list]\n\n"
            "Destructs the master copy of the file named "
            "and then attempts to reload a new version "
            "of it.\n"
            "The -r flag attempts to update all "
            "files in the target's inheritance tree.");
}

mixed cmd(string args) {
    object *obs, ob, mount;
    string *files, *tmpfiles;
    mixed tmp;
    string file, virt;
    int i, flags, err, ret;
    if( sizeof(args) ) {
        string str = args;
        args = "";
        foreach(string foo in explode(str, " ")) {
            if(!sizeof(foo)) continue;
            switch(foo) {
                case "-r" : flags |= U_RECURSIVE; break;
                case "-e" : flags |= U_INTERACTIVE; break;
                case "-a" : flags |= U_AUTOMATED; break;
                default: args += " " + foo;
            }   
        }
    }
    if( args == "" || !args ) {
        if(!this_player()) return "No player.";
        ob = room_environment(this_player());
        if( !ob ) return "You have no environment.";
        file = base_name(ob);
        if(ob->GetVirtual()){
            virt = path_prefix(file);
        }
        this_player()->eventPrint("Updating environment");
        obs = filter(all_inventory(ob), (: userp :));
        if(mount) obs += ({ mount });
        if( sizeof(obs) ) CacheAndCarry(obs);
        err = catch( ret = eventUpdate(file, flags, virt) );
        if( err || !ret ) {
            obs->eventPrint("You are thrown into the void as your "
                    "surroundings violently destruct.");
            return "Error in reloading environment.";
        }
        obs = filter(obs, (: $1 :));
        if( sizeof(obs) ) ReturnAndRelease(obs, file);
        return 1;
    }
    if(this_player()){
        tmpfiles = map(explode(args, " "),
                function(string x) {
                string tmp = this_player()->query_cwd();
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
            if( sizeof(tmp = wild_card(tmpfiles[i])) )
                files += tmp;
            else {
                this_player()->eventPrint(tmpfiles[i] + ": File not found.");
            }
        }
        i = sizeof(files);
        while(i--) eventUpdate(files[i], flags);
    }
    return 1;
}

varargs static int eventUpdate(string args, int flags, string virt) {
    object ob;
    string tmp;

    if( flags & U_RECURSIVE ) {
        string *ancestors;
        int i;

        if( !eventUpdate(args, flags ^ U_RECURSIVE) ) return 0;
        if( !(ob = find_object(args)) ) return 0;
        ancestors = deep_inherit_list(ob);
        if(this_player() && (flags & U_RECURSIVE) && !(flags & U_AUTOMATED))  
            this_player()->eventPrint("(%^CYAN%^Recursive "
                "update: " + args + "%^RESET%^)\n");
        i = sizeof(ancestors);
        while(i--) if( !eventUpdate(ancestors[i], flags ^ U_RECURSIVE) ) {
            if(this_player()) 
                this_player()->eventPrint("Recursive update failed.");
            return 0;
        }        
    }
    if( args[<2..] == ".c" ) args = args[0..<3];
    if(virt){
        object virtual = find_object(virt);
        if(virtual) virtual->eventDestruct();
        if(virtual) destruct(virtual);
    }
    ob = find_object(args);
    if(!ob) ob = load_object(args);
    if( ob ) {
        if(this_player() && inherits(LIB_ROOM,ob)){
            string coords = ROOMS_D->GetCoordinates(ob);
            MAP_D->RemoveCache(coords);
        }
        if( tmp = catch( ob->eventDestruct()) && this_player() )
            this_player()->eventPrint(args + ": error in eventDestruct()");
        if( ob ) destruct(ob);
        if( ob && this_player())
            this_player()->eventPrint(args + ": Failed to destruct old object.");
    }
    if( args == base_name(this_object()) && this_player() ) {
        this_player()->eventPrint("Cannot reload update after destruct.\n"
                "It will be reloaded at next reference.");
        return 0;
    }
    if(!sizeof(args)){
        write("Error updating. Mojo meditation 8675309.");
        return 1;
    }
    tmp = catch(call_other(args, "???"));
    if(this_player() && !(flags & U_AUTOMATED) ){
        if( !tmp ) 
            this_player()->eventPrint(args + ": Ok");
        else this_player()->eventPrint(args + ": Error in update\n" + tmp);
    }
    return 1;
}
