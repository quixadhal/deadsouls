/*    /adm/sefun/convert_name.c
 *    from Dead Souls
 *    changes any cap name to a true name
 *    created by Descartes of Borg 940214
 */

#include <daemons.h>

string convert_name(string str) {
    string a, b, ret;
    if(!str || str == "") return "";
    if(!grepp(str,"@")){
	str = replace_string(str, " ", "");
	str = replace_string(str, "'", "");
	return lower_case(replace_string(str, "-", ""));
    }
    if(sscanf(str, "%s@%s", a, b) == 2 &&
      ret = INTERMUD_D->GetMudName(b)) return a+"@"+ret;
    else return str; 
}
