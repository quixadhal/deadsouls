/*    /cmds/player/faq.c
 *    from the Foundation II LPC Library
 *    a command to access Frequently Asked Questions lists easily
 *    created by Descartes of Borg 950501 (power to the proletariate)
 */

#include <lib.h>

inherit LIB_DAEMON;

static void GetFAQ(string args);

mixed cmd(string args) {
    if( !args || args == "" ) {
        string *files;
        int cols, i, maxi, x, y;

        message("help", mud_name() + " has the following FAQ lists:",
                this_player());
        cols = ((int *)this_player()->GetScreen())[0];
        i = sizeof(files = get_dir(DIR_FAQS "/"));
        while(i--) if( (y =strlen(files[i])) > x ) x = y;
        cols = cols / (x+2);
        message("help", format_page(files, cols), this_player());
        message("prompt", "\nWhich FAQ (general)? ", this_player());
        input_to( (: GetFAQ :) );
        return 1;
    }
    else GetFAQ(args);
    return 1;
}

static void GetFAQ(string args) {
    string file;
    //bugfix courtesy of Manchi
    if( !args || args == "" || args == "y" || args == "Y" || lower_case(args) == "yes")
        args = "general";
    if( file_size(file = DIR_FAQS "/" + args) < 0 ) {
        message("error", "No such FAQ available.", this_player());
        return;
    }
    if( !(file = read_file(file)) ) {
        message("error", "Failed to read FAQ document.", this_player());
        return;
    }
    this_player()->eventPage(explode(file, "\n"), "help");
}

string GetHelp() {
    return ("Syntax: faq [FAQ]\n\n"
            "If you specify a particular FAQ to read, this command will "
            "display that FAQ for you.  If you fail to specify a FAQ, it "
            "will give you a list of FAQs from which to choose.  FAQ "
            "stands for Frequently Asked Questions.  Reading the FAQs is "
            "a good way of coming to understand topics basic to " +
            mud_name() + ".\n"
            "See also: help" + (creatorp(this_player()) ? ", man" : ""));
}


