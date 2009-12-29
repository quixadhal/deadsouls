//      /bin/system/_grep.c
//      from the Dead Souls Mudlib
//      searches for an expression in files
//      created by Descartes of Borg 930822
//        numbered results added by Nimrod@The Tower 060805

#include <lib.h>

#define GREP_NUMBERED_LINES            (1<<0)
#define GREP_RECURSE_DIRECTORIES    (1<<1)
#define MAX_FILE_SIZE 200000

inherit LIB_DAEMON;

int max_files = 1000;
string *allowed_types3 = ({ ".txt", ".cfg" });
string *allowed_types1 = ({ ".c", ".h" });

int cmd(string str) {
    mapping borg;
    string *lines, *files, *r_files;
    string output, exp, file, tmp;
    mixed txt;

    int err, i, i_lines, max, max_lines, flags;

    notify_fail("Correct syntax: <grep [-nr] '[pattern]' [file] (> [output])>\n");
    if(!str) return 0;
    //CHECK FOR FLAGS
    if(str[0] == '-'){
        if(sscanf(str, "-%s %s", tmp, txt) == 2){
            for(i = 0, max = sizeof(tmp); i<max; i++){
                switch(tmp[i]){
                    case 'n' : flags = flags|GREP_NUMBERED_LINES ;break;
                    case 'r' : flags = flags|GREP_RECURSE_DIRECTORIES ;break;
                }
            }
        }
        str = txt;
    }

    if(sscanf(str, "%s > %s", tmp, output) == 2) {
        if(output[0] != '/') output = previous_object()->get_path()+"/"+output;
        str = tmp;
    }
    else output = 0;
    if(sscanf(str, "'%s' %s", exp, file) != 2 &&
            sscanf(str, "%s %s", exp, file) != 2) return 0;
    if(!(max = sizeof(files = wild_card(file)))) {         
        message("system", "File not found.", this_player());
        return 1;
    }

    if(flags&GREP_NUMBERED_LINES){
        for(i=0, borg = ([]); i<max; i++) {
            if((file_size(files[i]) == -2)&&(flags&GREP_RECURSE_DIRECTORIES)){
                r_files = wild_card(files[i]+"/*");
                if(max + sizeof(r_files) > max_files){
                    write("Too many files in the recurse. Aborting grep.");
                    return 1;
                }
                files += r_files;
                max += sizeof(r_files);
                continue;
            }
            if(file_size(files[i]) > MAX_FILE_SIZE){
                write(files[i]+": too large. Skipping.");
                continue;
            }
#if 0
            if(member_array(last(files[i],2), allowed_types1) == -1 &&
                    member_array(last(files[i],4), allowed_types3) == -1 &&
                    grepp(files[i],".")){
                write(files[i]+": unrecognized extension. Skipping.");
                continue;
            }
#endif
            err = catch(txt = read_file(files[i]));
            if(err){
                if(file_exists(files[i])) write(files[i]+": corrupted file, or not text. Skipping.");
                continue;
            }
            if(txt) lines = explode(txt, "\n");
            else {
                write(files[i]+": unreadable file or directory. Skipping.");
                continue;
            }
            borg[files[i]] = ({});
            for(i_lines = 0, max_lines = sizeof(lines); i_lines<max_lines; i_lines++){
                if(regexp(lines[i_lines], exp)){
                    borg[files[i]] += ({sprintf("%d: %s", i_lines, lines[i_lines]) });
                }
            }
            if(!sizeof(borg[files[i]])) map_delete(borg, files[i]);
        }
    }
    else {
        for(i=0, borg = ([]); i<max; i++) {
            if((file_size(files[i]) == -2)&&(flags&GREP_RECURSE_DIRECTORIES)){
                r_files = wild_card(files[i]+"/*");
                if(max + sizeof(r_files) > max_files){
                    write("Too many files in the recurse. Aborting grep.");
                    return 1;
                }
                files += r_files;
                max += sizeof(r_files);
                continue;
            }
            if(file_size(files[i]) > MAX_FILE_SIZE){
                write(files[i]+": too large. Skipping.");
                continue;
            }
#if 0
            if(member_array(last(files[i],2), allowed_types1) == -1 &&
                    member_array(last(files[i],4), allowed_types3) == -1 &&
                    grepp(files[i],".")){
                write(files[i]+": unrecognized extension. Skipping.");
                continue;
            }
#endif
            err = catch(txt = read_file(files[i]));
            if(err || !txt){
                if(file_exists(files[i])) write(files[i]+": corrupted file, or not text. Skipping.");
                continue;
            }
            borg[files[i]] = regexp(explode(txt, "\n"), exp);
            if(!sizeof(borg[files[i]])) map_delete(borg, files[i]);
        }
    }
    if(!(max = sizeof(files = keys(borg)))) str = "No matches found.\n";
    else {
        for(i=0, str = ""; i<max; i++)           str += sprintf("%s:\n%s\n\n", files[i], implode(borg[files[i]],"\n"));
    }
    if(output) {
        if(!write_file(output, str)) message("system", "Failed to write to: "+output, this_player());
        else message("system", "Grep sent to: "+output, this_player());
    }
    else message("system", str, this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: grep [-nr] '[pattern]' [file] (> [redirect])\n\n"
            "Searches a file or group of files for a specific pattern.  "
            "If the pattern is a single word, then no '' is needed.  Patterns "
            "of more than one word or beginning with a '-' however, need to be enclosed in ''.  "
            "You may redirect the output of the grep to a file using the >."
            "\n  Options:\n"
            "    -r  recursive search, search extends to child directories.\n"
            "    -n  numbered lines, includes line numbers of successful hits."
            "\nSee also: cd, ls, mv, pwd, rm");
} 
