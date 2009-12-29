#include <lib.h>
#include <dirs.h>
#include <vendor_types.h>
#include <daemons.h>

inherit LIB_DAEMON;

string original, digested;
mapping AreaMap = ([]);
mapping RawMap = ([]);
string array source_dirs = ({ "zon", "mob", "obj", "shp", "trg", "wld", "gld" });
mapping Genders = ([ 0 : "neutral", 1 : "male", 2 : "female" ]);
mapping Directions = ([ 0 : "north", 1 : "east", 2 : "south",
        3 : "west", 4 : "up", 5 : "down", 6 : "northwest", 7 : "northeast",
        8 : "southeast", 9 : "southwest", 10 : "in", 11 : "out" ]);
mapping Types = ([ 1 : "LIB_ITEM", 2 : "LIB_ITEM", 3 : "LIB_ITEM", 4 : "LIB_ITEM",
        5 : "LIB_ITEM", 6 : "LIB_ITEM", 7 : "LIB_ITEM", 8 : "LIB_ITEM", 9 : "LIB_ARMOR", 10 : "LIB_POTION",
        11 : "LIB_ITEM", 12 : "LIB_ITEM", 13 : "LIB_ITEM", 14 : "LIB_ITEM", 15 : "LIB_STORAGE", 
        16 : "LIB_ITEM", 17 : "LIB_MEAL", 18 : "LIB_ITEM", 19 : "LIB_MEAL", 20 : "LIB_PILE",
        21 : "LIB_ITEM", 22 : "LIB_ITEM", 23 : "LIB_ITEM", 24 : "LIB_ITEM", 25 : "LIB_ITEM", 
        26 : "LIB_ITEM", 44 : "LIB_ITEM" ]);
string gdirection, cle, val;
int item_type;
mixed com,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13;
int weight, cost, material,ok,ok2;
int lock, key, room, direction;

mapping ObjectType = ([
        "zon" : "ZONE",
        "obj" : "OBJECTS",
        "mob" : "MOBILES",
        "wld" : "ROOMS",
        ]);

mixed cmd(string args){
    if(args == "clear"){
        write("Resetting the converter variables.");
        RELOAD_D->eventReload(this_object(),0);
        return 1;
    }
    if(grepp(args,"-r ")){
        args = replace_string(args,"-r ","");
        return this_object()->Report(args);
    }
    return this_object()->ConvertArea(args);
}

string clean_string(string str){
    str = replace_string(str,"\"","\'",);
    return str;
}

varargs mixed QueryMaps(string which, string what){
    if(!which || !what){
        write("AreaMap: "+(AreaMap ? identify(keys(AreaMap)) : "0"));
        write("RawMap: "+(AreaMap ? identify(keys(AreaMap)) : "0"));
        return 1;
    }
    if(which == "area"){
        if(AreaMap && AreaMap[what]) return identify(AreaMap[what]);
        else return "No such element.";
    }
    if(which == "raw"){
        if(RawMap && RawMap[what]) return identify(RawMap[what]);
        else return "No such element.";
    }
    return "IDK.";
}

int Report(string str){
    print_long_string(this_player(),"vnums: "+item_list(keys(AreaMap)),1);
    if(str == "both" || str == "area") print_long_string(this_player(),"%^RED%^AreaMap: "+identify(AreaMap),1);
    if(str == "both" || str == "raw") print_long_string(this_player(),"%^GREEN%^RAWMap: "+identify(RawMap),1);
    if(sscanf(str,"%*d")) this_object()->DisplayVnum(str);
    return 1;
}

int DisplayVnum(string str){
    if(!str || !AreaMap[str]){
        write("No such vnum in this area.");
        return 1;
    }
    print_long_string(this_player(),"vnum "+str+":\n"+identify(AreaMap[str]),1);
    return 1;
}

