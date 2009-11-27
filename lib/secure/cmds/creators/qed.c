/*    /secure/cmds/creator/ed.c
 *    from the Dead Souls LPC Library
 *    creator command to enter the editor
 *    created by Descartes of Borg 950406
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

int UnMuff(object whom){
    if(!whom) whom = this_player();
    whom->eventPauseMessages(0);
    return 1;
}

mixed cmd(string args) {
    int x;

    if( args == "" || !args ) 
        args = user_path(this_player()->GetKeyName()) + "tmp.edit";
    if( this_player()->GetForced() ) {
        message("system", "Someone tried to force you to edit "+ args +"\n" +
                identify(previous_object(-1)), this_player());
        return 0;
    }

    if(!this_player()->GetProperty("EdWarned")){
        write("This is the first time you've used ed. If you get stuck, simply "
                "hit return a few times, then enter a dot on a blank line, hit return, "
                "then type Q, then enter. Then visit this page to learn more about "
                "using the ed editor: http://dead-souls.net/editor.html");
        this_player()->SetProperty("EdWarned", 1);
    }
    args = absolute_path( this_player()->query_cwd(), args );
    if( (x = file_size(args)) == -2 ) 
        return "You cannot edit a directory!";
    else if( x == -1 )
        message("editor", args + ", new file, starting in input mode.\n",
                this_player());
    else message("editor", args + ", " + x + " bytes\n", this_player());
    this_player()->eventPauseMessages(1,MSG_EDIT);
    this_player()->eventEdit(args, (: UnMuff(this_player()) :) );
    return 1;
}

string GetHelp() {
    return ("Syntax: qed [filename]\n\n"
            "This is the quiet version of ed. While editing, "
            "you will receive only editing data and some other limited "
            "messages. "
            "Once done editing, the game messages you missed will be "
            "displayed to you. Be careful where you use this command, for you "
            "may engage in combat and be killed without knowing about it "
            "until you complete your editing.\nSee also: ed, ced, qcs, creweb");
}
