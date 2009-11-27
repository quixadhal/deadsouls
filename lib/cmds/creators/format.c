/* fake wiz command for the help format made by Valodin Jan 93 */
// NB A nicer solution is to make the help command check a directory in
// /doc if you are a wizard.
//          Signed... Campaign '93 "Keep /bin for commands"

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str)
{
    write("This is a fake wiz command.  Try \"help format\" to get" +
            " information on\nspecifying objects for certain wiz commands.\n");
    return 1;
}

string GetHelp(){
    return ("NOTE: This is not a command in itself.\n" +
            "Certain wizard commands require that you specify\n" +
            "objects in a certain format.\n" +
            "The format uses prefixes to specify types of\n" +
            "objects as follows:\n" +
            "  !ob   ob is NOT an NPC or player\n" +
            "  %ob   ob is a player\n" +
            "  $ob   ob is an NPC\n" +
            "  #int  int is an ordinal value, e.g. 0, 1 , 2...\n" +
            "  /str  str is a filename of an object\n" +
            "Objects can also be \"me\", which is your player\n" +
            "  object, or \"here\", which is you environment.\n");
    write("Objects denoted by prefixes are joined by a few\n" +
            "functional symbols:\n" +
            "  object@environment  denotes the object at an\n" +
            "    environment.  Environment can be either an\n" +
            "    object or an object@environment.\n" +
            "  e(object)  denotes the environment of object.\n\n");
    write("For example:\n" +
            "  !sword@%descartes  is the sword in Descartes'\n" +
            "     inventory.\n" +
            "  #2@!bag@$orc@here  is the 3rd item in the bag\n" +
            "     that the orc in front of you is holding.\n" +
            "     NOTE: #0 is the first object.\n" +
            "  $orc@/d/standard/square  is the orc that\n" +
            "     shouldn't be in the square.\n" +
            "See also: patch\n"+
            "See man: to_object, get_object, get_objects\n"+
            "");
}
