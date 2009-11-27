#include <lib.h>

inherit LIB_DAEMON;

mapping TermInfo;

void create(){
    TermInfo = 
        ([ "RED"  : "<FONT COLOR=\"#FF0000\">",
         "GREEN" : "<FONT COLOR=\"#00FF00\">",
         "BLUE"  : "<FONT COLOR=\"#0000FF\">",
         "YELLOW" : "<FONT COLOR=\"#FFFF00\">",
         "CYAN" : "<FONT COLOR=\"#00A0DD\">",
         "MAGENTA" : "<FONT COLOR=\"#C50067\">",
         "YELLOW" : "<FONT COLOR=\"#FFFF00\">",
         "RESET": "</FONT>",          "BR" : "<BR>", "P" : "<P>", "/P" : "</P>", ">" : ">",
         "HREF" : "<A HREF=", "NAME" : "<A NAME=", "/A" : "</A>",
         "I" : "<I>", "/I" : "</I>", "PRE" : "</PRE>", "/PRE" : "</PRE>",
         "STRONG" : "<STRONG>", "/STRONG" : "</STRONG>",
         "TABLE" : "<TABLE>", "/TABLE" : "</TABLE>", "TR" : "<TR>",
         "/TR" : "</TR>", "TD" : "<TD>" , "/TD" : "</TD>" ]);
}

mixed cmd(string args){
    string file = read_file(args);
    string *ret = explode(file,"\n");
    file = implode(ret,"<BR>");
    file = terminal_colour(file+"%^RESET%^\n", TermInfo);
    file = "<HTML>" + file + "</HTML>";
    write(file);
    return 1;
}

string GetHelp() {
    return "Syntax: html <FILE>\n\n"
        "Outputs to the screen an htmlified version of the "
        "file specified.";
}
