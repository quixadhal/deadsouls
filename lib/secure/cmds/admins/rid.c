/*    /secure/cmds/adm/rid.c
 *    from the Dead Souls Object Library
 *    command to get rid of players 
 *    created by Descartes of Borg 951015
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;

varargs static void EndRid(string str, string reason);

varargs int cmd(string who, string reason) {
    object ob = previous_object();
    string str, file, targetdir;

    if(!archp(ob) && base_name(ob) != PLAYERS_D) return 0;
    if( !who || who == "" ) {
        write("Rid whom?");
        return 1;
    }

    str = convert_name(who);
    who = capitalize(who);
    if( member_group(str, PRIV_SECURE) || member_group(str, PRIV_ASSIST) ){
        write("You must first remove this person from a secure group.");
        return 1;
    }

    if( !user_exists(str) ) {
        write("No such person: " + who + ".");
        return 1;
    }

    if( ob = find_player(str) ) {
        who = ob->GetCapName();
        message("system", "You are being ridded from " + mud_name() + ".",
                ob);
        if( !(ob->eventDestruct()) ) destruct(ob);
    }
    file = player_save_file(str);
    targetdir = DIR_RID + "/" + str[0..0] + "/" + str; 
    mkdir_recurse(targetdir);
    if( rename(file, save_file(targetdir + "/" + str + ".bak")) ) {
        write("Rename failed, security violation logged.");
        log_file("security", "\n*****\nRid violation attempted\n"
                "Target: " + who + "\nCall stack:\n" + 
                sprintf("%O\n", previous_object(-1)));
        return 1;
    }

    if(reason){
        EndRid(who, reason);
        return 1;
    }

    write("Enter reason for ridding " + who + ".");
    file = DIR_TMP + "/" + this_player()->GetKeyName();
    rm(file);
    this_player()->eventEdit(file, (: EndRid, who :));
    return 1;
}

varargs static void EndRid(string who, string reason) {
    string file, str, actor;
    object foo = previous_object();

    if(this_player()) actor = this_player()->GetCapName();
    else if(foo) actor = base_name(foo);
    else actor = "UNKNOWN";

    if(!reason){
        file = DIR_TMP + "/" + this_player()->GetKeyName();
        if( !(str = read_file(file)) ) str = "No comment.\n";
    }
    else str = reason;
    log_file("rid", "\n" + who + " by " + actor +
            "\n" + str + "\n");
    write(who + " has been ridded from " + mud_name() + ".");
    PLAYERS_D->RemovePendingEncre(lower_case(who));
    PLAYERS_D->RemoveUser(lower_case(who));
}

string GetHelp(){
    return "Syntax: rid <name>\n\n"
        "Deletes, nukes, wipes out and annhilates unwanted player <name>\n"
        "See also: demote, promote, sponsor";
}

