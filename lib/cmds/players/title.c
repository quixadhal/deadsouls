/*    /cmds/player/title.c
 *    from the Dead Souls LPC Library
 *    allows someone to set their titles
 *    created by Descartes of Borg 950716
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

static void eventOrderTitles(string *titles, int maxi);
static void GetTitle(string which, string *titles, int maxi);
static void GetDest(string which, string *titles, int i, int maxi);

mixed cmd(string args) {
    string *titles;
    int maxi;

    if( creatorp(this_player()) || avatarp(this_player()) ) {
        if( !args || args == "" ) return "Change your title to what?";
        else args = this_player()->SetShort(args);
        message("system", "Title changed to: " + args, this_player());
        this_player()->save_player(this_player()->GetKeyName());
        update("/secure/daemon/finger");
        return 1;
    }
    if( !(maxi = sizeof(titles = this_player()->GetTitles())) ) 
        return "You are totally unaccomplished.";
    if( args == "1" || args == "2" ) {
        this_player()->SetTitleLength(to_int(args));
        this_player()->SetShort("foo");
        this_player()->eventPrint("Number of titles in your descriptions changed to " + args + ".");
        this_player()->save_player(this_player()->GetKeyName());
        update("/secure/daemon/finger");
        return 1;
    }
    if( maxi == 1 ) {
        message("system", "You have only one title: " + titles[0], 
                this_player());
        return 1;
    }
    else if( maxi == 2 ) {
        function f;

        f = function(string yn, string *titles) {
            yn = (yn || "n");
            if( yn != "y" ) {
                message("system", "Titles remain unchanged.", this_player());
                return;
            }
            this_player()->SetTitles( ({ titles[1], titles[0] }) );
            message("system", "Titles reversed.", this_player());
            this_player()->save_player(this_player()->GetKeyName());
            update("/secure/daemon/finger");
        };
        message("system", "You have the following titles:\n\t" +
                titles[0] + "\n\t" + titles[1], this_player());
        message("prompt", "Do you wish to reverse them? [n] ", this_player());
        input_to(f, titles);
        return 1;
    }
    else eventOrderTitles(titles, maxi);
    return 1;
}

static void eventOrderTitles(string *titles, int maxi) {
    int i;

    message("system", "You have the following titles:", this_player());
    for(i=0; i<maxi; i++)
        message("system", "\t" + (i+1) + " " + titles[i], this_player());
    message("prompt", "Move which (q to quit)? [" + (maxi) + "] ", 
            this_player());
    input_to((: GetTitle :), titles, maxi);
}

static void GetTitle(string which, string *titles, int maxi) {
    int x;

    if( which == "q" ) {
        this_player()->SetTitles(titles);
        message("system", "Done.", this_player());
        return;
    }
    else if( which == "" || !which ) which = "" + maxi;
    x = to_int(which);
    if( x < 1 || x > maxi ) {
        message("system", "Illegal title number.", this_player());
        eventOrderTitles(titles, maxi);
        return;
    }
    x--;
    message("prompt", "Move \"" + titles[x] + "\" to position? [1] ",
            this_player());
    input_to( (: GetDest :), titles, x, maxi );
}

static void GetDest(string which, string *titles, int i, int maxi) {
    string *tmp;
    int j, maxj, x;

    if( which == "q" ) {
        this_player()->SetTitles(titles);
        message("system", "Done.", this_player());
        this_player()->save_player(this_player()->GetKeyName());
        update("/secure/daemon/finger");
        return;
    }
    else if( which == "" || !which ) which = "1";
    x = to_int(which);
    if( x < 1 || x > maxi || x == i+1 ) {
        message("system", "Illegal title number.", this_player());
        eventOrderTitles(titles, maxi);
        return;
    }
    x--;
    tmp = allocate(maxj = sizeof(titles));
    for(j=0; j<maxj; j++) {
        if( j == x ) tmp[j] = titles[i];
        else if( (j < x && j < i) || (j > x && j > i) ) tmp[j] = titles[j];
        else if( j > x && j <= i ) tmp[j] = titles[j-1];
        else tmp[j] = titles[j+1];
    }
    eventOrderTitles(tmp, maxi);
}

string GetHelp() {
    if( creatorp(this_player()) ) {
        return ("Syntax: title <title>\n\n"
                "Allows you to change your title.  You must include the "
                "token $N in your title, which will be replaced with your "
                "name as appropriate.  For example:\n"
                "\ttitle We are $N of Borg\n"
                "would make my short appear as:\n"
                "\tWe are Descartes of Borg.");
    }
    else {
        return ("Syntax: title\n\n"
                "Allows you to reorder your titles.  Your first two titles "
                "appear in your short description.  This command is "
                "interactive, meaning it prompts you for what to do.  "
                "hit 'q' at any point to save your changes and exit out of "
                "this command.\nYou can also type (title 1) or (title 2) "
                "to change how many titles appear in your description.");
    }
}

