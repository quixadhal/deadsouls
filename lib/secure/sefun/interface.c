/*    /adm/sefun/interface.c
 *    from Dead Souls
 *    user interface sefuns
 *    created by Descartes of Borg 940215
 */

#include <daemons.h>

string strip_colours(string str) {
    return (string)TERMINAL_D->no_colours(str);
}
