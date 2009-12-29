/*    /cmds/creator/scan.c
 *    from the Foundation II LPC Library
 *    scans an object
 *    created by Descartes of Borg 950516
 *    -fi flags added for Dead Souls by Blitz 951208
 */

#include <lib.h>

#define OPT_E       (1 << 1)
#define OPT_D       (1 << 2)
#define OPT_I       (1 << 3)
#define OPT_F       (1 << 4)

inherit LIB_DAEMON;

string inventory(object ob, int level, int scan);

mixed cmd(string args) {
    object *inv;
    object ob;
    string tmp;
    int scan, i, maxi;

    if( args == "" || !args ) {
        ob = this_player();
        scan = 0;
    }
    else {
        while(args[0] == '-') {
            switch(args[1]) {
                case 'd': scan |= OPT_D; break;
                case 'e': scan |= OPT_E; break;
                case 'i': scan |= OPT_I; break;
                case 'f': scan |= OPT_F; break;
            }
            if( strlen(args) > 3 ) args = trim(args[2..]);
            else args = "";
        }
        if( args != "" ) ob = to_object(args);
        if( !ob ) ob = this_player();
    }
    if( scan & OPT_E ) ob = environment(ob);
    if( !ob ) return "No environment for requested object.";
    if( scan & OPT_D ) tmp = "Deep scanning " + identify(ob) + ":\n";
    else tmp = "Scanning " + identify(ob) + ":\n";
    for(i=0, maxi = sizeof(inv = all_inventory(ob)); i<maxi; i++)
        tmp += inventory(inv[i], 1, scan);
    this_player()->eventPage(explode(tmp, "\n") + ({""}), "system");
    return 1;
}

string inventory(object ob, int level, int scan) {
    object *inv;
    string ret;
    int i, maxi;

    for(i = 1, ret = ""; i <= level; i++) ret += "\t";
    if( scan & OPT_I ) {
        string tmp;
        if( scan & OPT_F ) tmp = identify(ob) + "\n" + ret + " ";
        else tmp = "";
        tmp += sprintf("(%s) - Mass: %d  Value: %d  Class: %d",
                capitalize(ob->GetKeyName()),
                ob->GetMass(), ob->GetValue(),
                intp(ob->GetClass()) ? ob->GetClass() : 0 );
        if( ob->GetWorn() ) tmp += " (worn)";
        ret += tmp;
    }
    else ret += identify(ob);
    if( ( scan & OPT_D ) &&
            ( maxi = sizeof(inv = all_inventory(ob))) ) ret += ":\n";
    else ret += "\n";
    if( !( scan & OPT_D ) ) return ret;
    for(i=0; i<maxi; i++)
        ret += inventory(inv[i], level + 1, scan);
    return ret;
}

string GetHelp(string str) {
    return "Syntax: scan [-e] [-d] [-i] [-f] [object]\n\n"
        "Scans the inventory of the object you name.  If you do "
        "not specify an object, then it gives you your inventory.  "
        "If you specify the -e option, the command does the "
        "environment of the object which is targeted.  If the -d "
        "option is specified, then a deep scan is done.  The -i "
        "option displays information about each object.  The -f "
        "option forces filenames to be displayed (default).\n\n"
        "See also: inventory, stat";
}

