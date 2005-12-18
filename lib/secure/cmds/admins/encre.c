/*    /secure/cmd *    from the Dead Souls LPC Library
 *    turns a player into a creator
 *    created by Descartes of Borg 950322
 */

#include <lib.h>
#include <rooms.h>

inherit LIB_DAEMON;

string PlayerName;

mixed cmd(string args) {
    object ob, cre_ob, jeans, shirt, robe, hat, book, staff;
    string file, nom;
    string cdir, tdir, dir_line;

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
	make_workroom(cre_ob);
	cre_ob->eventForce("home");
	cre_ob->eventForce("cd");
	jeans = present("jeans",cre_ob);
	shirt = present("t-shirt",cre_ob);
	if(jeans) jeans->eventMove(ROOM_FURNACE);
	if(shirt) shirt->eventMove(ROOM_FURNACE);
        jeans = present("jeans",cre_ob);
        shirt = present("t-shirt",cre_ob);
        if(jeans) jeans->eventMove(ROOM_FURNACE);
        if(shirt) shirt->eventMove(ROOM_FURNACE);
        robe = new("/domains/default/armor/robe");
        if(robe) robe->eventMove(cre_ob);
        hat = new("/domains/default/armor/wizard_hat");
        if(hat) hat->eventMove(cre_ob);
        robe = present("robe",cre_ob);
        if(robe) cre_ob->eventForce("wear robe");
        hat  =  present("wizard hat",cre_ob);
        if(hat) cre_ob->eventForce("wear wizard hat");
        staff = new("/secure/obj/staff");
        if(staff) staff->eventMove(cre_ob);
        book = new("/domains/default/obj/manual");
        if(book) book->eventMove(cre_ob);
    }
    return 1;
}

string GetKeyName() { return PlayerName; }
