#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str)
{
    string thing, file, *inheriteds;
    object ob;
    if(!str || sscanf(str,"%s -f %s",thing,file) != 2){
        write("Syntax: inherits THING -f FILE");
        return 1;
    }
    ob = get_object(thing);
    if(!ob){
        write("Sorry I can't find that thing.");
        return 1;
    }

    if(last(file,2) == ".c") file = truncate(file,2);

    if(!strsrch(file,"LIB_") || !strsrch(file,"lib_")){
        string tmpfile =
        ""+
        "#include <lib.h>\n"+
        "#include <privs.h>\n"+
        "#include <cfg.h>\n"+
        "#include <save.h>\n"+
        "#include <rooms.h>\n"+
        "#include <config.h>\n"+
        "#include <daemons.h>\n"+
        "#include <objects.h>\n"+
        "#include <commands.h>\n"+
        "#include <armor_types.h>\n"+
        "#include <damage_types.h>\n"+
        "#include <position.h>\n"+
        "#include <runtime_config.h>\n"+
        "#include <terrain_types.h>\n"+
        "#include <body_types.h>\n"+
        "#include <size_types.h>\n"+
        "#include <respiration_types.h>\n"+
        "#include <message_class.h>\n"+
        "#ifndef "+upper_case(file)+"\n"+
        "#define "+upper_case(file)+" 0\n"+
        "#endif\n"+
        "inherit LIB_ITEM;\n"+
        "string GetInheritable() { return " + upper_case(file) + "; }\n"+
        "";
        string filename = "/open/"+this_player()->GetKeyName()+"_inherits.c"; 
        write_file( filename, tmpfile,1 );
        reload(filename);
        file = (mixed)call_other(filename, "GetInheritable");
        if(!file) file = load_object(filename)->GetInheritable();
    }
    write("Checking "+identify(ob)+" for inheritance of "+file+" , just a moment...");
    if(file){
        if(inherits(file, ob)){
            write(ob->GetShort()+" inherits "+file+".");
            return 1;
        }
        inheriteds = deep_inherit_list(ob);
        foreach(string item in inheriteds){
            if(!grepp(item, file)) inheriteds -= ({ item });
        }
    }
    if(!sizeof(inheriteds)){
        write("No matches found. "+ob->GetShort()+" does not appear to inherit that file.");
        return 1;
    }
    write("Exact match not found, but here are some possible matches: ");
    write(implode(inheriteds,"\n"));
    return 1;
}

void help()
{
    write("Syntax: inherits THING -f FILE\n\nTries to match FILE with the list of inherited files for THING.\n\nSee also: showtree, findobj");
}
