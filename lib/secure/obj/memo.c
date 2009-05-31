/* memo pad by Cratylus @ Frontiers
 * Sep 21 2005
 */

int read_memo(string str);
int help_msg();

string ReadFun(mixed args...){
    help_msg();   
    return "";
} 

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
int dirchecked;
string ownerstr,capownerstr,fileread;
object ownerob;
void create(){
    ::create();
    SetKeyName("memo pad");
    SetId( ({"pad","memo","memo pad","notepad"}) );
    SetShort("a yellow memo pad");
    SetLong("A yellow pad of papers to write your memos on. "+
            "There seems to be instructions written on it.");
    SetMass(20);
    SetVendorType(VT_TREASURE);
    SetItems(([ ({"instruction", "instructions", "words"}) : "Words that "+
                "explain how to use the notepad. Try: read instructions on notepad.",
                ]) );
    SetRead( ([ ({"instruction", "instructions", "words"}) : (: ReadFun :),
                "default" : (: read_memo :),
                ]) );
}

void init(){
    ::init();
    add_action("memo","memo");
    add_action("clean_memo","fresh");
    add_action("erase_memo","erase");
    ownerob=environment(this_object());
    if(living(ownerob)) capownerstr=capitalize(ownerob->GetKeyName());
    if(capownerstr) ownerstr=lower_case(capownerstr);
    dirchecked=0;
}
int memo(string str){
    if(!dirchecked){
        this_object()->checkdir();
    }
    if(!str){
        write("You do not write anything on your memo pad.\n");
        return 1;
    }
    write_file(homedir(this_player())+"/log/memo",str+"\n");
    write("You add a memorandum to your memo pad.\n");
    say(capownerstr+" scribbles in "+possessive(this_player())+" memo pad.");
    return 1;
}
int read_memo(mixed str...){
    if(!dirchecked){
        this_object()->checkdir();
    }
    fileread=read_file(homedir(this_player())+"/log/memo");
    if(!fileread){
        write("The memo pad is blank.\n");
        write("For instructions: read instructions on memo pad");
        return 1;
    }
    write("The pad reads:\n"+fileread+"\n");
    say(capownerstr+" flips through "+possessive(this_player())+" memo pad.\n");
    return 1;
}
int help_msg(){
    write("The back of the pad reads:\n\n"+
            "cReMem Memo Pad, For your note-taking needs! Memo pad commands:\n"+
            "----------\n"+
            "memo <string> : Writes <string> into the pad.\n"+
            "read memo      : Reads the contents of your memo pad.\n"+
            "----------\n");
    say(capownerstr+" reads the back of a memo pad.\n");
    return 1;
}
int checkdir(){
    if(file_size(homedir(this_player())+"/log") != -2) mkdir(homedir(this_player())+"/log");
    dirchecked=1;
    return 1;
}
