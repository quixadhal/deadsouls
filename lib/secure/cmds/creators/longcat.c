#include <lib.h>
#include <runtime_config.h>

inherit LIB_DAEMON;

static int send_queued_mess(string tmp, object whom){
    message("receive", tmp, whom);
    return 1;
}

mixed cmd(string str) {
    string *arr;
    mixed tmp;
    int delay = -1;
    int bmax = (__LARGEST_PRINTABLE_STRING__ - 1024);
    int chunks, rem, currchunk, smax = (get_config(__MAX_STRING_LENGTH__) - 10);

    if( !str ) return "You must specify a file to longcat.";
    else str = absolute_path(this_player()->query_cwd(), str);
    if( !file_exists(str) ) return "File " + str + " not found.";
    if((tmp = file_size(str)) < smax){
        if( !(tmp = read_file(str)) )
            return "Unable to read file " + str + ".";
        arr = explode(tmp,"\n");
        foreach(string line in arr){
            message("system", line, this_player());
        }
        return 1;
    }
    chunks = tmp / bmax;
    rem = tmp % bmax;
    if(rem) chunks++;
    while(chunks){
        delay++;
        chunks--;
        if(chunks){
            tmp = read_bytes(str, currchunk, bmax);           
            currchunk += bmax;
        }
        if(rem && !chunks){
            tmp = read_bytes(str, currchunk, rem);
            currchunk += rem;
        }
        call_out("send_queued_mess", (delay/5), tmp, this_player());
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: longcat <file>\n\n"
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
            "the command to error out with a \"Too long evaluation\".");
}
