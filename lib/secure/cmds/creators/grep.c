//      /bin/system/_grep.c
//      from the Dead Souls Mudlib
//      searches for an expression in files
//      created by Descartes of Borg 930822

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    mapping borg;
    string *lines, *files;
    string output, exp, file, tmp, txt;
    int i, max;

    notify_fail("Correct syntax: <grep '[pattern]' [file] (> [output])>\n");
    if(!str) return 0;
    if(sscanf(str, "%s > %s", tmp, output) == 2) {
        if(output[0] != '/')
          output = (string)previous_object()->get_path()+"/"+output;
        str = tmp;
    }
    else output = 0;
    if(sscanf(str, "'%s' %s", exp, file) != 2 &&
      sscanf(str, "%s %s", exp, file) != 2) return 0;
    if(!(max = sizeof(files = (string *)previous_object()->wild_card(file)))) { 
        message("system", "File not found.", this_player());
        return 1;
    }
    for(i=0, borg = ([]); i<max; i++) {
        if(!(txt = read_file(files[i]))) continue;
        borg[files[i]] = regexp(explode(txt, "\n"), exp);
        if(!sizeof(borg[files[i]])) map_delete(borg, files[i]);
    }
    if(!(max = sizeof(files = keys(borg)))) str = "No matches found.\n";
    else {
        for(i=0, str = ""; i<max; i++)
          str += sprintf("%s:\n%s\n\n", files[i], implode(borg[files[i]],"\n"));
    }
    if(output) {
        if(!write_file(output, str)) 
          message("system", "Failed to write to: "+output, this_player());
        else message("system", "Grep sent to: "+output, this_player());
    }
    else message("Nsystem", str, this_player());
    return 1;
}

void help() {
    message("help",
      "Syntax: <grep '[pattern]' [file] (> [redirect])>\n\n"
      "Searches a file or group of files for a specific pattern.  "
      "If the pattern is a single word, then no '' is needed.  Patterns "
      "of more than one word however, need to be enclosed in ''.  "
      "You may redirect the output of the grep to to a file using the >."
      "\n\nSee also: cd, ls, mv, pwd, rm", this_player()
    );
}
