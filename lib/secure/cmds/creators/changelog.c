/*    /cmd *    from Dead Souls
 *    a command for logging changes made
 *    created by Descartes of Borg 940701
 *    bug fixed by Pallando, 940702
 */

#include <lib.h>
#include <dirs.h>
#include <objects.h>

#define CHANGELOG "ChangeLog"
#define TMP_FILE DIR_TMP + "/" + previous_object()->GetKeyName() + ".change"

inherit LIB_DAEMON;

int post_change(mixed *args);

int cmd(string str) {
    string *tmp;
    string file;
    int i;

    if(!str) str = this_player()->get_path();
    str = absolute_path(this_player()->get_path(), str);
    if(file_size(str) == -1) return notify_fail("Invalid path.\n");
    if(file_size(file = str) != -2) {
        i = sizeof(tmp = explode(str, "/"));
        if(i < 2) str = "/";
        else str = "/"+implode(tmp[0..i-2], "/");
    }
    if(str[strlen(str)-1] != '/') str += "/";
    str += CHANGELOG;
    message("system", "Logging changes to "+str+"...", this_player());
    this_player()->eventEdit(TMP_FILE, (: post_change, ({ file, str }) :));
    return 1;
}

void abort() { rm(TMP_FILE); }

void post_change(mixed *args) {
    string *tmp;
    string file, change, str, str2;
    int i, maxi;

    change = args[0];
    file = args[1];
    if(!userp(previous_object())) return;
    str = read_file(TMP_FILE);
    rm(TMP_FILE);
    if( !str ) { write( "Aborting.\n" ); return; }
    str = replace_string(str, "\n", " ");
    maxi = sizeof(tmp = explode(wrap(str, 60), "\n"));
    str = "-  "+ change+"\n";
    str += "   "+ctime(time())+" by "+previous_object()->GetCapName()+
        "("+previous_object()->query_rname()+"):\n";
    for(i=0; i<maxi; i++) str += "     * "+tmp[i]+"\n";
    if(str2 = read_file(file)) str2 = str+"\n\n"+str2;
    else str2 = str;
    rm(file);
    write_file(file, str2);
    if(str2 = read_file(DIR_LOGS+"/"+CHANGELOG)) str = str + "\n\n"+str2;
    rm(DIR_LOGS+"/"+CHANGELOG);
    write_file(DIR_LOGS+"/"+CHANGELOG, str);
}

string GetHelp(){
    return ("Syntax: changelog <directory>\n"
            "Lets you enter documentation on changes you have made to files in "
            "directory <directory>\n");
}
