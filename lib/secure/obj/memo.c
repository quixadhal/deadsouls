/* memo pad by Cratylus @ Frontiers
 * Sep 21 2005
 */

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
      "There seems to be instructions on the reverse side.");
    SetMass(20);
    SetValue(10);
    SetVendorType(VT_TREASURE);
}
void init(){
    add_action("memo","memo");
    add_action("read_memo","read");
    add_action("clean_memo","fresh");
    add_action("erase_memo","erase");
    add_action("help_memo","help");
    add_action("exa_inst","examine");
    add_action("exa_inst","look");
    add_action("exa_inst","look at");
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
    write_file("/realms/"+ownerstr+"/log/memo",str+"\n");
    write("You add a memorandum to your memo pad.\n");
    say(capownerstr+" scribbles in "+possessive(this_player())+" memo pad.");
    return 1;
}
int read_memo(string str){
    if(str=="memo" || str=="memo pad" || str=="pad" || str=="notepad"){
        if(!dirchecked){
            this_object()->checkdir();
        }
        fileread=read_file("/realms/"+ownerstr+"/log/memo");
        if(!fileread){
            write("The memo pad is blank.\n");
            return 1;
        }
        write("The pad reads:\n"+fileread+"\n");
        say(capownerstr+" flips through "+possessive(this_player())+" memo pad.\n");
        return 1;
    }
    if(str=="instructions" || str=="instruction"){
        this_object()->help_msg();
        return 1;
    }
}
int help_mem(string str){
    if(str=="memo" || str=="memo pad" ||str=="pad"||str=="notepad"){
        this_object()->help_msg();
        return 1;
    }
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
int exa_inst(string str){
    if(str=="instruction" || str=="instructions" ||
      str=="at instruction" || str=="at instructions"){
        write("Instructions on the use of the memo pad.\n");
        return 1;
    }
}
int checkdir(){
    if(file_size("/realms/"+ownerstr+"/log") != -2) mkdir("/realms/"+ownerstr+"/log");
    dirchecked=1;
    return 1;
}
