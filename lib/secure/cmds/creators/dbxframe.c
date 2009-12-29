mapping frame;

int cmd(string str) {
    object ob;
    string who;
    int num, i;
    string ret = "";

    if (!str || (sscanf(str, "%d", num)==0 && sscanf(str, "%s %d", who, num)==0)) {
        write("dbxframe [who] <frame>");
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

    ret += sprintf("------\n%s:%i - %s(%s)\n", frame["program"], frame["line"],
            frame["function"],frame["arguments"]);
    ret += sprintf("----------------------------------------------------------------\n");
    ret += sprintf("%s=>%s%s",
            unguarded( (: read_file(frame["program"], frame["line"]-5, 5) :) ),
            unguarded( (: read_file(frame["program"], frame["line"], 1) :) ),
            unguarded( (: read_file(frame["program"], frame["line"]+1, 5) :) ));
    tell_object(this_player(),ret);
    return 1;
}

string GetHelp(){
    return "Syntax: dbxframe [who] <frame>\n\n"
        "Some kind of thing.";
}
