/*    /secure/cmds/adm/decre.c 
 *    from the Dead Souls LPC Library
 *    turns a creator into a player
 *    created by Descartes of Borg 950323
 */

#include <lib.h>
#include <privs.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_DAEMON;

string home_dir, PlayerName;
object ob, player_ob;

mixed cmd(string args) {
    object *inv, *purge_array;
    string nom, file;

    if( !(master()->valid_apply(({ PRIV_ASSIST, PRIV_SECURE, LIB_CONNECT }))) )
        error("Illegal decre attempt: "+get_stack()+" "+identify(previous_object(-1)));

    ob = 0;
    player_ob = 0;

    if( args == "" || !stringp(args) ) 
        return "Who do you want to make a player?";
    nom = convert_name(args);
    if( !user_exists(nom) ) return capitalize(nom) + " is not a member of " +
        possessive_noun(mud_name()) + " reality.";
    WEB_SESSIONS_D->EndSession(lower_case(nom));
    if( !strsrch(file = player_save_file(nom), DIR_PLAYERS) )
        return "You cannot make "+capitalize(args)+" a player.";

    if(!ob = find_player(nom)){
        if(nom == this_player()->GetKeyName()){
            return "I've no idea how you've managed this, but \"no\".";
        }
        PLAYERS_D->RemovePendingEncre(lower_case(nom));
        PLAYERS_D->AddPendingDecre(lower_case(nom));
        write(capitalize(nom)+" will be demoted on their next login.");
        return 1;
    }
    else {
        mixed attrape;
        if(ob == this_player() || securep(ob)){
            return "Nonsense.";
        }
        home_dir = homedir(ob);
        write("You decre "+capitalize(nom)+".");
        PlayerName = nom;

        //Try to remove inventory and move the guy to the pod
        //
        ob->eventMove(ROOM_POD);
        inv = deep_inventory(ob);
        if(sizeof(inv))
            foreach(object thing in inv){
                if(thing) {
                    thing->eventMove(ROOM_FURNACE);
                }
            }	//Save the user to sync its state with his inventory
        unguarded( (: ob->save_player(ob->GetKeyName()) :) );

        //Move the user file to the player dir
        //
        if( file_size(DIR_PLAYERS+"/"+nom[0..0]) != -2) 
            mkdir(DIR_PLAYERS+"/"+nom[0..0]);
        if(rename(file, save_file(DIR_PLAYERS+"/"+nom[0..0]+"/"+nom))) 
            return "You failed due to lack of write access to "+DIR_PLAYERS+".";
        //Remove their homedir, save it to a backup dir.
        if(home_dir && directory_exists(home_dir))
            rename(home_dir,"/secure/save/decre/"+nom+"."+timestamp());
        //Remove their name from the creator list in the player daemon, add it to the player list.
        PLAYERS_D->eventDecre(lower_case(nom));

        //Try to create the new player object
        //
        attrape = catch(player_ob = (object)master()->player_object(nom));
        PlayerName = 0;
        if( attrape || !player_ob ) {
            message("system", "\nFailed to create a player object.", 
                    this_player());
            message("system", "Please log out and log back in.", ob);
            return 1;
        }

        //Do the decre
        //
        exec(player_ob, ob);
        player_ob->Setup();

        //Remove creator channels and such
        //
        foreach(string channel in player_ob->GetChannels())
            player_ob->RemoveChannel(channel);
        if( avatarp(player_ob) ) player_ob->AddChannel(({ "avatar" }));
        if( high_mortalp(player_ob) ) player_ob->AddChannel( ({ "hm" }) );
        if( newbiep(player_ob) ) player_ob->AddChannel( ({ "newbie" }) );
        player_ob->AddChannel( ({ "gossip" }) );
        if( councilp(player_ob) ) player_ob->AddChannel( ({ "council" }) );
        player_ob->AddChannel(player_ob->GetClass());

        //Destroy any objects loaded from their homedir
        //
        purge_array = filter(objects(), (: !strsrch(base_name($1), home_dir) :) );
        foreach(object tainted in purge_array){
            if(clonep(tainted)){
                tainted->eventMove(ROOM_FURNACE);
                purge_array -= ({ tainted });
            }
        }
        foreach(object tainted in purge_array){
            tainted->eventDestruct();
        }
    }
    call_out( (: ob->eventMove(ROOM_FURNACE) :), 1 );
    player_ob->eventMoveLiving(ROOM_START);
    player_ob->SetLoginSite(ROOM_START);
    unguarded( (: player_ob->save_player(player_ob->GetKeyName()) :) );
    message("system", "You are now a player.", player_ob);
    message("system", player_ob->GetName() + " is now a player!",
            this_player());
    return 1;
}

string GetKeyName() { return PlayerName; }

string GetHelp(){
    return ("Syntax: decre <person>\n\n"
            "Demotes the specified creator to player status. "
            "If the target is not "
            "logged in, they will be made a player when "
            "they next log in.\n"
            "See also: encre, rid");
}
