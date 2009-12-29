/*    /cmds/creator/home.c
 *    from the Dead Souls LPC Library
 *    command to take a creator home
 *    created by Descartes of Borg long ago (modified 950331)
 */

#include <lib.h>
#include <objects.h>
#include <daemons.h>

inherit LIB_DAEMON;
string arg;

mixed GoHome(string str) {
    object ob, prev;
    string who, room;

    prev = environment(this_player());
    if( !str || str == "" || !creatorp(this_player())){
        who = this_player()->GetKeyName();
    }
    else who = lower_case(str);
    if(!user_exists(who)) return "There's no such user.";
    room = PLAYERS_D->GetHomeRoom(who);
    if(!room || !strsrch(room, "/tmp/")){
        str = user_path(who, 1);
        if(!directory_exists(str)) return "That person has no home dir.";
        str = user_path(who, 1)+"workroom.c";
        arg = str;
        if(!unguarded((: file_exists(arg) :)))
            return capitalize(who)+" has no active workroom.";
    }
    else str = room;
    catch(ob = load_object(str));
    if(!ob){
        if(who != this_player()->GetKeyName())
            return "\n"+capitalize(who)+"'s workroom is broken.";
        return "\nYour workroom is broken.";
    }
    if(ob == prev)
        return "You twitch.";
    if(who == this_player()->GetKeyName())
        if( this_player()->eventMoveLiving(ob,"$N goes home.","$N returns home.") ) {
            return 1;
        }
    if(who != this_player()->GetKeyName())
        if( this_player()->eventMoveLiving(ob,"$N leaves to visit "+capitalize(who)+".",
                    "$N comes in for a visit.") ) {
            return 1;
        }
    return "You stay where you are.";
}

mixed cmd(string str){
    return unguarded( (: GoHome($(str)) :) );
}

string GetHelp(){
    return ("Syntax: home\n        home [creator]\n\n"
            "Without arguments, this command will take you to your workroom.  "
            "With arguments, it takes you to the workroom of the person "
            "you specify. Non-creator builders can only go to their own "
            "workroom.\n"
            "See also: goto, trans, homeroom");
}
