/*    /secure/cmd *    from the Dead Souls LPC Library
 *    turns a player into a creator
 *    created by Descartes of Borg 950322
 */

#include <lib.h>

inherit LIB_DAEMON;

string PlayerName;

mixed cmd(string args) {
    object ob, cre_ob;
    string nom, file;

    if( args == "" || !stringp(args) ) 
      return "Who do you want to make a creator?";
    nom = convert_name(args);
    if( !user_exists(nom) ) return capitalize(nom) + " is not a member of " +
      possessive_noun(mud_name()) + " reality.";
    if( !strsrch(file = save_file(nom), DIR_CRES) )
      return "You cannot make "+capitalize(args)+" a creator.";
    if( file_size(DIR_CRES+"/"+nom[0..0]) != -2) mkdir(DIR_CRES+"/"+nom[0..0]);
    if(rename(file+__SAVE_EXTENSION__, DIR_CRES+"/"+nom[0..0]+"/"+nom+__SAVE_EXTENSION__))
      return "You failed due to lack of write access to "+DIR_CRES+".";
    mkdir(user_path(nom));
    if( ob = find_player(nom) ) {
        PlayerName = nom;
        catch(cre_ob = (object)master()->player_object(nom));
        PlayerName = 0;
        if( !cre_ob ) {
	    message("system", "Failed to create a cre object.", this_player());
	    message("system", "Please log out and log back in.", ob);
	    return 1;
	}
	exec(cre_ob, ob);
	all_inventory(ob)->eventMove(cre_ob);
	cre_ob->Setup();
	ob->eventDestruct();
	message("system", "You are now a creator.", cre_ob);
	message("shout", (string)cre_ob->GetName() + " is now a creator!",
          users(), ({ this_player(), cre_ob }));
	if( file_size(file+__SAVE_EXTENSION__) > -1 ) rm(file+__SAVE_EXTENSION__);
    }
    message("system", "You make "+capitalize(args) + " a creator.", this_player());
    return 1;
}

string GetKeyName() { return PlayerName; }
