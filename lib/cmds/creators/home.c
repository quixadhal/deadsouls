/*    /cmds/creator/home.c
 *    from the Dead Souls LPC Library
 *    command to take a creator home
 *    created by Descartes of Borg long ago (modified 950331)
 */

#include <lib.h>
#include <objects.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object ob, prev;
    string who;

    prev = environment(this_player());
    if( !str || str == "" ) who =(string)this_player()->GetKeyName();
    else who = lower_case(str);
    str = user_path(who);
    if( ob = (object)this_player()->query_property("workroom") ) {
        if(ob == prev) return "You twitch.";
        if( (int)this_player()->eventMove(ob) ) {
            this_player()->eventDescribeEnvironment(0);
            if(hiddenp(this_player()) || this_player()->GetInvis())
              return 1;
            message("mmout", (string)this_player()->GetMessage("home"), prev);
            message("mmin", (string)this_player()->GetMessage("telin"), ob, 
              ({ this_player() }));
            return 1;
        }
    }
    if( !ob ) {
      if( !(ob = load_object(str+"workroom")) &&
        str == user_path((string)this_player()->GetKeyName())) {
        ob = new(DIR_OBJ + "workroom");
        this_player()->set_property("workroom", ob);
      }
      else if( !(ob = load_object(str+"workroom")) ) {
        if(!(ob = find_player(who)) || !(ob=(object)ob->query_property("workroom")))
          return capitalize(who)+" has no active workroom.";
      }
      else {
        if( file_name(prev) == str+"workroom" )
          return "You twitch.";
        if( (int)this_player()->eventMove(ob) ) {
            this_player()->eventDescribeEnvironment(0);
            if(hiddenp(this_player()) || this_player()->GetInvis())
              return 1;
            message("mmout", (string)this_player()->GetMessage("home"), prev);
            message("mmin", (string)this_player()->GetMessage("telin"),
              environment(this_player()), ({ this_player() }));
            return 1;
        }
        return "You stay where you are.";
      }
    }
    if(ob == prev) return "You twitch.";
    if( (int)this_player()->eventMove(ob) ) {
        this_player()->eventDescribeEnvironment(0);
        if(hiddenp(this_player()) || this_player()->GetInvis()) return 1;
        message("mmout", (string)this_player()->GetMessage("home"), prev);
        message("mmin",(string)this_player()->GetMessage("telin"),ob,({this_player()}));
        return 1;
    }
    return "You stay where you are.";
}

void help() {
    message("help", "Syntax: <home>\n        <home [creator]>\n\n"
	    "Without arguments, this command will take you to your workroom.  "
	    "With arguments, it takes you to the workroom of the person "
	    "you specify.\n\n"
	    "See also: goto, trans", this_player());
}

