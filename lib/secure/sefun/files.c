/*    /secure/sefun/files.c
 *    from Dead Souls 3.3
 *    functions which handle file operations
 *    created by Descartes of Borg 940926
 */

int file_exists(string str) { return (file_size(str) > -1); }

string save_file(string who) {
    if( !stringp(who) ) error("Bad argument 1 to save_file().");
    who = convert_name(who);
    return (string)master()->player_save_file(who);
}
