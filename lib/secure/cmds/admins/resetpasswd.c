#include <lib.h>
#include <dirs.h>
#include <flags.h>
#include <message_class.h>

inherit LIB_DAEMON;

static void NewPass(string pass);
static void ConfirmPass(string str, string newpass);

string savefile;

mixed cmd(string args) {

    if(!archp(previous_object())) {
        write("No.");
        return 1;
    }

    if(!args || !sizeof(args)) args = "me";

    args = lower_case(args);

    if(args == "me" || args == this_player()->GetKeyName()){
        write("To reset your own password, use the command: passwd");
        return 1;
    }

    if( previous_object()->GetForced() )
        return "You cannot be forced to change a password.";
    if( previous_object() != this_player() )
        return "You're being sploited to reset someone's password.";

    if(!user_exists(args)){
        write("That person does not exist on this mud.");
        return 1;
    }

    if(find_player(args)){
        write("That player is currently logged on. Please use "
                "the passwd command to reset their password.");
        return 1;
    }

    savefile = save_file(DIR_CRES + "/" + args[0..0] + "/" + args);
    if(!file_exists(savefile)){
        write("Couldn't find "+savefile+". Looking for alternate.");
        savefile = save_file(DIR_PLAYERS + "/" + args[0..0] + "/"+ args);
    }

    if(!file_exists(savefile)){
        write("Save file could not be found. Exiting.");
        return 1;
    }

    write("Found "+savefile+".");

    previous_object()->eventPrint("New password: ", MSG_PROMPT);
    input_to( (: NewPass :), I_NOECHO | I_NOESC, previous_object() || args);
    return 1;
}

static void NewPass(string pass) {
    if( !pass || strlen(pass) < 5 ) {
        this_player()->eventPrint("Password must be at least 5 "
                "characters, password change failed.",
                MSG_SYSTEM);
        return;
    }
    this_player()->eventPrint("\nConfirm: ", MSG_PROMPT);
    input_to( (: ConfirmPass :), I_NOECHO | I_NOESC, pass);
}

static void ConfirmPass(string str, string newpass) {
    string *lines;
    if( str != newpass ) {
        this_player()->eventPrint("Passwords do not match.", MSG_SYSTEM);
        return;
    }

    lines = explode(unguarded((: read_file, savefile :)),"\n");
    unguarded((: rm, savefile :));
    foreach(string line in lines) {
        string val;

        if( sscanf(line, "Password %s", val) )
            line = "Password \"" + crypt(newpass, 0) + "\"";
        unguarded((: write_file, savefile, line + "\n" :));
    }
    this_player()->eventPrint("\nPassword changed.", MSG_SYSTEM);
}

string GetHelp() {
    return ("Syntax: resetpasswd <player>\n\n"
            "Allows you to change someone's password if they "
            "are not logged on.\n"
            "See also: passwd");
}
