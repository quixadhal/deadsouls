#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *arr;
    string tmp;

    if( !str ) return "You must specify a file to cat.";
    else str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) return "File " + str + " not found.";
    else if( !(tmp = read_file(str)) )
        return "Unable to read file " + str + ".";
    arr = explode(tmp,"\n");
    foreach(string line in arr){
        message("system", line, this_player());
    }
    return 1;
}

int help() {
    message("help", "Syntax: <longcat [file]>\n\n"
      "    /\___/\ \n"
      "   /       \ \n"
      "  |  #    # | \n"
      "  \     @   | \n"
      "   \   _|_ / \n"
      "   /       \______ \n"
      "  / _______ ___   \ \n"
      "  |_____   \   \__/ \n"
      "   |    \__/ \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   |       | \n"
      "   /        \ \n"
      "  /   ____   \ \n"
      "  |  /    \  | \n"
      "  | |      | | \n"
      " /  |      |  \ \n"
      " \__/      \__/ \n"
      "Displays the contents of the file mentioned all at once, "
      "with no limit on the output size. Note that the output "
      "for ludicrously large files may lag the mud and cause "
      "the command to error out with a \"Too long evaluation\".",
      this_player());
}
