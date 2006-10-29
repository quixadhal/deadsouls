#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int cased = 1;
    int strict = 0;
    string ret = "";
    string all_files = FILE_D->GetFiles();

    if(!str || str == ""){
	write("Please specify the filename or substring you're looking for.");
	return 1;
    }

    if(grepp(str,"-i ")){
	cased = 0;
	str = replace_string(str,"-i ","");
    }

    if(grepp(str,"-s ")){
	strict = 1;
	str = replace_string(str,"-s ","");
    }

    foreach(string element in all_files){
	string tmpstr, tmpelement;

	if(!cased){ 
	    tmpelement = lower_case(element);
	    tmpstr = lower_case(str);
	} 
	else {
	    tmpelement = element;
	    tmpstr = str;
	}  

	if(!strict && grepp(last_string_element(tmpelement,"/"), tmpstr)) ret += element + "\n"; 
	else if(strict && last_string_element(tmpelement,"/") == tmpstr) ret += element + "\n";
    }
    write("Matches:");
    if(sizeof(ret)) write(ret);
    else write("None.");
    return 1;
}

string GetHelp(string str) {
    return "Syntax: find [-s] [-i] <name>\n"
    "This command searches the list of files known to the file daemon for files "
    "matching the specified search parameter. This list of files is refreshed approximately "
    "once every 25 hours automatically, or every time the mud boots, or every time the "
    "file daemon is reloaded.\n"
    "Options:\n"
    "-s Strict matching, aka not substrings.\n"
    "-i Case insensitive\n"
    "Examples\n"
    "find FooBar: finds FooBar, and FooBar.h, but not foobar or FOObar.h\n"
    "find -i FooBar: finds foobar, FooBar, FooBarBaz, and Foobar.c\n"
    "find -s FooBar: finds FooBar, but NOT FooBar.c or foobarbaz\n" 
    "find -i -s FooBar: finds fooBAR, and FooBar, but not FooBar.c\n"
    "";
}
