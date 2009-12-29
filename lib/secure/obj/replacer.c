// string replacer, by Cratylus.
// Use this carelessly at your own peril.

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_TURN;

string a1,a2,a3,line;
int n, active;

static private void validate() {
    if(!this_player()) return 0;
    if( !(master()->valid_apply(({ "SECURE" }))) || !securep(this_player()))
        error("Illegal attempt to use replacer: "+get_stack()+" "+identify(previous_object(-1)));
}

void create(){
    ::create();
    SetKeyName("string replacer");
    SetId(({"replacer","thing","gadget","string replacer","device"}));
    SetShort("a string replacer");
    SetLong("This is the most dangerous device on the mud.\n"+
            "It allows you to replace strings in files.\n"+
            "To use it, you must first \"turn on replacer\"\n"+
            "Then you have two options:\n"+
            "To replace a word in one file: replace <filename> <oldword> <newword>\n"+
            "To replace a word in EVERY FILE IN YOUR CURRENT WORKING DIRECTORY:\n autorep <oldword> <newword>\n\n"+
            "It does not handle spaces or special characters well. Using "+
            "strings with parentheses, brackets, or anything other than "+
            "the english alphabet may have unexpected results."+
            "\n%^RED%^%^BOLD%^ADMIN USE ONLY!%^RESET%^");
    SetMass(20);
    SetValue(10);
    SetVendorType(VT_TREASURE);
    active = 0;
}

void init(){
    add_action("rep_string","replace");
    add_action("autorep","autorep");
}

int eventActivate(){
    validate();
    write("With an unearthly roar, the string replacer comes "+
            "to life. You sense great danger.");
    write("\n\nWARNING: Your current working directory is:\n\n");
    write(this_player()->query_cwd()+"\n\n");
    say(this_player()->GetName()+" turns on "+possessive(this_player())+
            " string replacer and you hear a loud, unearthly "+
            "roar that makes the ground tremble.");
    active = 1;
    return 1;
}

int eventDeactivate(){
    validate();
    write("The string replacer powers off with a peculiar "+
            "mechanical whine. You feel safer.");
    say(this_player()->GetName()+"'s string replacer "+
            "powers down.");
    active = 0;
    return 1;
}

varargs mixed eventTurn(string str){
    validate();
    if( this_player() != environment() ) { write("You don't have that."); return 0; }
    write("You turn the replacer over in your hands.");
    say(this_player()->GetKeyName()+" turns over a string replacer in "+possessive(this_player())+" hands.");
    return 1;
}

int eventTurnOn(object ob){
    validate();

    if( this_player() != environment() ) { write("You don't have that."); return 0; }

    if(!creatorp(this_player())) {
        write("This device is not usable by mortals.");
        return 0;
    }

    if(active){
        write("It's already on.");
        return 0;
    }
    else eventActivate();

    return 1;
}

varargs mixed eventTurnOff(string str){
    validate();
    if( this_player() != environment() ) { write("You don't have that."); return 0; }
    if(!active){
        write("It's already off.");
        return 0;
    }
    else eventDeactivate();

    return 1;
}

int autorep(string str){
    string dir, a1, a2;
    string *listing;

    validate();
    if(!active){
        write("The string replacer is not turned on.");
        return 1;
    }

    if( !str || str=="" || sscanf(str,"%s %s",a1,a2) != 2){
        write("Syntax: autorep <oldword> <newword>\n");
        eventDeactivate();
        return 1;
    }
    dir=this_player()->query_cwd()+"/";
    listing=get_dir(dir);
    foreach(string file in listing){
        if(file_size(dir+file) > 0) this_object()->rep_string(file+" "+str);
    }
    eventDeactivate();
    return 1;
}

int rep_string(string str){
    string dir,file,tempfile,backup;
    int valid_line;

    validate();
    if(!active){
        write("The string replacer is not turned on.");
        return 1;
    }

    valid_line=1;
    n=1;
    if( !str || str==""){
        write("Syntax: replace <filename> <oldword> <newword>\n");
        eventDeactivate();
        return 1;
    }
    if(sscanf(str,"%s %s %s",a1,a2,a3) == 3){
        dir=this_player()->query_cwd()+"/";
        file=dir+a1;

        if(file_size(file) <= 0 ) {
            write("Invalid filename.");
            eventDeactivate();
            return 1;
        }

        tempfile="/tmp/"+lower_case(this_player()->GetName())+".TempReplace";
        backup="/tmp/"+lower_case(this_player()->GetName())+"."+a1+".bak";
        if(file_size(backup)){ rm(backup); }
        if(file_size(tempfile)){ rm(tempfile); }
        write("Searching "+a1+" for string: "+a2+".\n");
        while(valid_line){
            line=read_file(file, n, 1);
            if(!line || line=="") valid_line=0;
            this_object()->replace(line);
            if(line) write_file(tempfile,line);
            n++;
        }
        cp(file, backup);
        cp(tempfile, file);
        if(file_size(backup)){ rm(backup); }
        if(file_size(tempfile)){ rm(tempfile); }
        write("Replace action complete. File "+file+" modified.\n");
        if(query_verb() == "replace") eventDeactivate();
        return 1;
    }
    write("Syntax: replace <filename> <oldstring> <newstring>\n");
    eventDeactivate();
    return 1;
}

int replace(string str){
    string s1,s2;
    validate();
    if(sizeof(str) && sscanf(str,"%s"+a2+"%s",s1,s2)>1){
        line=s1+a3+s2;
        write("Found string in line "+n+". Replacing with: "+a3+".\n");
        this_object()->replace(line);
    }
    return 1;
}
