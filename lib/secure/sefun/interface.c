/*    /adm/sefun/interface.c
 *    from Dead Souls
 *    user interface sefuns
 *    created by Descartes of Borg 940215
 */

mapping Uncolor = ([ "RESET": "", "BOLD": "", "FLASH":"", "BLACK":"", "RED":"",
  "BLUE":"", "CYAN":"", "MAGENTA":"", "ORANGE":"", "YELLOW":"",
  "GREEN":"", "WHITE":"", "BLACK":"", "B_RED":"", "B_ORANGE":"",
  "B_YELLOW":"", "B_BLACK":"", "B_CYAN":"","B_WHITE":"", "B_GREEN":"",
  "B_MAGENTA":"", "STATUS":"", "WINDOW":"", "INITTERM": "",
  "ENDTERM":""]);

#include <daemons.h>

string strip_colours(string str) {
    return (string)TERMINAL_D->no_colours(str);
}

string strip_colors_old(string str){
    string output = "";
    string *input = explode(str,"%^");
    string *list = ({ "RED","YELLOW","BLUE","GREEN","MAGENTA","ORANGE","CYAN","BLACK","WHITE"});
    list += ({ "B_RED","B_YELLOW","B_BLUE","B_GREEN","B_MAGENTA","B_ORANGE","B_CYAN","B_BLACK","B_WHITE"});
    list += ({"BOLD","FLASH","RESET"});
    foreach(string color in list) input -= ({ color });
    output = implode(input,"");
    if(sizeof(output)) return output;
    else return "";
}

string strip_colors(string str){
#ifdef F_TERMINAL_COLOR
#if F_TERMINAL_COLOR
    return terminal_color(str, Uncolor);
#else
    return strip_colors_old(str);
#endif
#else
    if(grepp(version(),"FluffOS v2.9")) return strip_colors_old(str);
    else return terminal_colour(str, Uncolor);
#endif
}
