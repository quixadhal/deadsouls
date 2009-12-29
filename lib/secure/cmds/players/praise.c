/*    /secure/cmds/player/praise.c
 *    from the Dead Souls LPC Library
 *    makes a praisereport
 *    created by Descartes of Borg 950427
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

void eventEndEdit(object ob);
string GetCreator(object ob);

mixed cmd(string text) {
    object ob;
    string secondary, tmp, file;

    if( text ) {
        string *words;
        object env;
        int i, maxi;

        maxi = sizeof( words = explode(text, " ") );
        for(i=0, tmp = ""; i<maxi; i++ ) {
            tmp += words[i];
            if( ob = present(tmp, this_player()) ) {
                if( i == maxi - 1 ) text = "";
                else text = implode(words[i+1..], " ");
                break;
            }
            if( (env=environment(this_player())) && ob = present(tmp, env) ) {
                if( i == maxi - 1 ) text = "";
                else text = implode(words[i+1..], " ");
                break;
            }
            tmp += " ";
        }
    }
    else text = "";
    ob = (ob || environment(this_player()));
    if( text != "" ) {
        if( ob ) {
            tmp = this_player()->GetCapName() + " reports praise in: " +
                identify(ob) + "\non " + ctime(time()) + "\n";
            secondary = GetCreator(ob);
        }
        else 
            tmp = this_player()->GetCapName() + " reports praise on: " +
                ctime(time()) + ":\n";
        log_file("praise", tmp + text + "\n\n");
        if( secondary ) log_file("reports/" + secondary, tmp + text + "\n\n");
        message("system", "Praise reported!!! Thank you!!!", this_player());
        return 1;
    }
    rm( file = DIR_TMP "/" + this_player()->GetKeyName() );
    this_player()->eventEdit( file, (: eventEndEdit, ob :) );
    return 1;
}

void eventEndEdit(object ob) {
    string tmp, text, file, secondary;

    file = DIR_TMP "/" + this_player()->GetKeyName();
    if( !file_exists(file) || !(text = read_file(file)) ) {
        message("system", "Praise report aborted.", this_player());
        return;
    }
    rm(file);
    if( ob ) {
        tmp = this_player()->GetCapName() + " reports praise in: " +
            identify(ob) + "\non: " + ctime(time()) + "\n";
        secondary = GetCreator(ob);
    }
    else 
        tmp = this_player()->GetCapName() + " reports praise on " +
            ctime(time()) + ":\n";
    log_file("praise", tmp + text + "\n\n");
    if( secondary ) log_file("reports/" + secondary, tmp + text + "\n\n"); 
    message("system", "Praise reported!!! Thank you!!!", this_player());
}

string GetCreator(object ob) {
    string fn, nom;

    fn = file_name(ob);
    if( sscanf(fn, REALMS_DIRS "/%s/%*s", nom) ) return nom;
    else if( sscanf(fn, DOMAINS_DIRS "/%s/%*s", nom) ) return nom;
    else return 0;
}

string GetHelp(){
    return ("Syntax: praise [object] [message]\n\n"
            "Allows you to praise things in the MUD to both the MUD "
            "administrators as well as to the creator of the object you "
            "are referencing (if any).  If you do not specify an object, "
            "then the report will be sent to the creator of the room in which "
            "you are located in addition to the MUD administrators.  If you "
            "fail to specify text on the command line, then you will be put "
            "into the editor in order to compose the report.\n"
            "See also: bug, idea");
}
