#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
string section;
void create(){
    ::create();
    SetKeyName("coder manual");
    SetId( ({"manual","book","handbook","guide","textbook"}) );
    SetAdjectives( ({"leather-bound","creator's","creator","coder's","coder"}) );
    SetShort("a Creator's Manual");
    SetLong("This is a large, leather-bound text containing a "
      "wealth of knowledge for learning Creators. To learn how to "
      "use it, type: 'info manual'.");
    SetMass(20);
    SetDollarCost(1);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
    add_action("help_man","info");
    add_action("help_man","read");
    add_action("chap_man","chapter");
    add_action("index_man","index");
}
int help_man(string str){
    if(str=="manual"){
	this_player()->more("/doc/manual/man_help");
	return 1;
    }
}
int chap_man(string str){
    int num;
    if(sscanf(str,"%d",num) > 0 && num < 10){
	if(num > -1) str = "0"+str;
    }
    if(file_size("/doc/manual/chapter"+str) < 1){
	write("That doesn't seem to be a valid chapter number.");
	return 1;
    }
    this_player()->more("/doc/manual/chapter"+str);
    return 1;
}
int index_man(string str){
    if(!str){
	this_player()->more("/doc/manual/index");
	return 1;
    }
}
