string print_vars(mixed *vars) {
    string *result = allocate(sizeof(vars));
    int i;

    for (i=0; i<sizeof(vars); i++) {
	if (mapp(vars[i]))
	    result[i] = "([ ... ])";
	else if (functionp(vars[i]))
	    result[i] = "(: ... :)";
	else if (intp(vars[i]))
	{
	    if (vars[i]) result[i]=vars[i]+"";
	    else if (nullp(vars[i])) result[i]="NULL";
	    else if (undefinedp(vars[i])) result[i]="UNDEFINED";
	    else result[i]="0";
	}
	else if (stringp(vars[i]))
	    result[i] = "\""+vars[i]+"\"";
	else if (pointerp(vars[i]))
	    result[i] = "({ ... })";
	else if (floatp(vars[i]))
	    result[i] = vars[i]+"";
	else if (bufferp(vars[i]))
	    result[i] = "<BUFFER>";
    }
    return implode(result, ", ");
}

int
cmd(string str) {
    mapping frame;
    object ob;
    string who;
    int num;
    int i;

    if (!str || (sscanf(str, "%d", num)==0 && sscanf(str, "%s %d", who, num)==0)) {
        write("dbxframe [<who>] <frame>");
	return 1;
    }
    if (who) {
        ob = find_player(who);
        if (!ob) {
            write("No such player.");
            return 1;
        }
    }
    else ob = this_player();
    frame = ob->GetLastError();
    if (!frame) {
	write("No error.");
	return 1;
    }
    if (num<0 || num>=sizeof(frame["trace"]))
	return notify_fail("No such frame.\n");
    frame = frame["trace"][num];

    printf("------%s:%i - %s(%s)\n", frame["program"], frame["line"],
      frame["function"],print_vars(frame["arguments"]));
    printf("locals: %s\n", print_vars(frame["locals"]));
    printf("----------------------------------------------------------------\n");
    printf("%s=>%s%s",
      read_file("/"+frame["program"], frame["line"]-5, 5),
      read_file("/"+frame["program"], frame["line"], 1),
      read_file("/"+frame["program"], frame["line"]+1, 5));

    return 1;
}
