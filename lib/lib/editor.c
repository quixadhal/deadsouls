/*    /lib/editor.c
 *    from the Dead Souls LPC Library
 *    mudlib based editor interface
 *    created by Descartes of Borg 950406
 */


#include "include/editor.h"

private static function EditCallback;

static void create() {
    EditCallback = 0;
}

varargs void eventEdit(string file, function callback) {
    EditCallback = callback;
    ed_start(file, !creatorp());
    if( !creatorp() || file_size(file) < 1 ) ed_cmd("$a");
}

static string process_input(string str) {
    int x;

    if( (x = query_ed_mode()) == -1 ) return str;
    if( str && str[0..0] == "!" && strlen(str) > 1 ) return str[1..];
    if( !x ) {
	str = ed_cmd(str);
	message("editor", str, this_object());
	if( query_ed_mode() == -1 ) {
	    if( functionp(EditCallback) ) evaluate(EditCallback);
	    EditCallback = 0;
	    return "";
	}
	return "";
    }
    else str = ed_cmd(str);
    message("editor", str, this_object());
    return "";
}

