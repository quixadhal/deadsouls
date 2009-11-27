/*   statconvert.c
 *   A utility which reduces or raises SetSkill and SetStat lines
 *   in a file by a specified percentage.
 *   created by Blitz@Dead Souls
 */

void GetHelp(string str);

string ConvertLine(string str, int change, int line, string search)
{
    string s1, s2, cut, n, tmp;
    int a, b, val;

    if(!archp(previous_object())) return 0;
    while( (a = strsrch(str, search)) > -1 ) {
        tmp = str[a..];
        b = strsrch(tmp, ")");
        if( b < 0) break;
        cut = tmp[0..b];
        if( sscanf(cut, "%s,%s", s1, s2) != 2 ) break;
        s2 = replace_string(s2, " ", "");
        sscanf(s2, "%s)", s2);
        val = to_int(s2);
        if( !val ) break;
        val = val - (val / (100 / change));
        n = sprintf("%s, %d)", s1[0] + "@@@" + s1[1..], val);
        str = replace_string(str, cut, n);
        write("Line "+line+": "+cut+" -> "+val+".");
    }
    str = replace_string(str, search[0] + "@@@" + search[1..], search);
    return str;
}

void Convert(string s, int change) {
    string args, file, str;
    string *files, *tmp;
    int i;

    if(!archp(previous_object())) return 0;
    if( !sizeof(s) ) args = "*.c";
    else args = s;
    files = wild_card(args);
    if( !i = sizeof(files) ) {
        message("info", "No file(s) found: "+implode(files, " "),
                this_player() );
        return;
    }
    foreach(file in files) {
        string line;
        int    changed = 0;

        str = read_file(file);
        if( !sizeof(str) ) continue;
        i = sizeof(tmp = explode(str, "\n"));
        write("Checking: "+file);
        while(i--) {
            line = tmp[i];
            line = ConvertLine(line, change, i, "SetSkill");
            line = ConvertLine(line, change, i, "SetStat");
            if( tmp[i] != line ) changed = 1;
            if( changed ) tmp[i] = line;
        }
        if( changed ) {
            rename(file, file+"~");
            write(file+": Writing new file...");
            write_file(file, implode(tmp, "\n"));
        }
    }
}

mixed cmd(string str) {
    string files;
    int change;

    if(!archp(previous_object())) return 0;
    if( !sizeof(str) || (sscanf(str, "%s %d", files, change) != 2) || !change) {
        GetHelp(0);
        return 1;
    }
    files = absolute_path(this_player()->query_cwd(), files);
    Convert(files, change);
    return 1;
}


string GetHelp(){
    return ("Syntax: statconvert <filename | wild card> <percentage>\n\n"
            "This command searches through the specified file(s) and lowers "
            "all occurrences of SetSkill and SetStat by a <percentage> "
            "amount.\n\n"
            "Example: statconvert *.c 10\n"
            "  - Searches all .c files in your currenct path, and reduces\n"
            "    all skill and stat settings by 10%\n\n"
            "Note: It is possible to raise settings by passing "
            "a negative number.");
}
