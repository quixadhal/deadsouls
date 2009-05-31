/*    /adm/sefun/interface.c
 *    from Dead Souls
 *    user interface sefuns
 *    created by Descartes of Borg 940215
 */

#include <daemons.h>

string strip_colours(string str){
    string ret;
    mapping Uncolor = ([ "RESET": "\b", "BOLD": "", "FLASH":"", "BLACK":"", "RED":"",
            "BLUE":"", "CYAN":"", "MAGENTA":"", "ORANGE":"", "YELLOW":"",
            "GREEN":"", "WHITE":"", "BLACK":"", "B_RED":"", "B_ORANGE":"",
            "B_YELLOW":"", "B_BLACK":"", "B_CYAN":"","B_WHITE":"", "B_GREEN":"",
            "B_MAGENTA":"", "STATUS":"", "WINDOW":"", "INITTERM": "", "B_BLUE":"",
            "ENDTERM":""]);
    ret = terminal_colour(str, Uncolor);
    return replace_string(ret, "\b", "");
}

string strip_colors(string str){
    return strip_colours(str);
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
