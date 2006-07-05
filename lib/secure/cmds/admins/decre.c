/*    /secure/cmds/adm/decre.c 
 *    from the Dead Souls LPC Library
 *    turns a creator into a player
 *    created by Descartes of Borg 950323
 */

#include <lib.h>
#include <privs.h>
#include <rooms.h>
#include <daemons.h>

inherit LIB_DAEMON;

string PlayerName;

mixed cmd(string args) {
    object ob, player_ob;
    object *inv;
    string nom, file;

    if( !((int)master()->valid_apply(({ PRIV_ASSIST, PRIV_SECURE, LIB_CONNECT }))) )
	error("Illegal decre attempt: "+get_stack()+" "+identify(previous_object(-1)));

    if( args == "" || !stringp(args) ) 
	return "Who do you want to make a player?";
    nom = convert_name(args);
    if( !user_exists(nom) ) return capitalize(nom) + " is not a member of " +
	possessive_noun(mud_name()) + " reality.";
    if( !strsrch(file = save_file(nom), DIR_PLAYERS) )
	return "You cannot make "+capitalize(args)+" a player.";

    if(!ob = find_player(nom)){
	PLAYERS_D->RemovePendingEncre(lower_case(nom));
	PLAYERS_D->AddPendingDecre(lower_case(nom));
	write(capitalize(nom)+" will be demoted on their next login.");
	return 1;
    }

    if(ob == this_player()){
	return "Nonsense.";
    }

    if( file_size(DIR_PLAYERS+"/"+nom[0..0]) != -2) 
	mkdir(DIR_PLAYERS+"/"+nom[0..0]);
    if(rename(file+__SAVE_EXTENSION__, DIR_PLAYERS+"/"+nom[0..0]+"/"+nom+__SAVE_EXTENSION__))
	return "You failed due to lack of write access to "+DIR_PLAYERS+".";
    PLAYERS_D->eventDecre(lower_case(nom));

    if( ob = find_player(nom) ) {
	PlayerName = nom;
	inv = deep_inventory(ob);
	ob->eventMove(ROOM_FURNACE);
	if(sizeof(inv)) inv->eventMove(ROOM_FURNACE);
	catch(player_ob = (object)master()->player_object(nom));
	PlayerName = 0;
	if( !player_ob ) {
	    message("system", "Failed to create a player object.", 
	      this_player());
	    message("system", "Please log out and log back in.", ob);
	    return 1;
	}
	exec(player_ob, ob);
	inv=deep_inventory(ob);
	if(sizeof(inv)) inv->eventMove(ROOM_FURNACE);
	player_ob->Setup();
	ob->eventDestruct();
	inv=deep_inventory(player_ob);
	if(sizeof(inv)) inv->eventMove(ROOM_FURNACE);
	message("system", "You are now a player.", player_ob);
	message("system", (string)player_ob->GetName() + " is now a player!",
	  this_player());
	if( file_size(file+__SAVE_EXTENSION__) > -1 ) rm(file+__SAVE_EXTENSION__);
    }
    player_ob->eventMove(ROOM_START);
    return 1;
}

string GetKeyName() { return PlayerName; }
void help() {
    message("help",
      "Syntax: decre <person>\n\n"
      "Demotes the specified creator to player status. "
      "If the target is not "
      "logged in, they will be made a player when "
      "they next log in."
      "\n\n"
      "See also: encre, rid", this_player()
    );
}

