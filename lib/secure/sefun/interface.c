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

string strip_colors(string str){
    return terminal_color(str, Uncolor);
}

