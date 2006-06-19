/*    /secure/cmds/adm/rid.c
 *    from the Dead Soulsr2 Object Library
 *    command to get rid of players 
 *    created by Descartes of Borg 951015
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;

static void EndRid(string str);

int cmd(string who) {
    object ob;
    string str, file;

    if(!archp(previous_object())) return 0;
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
	who = (string)ob->GetCapName();
	message("system", "You are being ridded from " + mud_name() + ".",
	  ob);
	if( !((int)ob->eventDestruct()) ) destruct(ob);
    }
    file = save_file(str) + __SAVE_EXTENSION__;
    if( rename(file, DIR_RID + "/" + str + __SAVE_EXTENSION__) ) {
	write("Rename failed, security violation logged.");
	log_file("security", "\n*****\nRid violation attempted\n"
	  "Target: " + who + "\nCall stack:\n" + 
	  sprintf("%O\n", previous_object(-1)));
	return 1;
    }
    write("Enter reason for ridding " + who + ".");
    file = DIR_TMP + "/" + (string)this_player()->GetKeyName();
    rm(file);
    this_player()->eventEdit(file, (: EndRid, who :));
    return 1;
}

static void EndRid(string who) {
    string file, str;

    file = DIR_TMP + "/" + (string)this_player()->GetKeyName();
    if( !(str = read_file(file)) ) str = "No comment.\n";
    log_file("rid", "\n" + who + " by " + (string)this_player()->GetCapName() +
      "\n" + str + "\n");
    write(who + " has been ridded from " + mud_name() + ".");
    PLAYERS_D->RemoveUser(lower_case(who));
}

void help() {
    write( @EndText
Syntax: rid <name>
Effect: Deletes, nukes, wipes out and annhilates unwanted player <name>
See also: demote, promote, sponsor
EndText
    );
}

