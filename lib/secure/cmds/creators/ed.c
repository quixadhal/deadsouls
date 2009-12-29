/*    /secure/cmds/creator/ed.c
 *    from the Dead Souls LPC Library
 *    creator command to enter the editor
 *    created by Descartes of Borg 950406
 */

#include <lib.h>

inherit LIB_DAEMON;

int ed_callback(object ob){
    if(ob) ob->CheckCharmode();
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
    this_player()->eventEdit(args, (: ed_callback, this_player() :) );
    return 1;
}

string GetHelp() {
    return ("Syntax: ed [filename]\n\n"
            "Ye olde LP mud in-game text editor. It is a line-mode editor, "
            "meaning it is archaic, hard to use, and complimicated. "
            "But it's the "
            "one way to edit stuff in-game that works for sure. "
            "For a brief walkthrough:\n"
            "%^GREEN%^faq ed%^RESET%^\n"
            "For a lengthy doc:\n"
            "%^GREEN%^help editor%^RESET%^\n"    
            "For a web page FAQ, use your web browser to go to; http://dead-souls.net/editor.html\n\n"
            "See also: ced, creweb, qcs");
}
