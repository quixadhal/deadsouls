/*    /secure/cmds/admins/addclass.c
 *    from the NIghtmare V Object Library
 *    creates new classes
 *    created by Descartes of Borg 960528
 *    Version: %I%
 *    Last Modified: %D%
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string file;

    if( !args || args == "" ) return "Syntax: <addclass CLASS>";
    if( !file_exists(file=DIR_SECURE_CFG "/classes/" + args) )
	return "File not found: " + file;
    CLASSES_D->AddClass(file);
    previous_object()->eventPrint("Class added.");
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <addclass CLASS>\n\n"
	    "Allows you to add a new class to the class data stored in the "
	    "classes daemon.  Specifically, you create a configuration file "
	    "and then issue this command to load the new class into the "
	    "classes daemon.  The file should be in " + DIR_SECURE_CFG +
	    "/classes.  The format of the file being read is:\n"
	    "class_name\n"
	    "main_class1:multi_class_name1\n"
	    "...\n"
	    "main_classN:multi_class_nameN\n"
	    "skill1:skill_average1:skill_class1\n"
	    "...\n"
	    "skillN:skill_averageN:skill_classN\n"
	    "An example of a fighter exists in /secure/cfg/classes/fighter.  "
	    "This system is admittedly rather complex, but it beats hard "
	    "coding these values.  In addition, a web-based administration "
	    "client is being developed to make class creation nothing more "
	    "than filling in a form.\n\n"
	    "See also: addemote, addrace");
}
