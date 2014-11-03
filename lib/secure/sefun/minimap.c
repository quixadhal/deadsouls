string query_door(object what, string direction){
    string door = what->GetDoor(direction);
    object puerta;
    if(!door) return "#";
    else {
        puerta = load_object(door);
        if(!puerta) return "#";
        if(puerta->GetClosed()) return "O";
        return "#";
    }
    return " ";
}

varargs string simple_map(mixed arg){
    string *exits;
    string *extra_desc = ({});
    string ret = "";
    string line0 = "---------\n";
    string line6 = "---------\n";
    string line1 = "|       |\n";
    string line2, line3, line4, line5;
    line2 = line3 = line4 = line5 = line1;
    if(!arg && !this_player()) return "";
    if(!arg) arg = environment(this_player());
    if(stringp(arg)) arg = load_object(arg);
    if(living(arg) || arg->GetMount()) arg = environment(arg);
    if(!arg) return "";
    exits = arg->GetExits();
    if(!exits || !sizeof(exits)) return "";
    line3[4..4] = "*";

    foreach(string element in exits){
        switch(element){
            case "north" : line1[4..4] = query_door(arg, element); line2[4..4] = "|";break;
            case "south" : line5[4..4] = query_door(arg, element); line4[4..4] = "|";break;
            case "east" : line3[7..7] = query_door(arg, element); line3[5..5] = "-";line3[6..6] = "-";break;
            case "west" : line3[1..1] = query_door(arg, element); line3[2..2] = "-";line3[3..3] = "-";break;
            case "northeast" : line1[7..7] = query_door(arg, element); line2[6..6] = "/";break;
            case "northwest" : line1[1..1] = query_door(arg, element); line2[2..2] = "\\";break;
            case "southeast" : line5[7..7] = query_door(arg, element); line4[6..6] = "\\";break;
            case "southwest" : line5[1..1] = query_door(arg, element); line4[2..2] = "/";break;
                               //case "up" : extra_desc += ({ "up" });break;
                               //case "down" : extra_desc += ({ "down" });break;
            default : extra_desc += ({ element });break;
        }
    }
    if(sizeof(extra_desc)){
        line3 = replace_string(line3,"\n"," Also available: "+item_list(extra_desc)+".\n");
    }
    ret = line0 + line1 + line2 + line3 + line4 + line5 +line6;
    return ret;
}