int ConvertArea(string arg){
    string area, tmp, str, name, prefix;
    int i;
    string *room_arr;
    string *mob_arr;
    string *obj_arr;
    string *segmented = ({});

    if(!arg || sscanf(arg,"%s %s %s",str, area, name) != 3){
        write("convert PATH AREA NAME");
        return 1;
    }

    if(!str || !directory_exists(str)){
        write("No such area directory exists.");
        return 1;
    }

    if(directory_exists(DOMAINS_DIRS+"/"+name)){
        write("That domain already exists. Backing up the current domain to a unique name.");
        rename(DOMAINS_DIRS+"/"+name, DOMAINS_DIRS+"/"+name+"."+time());
    }

    foreach(string elemento in source_dirs){
        string path = str+"/"+elemento+"/"+area+"."+elemento;
        if(!file_exists(path)){
            write(path+" does not exist. Skipping.");
            continue;
        }

        original = read_file(path);
        digested = clean_string(original);

        segmented = explode(digested,"\n");
        if(!RawMap) RawMap = ([]);
        RawMap[ObjectType[elemento]]= segmented[0..];
    }


    /////
    // ITEMS
    /////

    tmp = implode(RawMap["OBJECTS"],"\n");
    prefix = DOMAINS_DIRS+"/"+name+"/obj/";
    obj_arr = explode(tmp,"#");
    foreach(mixed block in obj_arr){
        string *ob_id;
        string *lines = explode(block,"\n");
        string short, long, ob_name;
        string header = "";
        if(block && sizeof(lines) > 1){
            ob_id = explode(truncate(lines[1],1)," ");
            if(!sizeof(ob_id)) continue;
            ob_name = ob_id[0];
            short = truncate(lines[2],1);
            long = "";
            for(i=2;i < sizeof(lines);i++){
                int num = i;
                if(sscanf(lines[i],"%d %s %s %s %s %s %s %s %s %s %s %s %s",
                            v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13) == 13) {
                    item_type = v1;
                    continue;
                }
                if(sscanf(lines[i],"%d %d %d %d %d %d %d %d",v1,v2,v3,v4,v5,
                            v6,v7,v8) == 8){
                    material = v8;
                    continue;

                }
                if(sscanf(lines[i],"%d %d %d %d",v1,v2,v3,v4) == 4){
                    weight = v1;
                    cost = v2;
                    continue;
                }

                if(lines[i] == "E"){
                    for(num = i+2; num < sizeof(lines);num++){
                        if(lines[num] == "~") break;
                        long += lines[num]+" ";
                    }
                    break;
                }
            }
        }

        header += "#include <lib.h>\n\n";
        header += "inherit "+(Types[item_type] || "LIB_ITEM")+";\n\n";
        header += "static void create() {\n";
        header += "    ::create();\n";
        header += "    SetKeyName(\""+ob_name+"\");\n";
        header += "    SetId( "+identify(ob_id)+" );\n";
        header += "    SetShort(\""+short+"\");\n";
        header += "    SetLong(\""+long+"\");\n";
        header += "    SetMass("+(weight*10)+");\n";
        header += "    SetBaseCost("+cost+");\n";
        if (Types[item_type] == "LIB_STORAGE") {
            header += "    SetInventory( ([\n";
            header += "    ]) );\n";
        }
        header += "}\nvoid init(){\n::init();\n}\n";

        mkdir_recurse(truncate(prefix,1));
        if(directory_exists(truncate(prefix,1))){
            write_file(prefix+lines[0]+"_"+ob_name+".c",header,1);
        }
        else write("Directory "+truncate(prefix,1)+" does not exist.");

        if(!AreaMap) AreaMap = ([]);

        if(!AreaMap["i"+lines[0]]) AreaMap["i"+lines[0]] =  
            ([ prefix+lines[0]+"_"+ob_name+".c" : header ]);
        else  AreaMap[lines[0]][prefix+lines[0]+"_"+ob_name+".c"] = header;
    }

    /////
    // MOBS
    /////

    tmp = implode(RawMap["MOBILES"],"\n");
    prefix = DOMAINS_DIRS+"/"+name+"/npc/";
    mob_arr = explode(tmp,"#");
    foreach(mixed block in mob_arr){
        string *ob_id;
        string *lines = explode(block,"\n");
        string short, long, ob_name;
        string header = "";
        ok = 0;
        ok2 = 0;
        if(block && sizeof(lines) > 1){
            ob_id = explode(truncate(lines[1],1)," ");
            if(!sizeof(ob_id)) continue;
            ob_name = ob_id[0];
            short = truncate(lines[2],1);
            long = "";
            for(i=0;i < sizeof(lines);i++){
                int num = i;
                if(!ok && sscanf(lines[i],"%d %d %d %s %s",v1,v2,v3,v4,v5) == 5){
                    ok =1;
                }
                if( ok == 1 && sizeof(explode(lines[i]," ")) == 3 && 
                        sscanf(lines[i],"%d %d %d",v2, v3, v4) == 3){
                    ok++;
                }
                if(!ok2 && i > 3){
                    if(i > 4 && lines[i] == "~"){
                        ok2 = 1;
                        continue;
                    }
                    if(lines[i] != "~") long += lines[i]+" ";
                }
            }

            header += "#include <lib.h>\n\n";
            header += "inherit LIB_SENTIENT;\n\n";
            header += "static void create(){\n";
            header += "    ::create();\n";
            header += "    SetKeyName(\""+ob_name+"\");\n";
            header += "    SetId( "+identify(ob_id)+" );\n";
            header += "    SetShort(\""+short+"\");\n";
            header += "    SetLong(\""+long+"\");\n";
            header += "    SetLevel("+v1+");\n";
            header += "    SetRace(\"human\");\n";
            header += "    SetNoCondition(1);\n";
            header += "    SetGender(\""+Genders[v4]+"\");\n";
            header += "    SetInventory( ([\n";
            header += "    ]) );\n";

            header += "}\nvoid init(){\n::init();\n}\n";

            mkdir_recurse(truncate(prefix,1));
            if(directory_exists(truncate(prefix,1))){
                write_file(prefix+lines[0]+"_"+ob_name+".c",header,1);
            }
            else write("Directory "+truncate(prefix,1)+" does not exist.");

            if(!AreaMap) AreaMap = ([]);

            if(!AreaMap["m"+lines[0]]) AreaMap["m"+lines[0]] =
                ([ prefix+lines[0]+"_"+ob_name+".c" : header ]);
            else  AreaMap[lines[0]][prefix+lines[0]+"_"+ob_name+".c"] = header;
        }
    }
    /////
    // ROOMS
    /////

    tmp = implode(RawMap["ROOMS"],"\n");
    prefix = DOMAINS_DIRS+"/"+name+"/room/";
    room_arr = explode(tmp,"#");
    foreach(mixed block in room_arr){
        string *lines = explode(block,"\n");
        string short, long;
        string header = "#include <lib.h>\n";
        header += "#include ROOMS_H\n\n";
        header += "inherit LIB_ROOM;\n\n";
        header += "void create() {\n";
        header += "    room::create();\n";
        header += "    SetClimate(\"indoors\");\n";
        header += "    SetAmbientLight(30);\n";


        short = truncate(lines[1],1);
        long = "";
        for(i=2;i < sizeof(lines);i++){
            if(lines[i] == "~") break;
            long += lines[i]+" ";
        }

        header += "    SetShort(\""+short+"\");\n";
        header += "    SetLong(\""+long+"\");\n";
        header += "    SetExits( ([\n";

        for(i=i;i < sizeof(lines);i++){
            int rev = 1;
            lock = key = room = direction = 0;
            if(lines[i] == "S") break;
            if(sscanf(lines[i],"%d %d %d",lock, key, room) == 3){
                while(rev){
                    if(!sscanf(lines[((i-rev > -1) ? (i-rev) : 0)],"D%d",direction)) rev++;
                    else rev = 0;
                    if(rev > 20) rev = 0;
                }
                if(!rev){
                    if(room == 0){
                        gdirection = Directions[direction];
                        //continue;
                    }
                    else header += "    \""+Directions[direction]+"\" : \""+prefix+room+"\",\n";
                }
                else if(sizeof(gdirection) > 1){
                    if(room != 0) header += "    \""+gdirection+"\" : \""+prefix+room+"\",\n";
                    gdirection = "";
                }
            }
        }

        header += "    ]) );\n";
        header += "    SetInventory( ([\n";
        header += "    ]) );\n";
        header += "}\nvoid init(){\n::init();\n}\n";

        mkdir_recurse(truncate(prefix,1));
        if(directory_exists(truncate(prefix,1))){
            write_file(prefix+lines[0]+".c",header,1);
        }
        else write("Directory "+truncate(prefix,1)+" does not exist.");

        if(!AreaMap) AreaMap = ([]);

        if(!AreaMap[lines[0]]) AreaMap[lines[0]] =  ([ prefix+lines[0]+".c" : header ]);
        else  AreaMap[lines[0]][prefix+lines[0]+".c"] = header;
    }

    return 1;
}

string GetHelp() {
    message("help", "Syntax: cwgconv <source path> <areaname> <domain name>\n\n"
            "Tries to convert a CWG area into a Dead Souls domain.\n"
            "Example: cconv /tmp/areadir 3 newdomain\n"
            "This would try to create /domains/newdomain and convert the area described\n"
            "in /tmp/areadir/3 into LPC files in the new DS domain."
            "\n", this_player());
}

