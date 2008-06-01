#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mapping FilesMap = ([]);

varargs int cmd(string str){
    object wget = new(WGET_D, this_object()); 
    if(!FilesMap) FilesMap = ([]);
    write("Requesting url: "+str);
    FilesMap[wget] = this_player();
    wget->eventGet(str);
    return 1;
}

varargs mixed eventReceiveWebData(string content, string file){
    object wget = previous_object();
    object who;
    string savefile;
    if(!FilesMap[wget] || !(who = FilesMap[wget])){
        map_delete(FilesMap, wget);
        return 1;
    }
    savefile = user_path(who)+last_string_element(file,"/");
    who->eventPrint("File received: "+file+" , saving to: "+savefile);
    write_file(savefile,content,1);
    return 1;
}

int help()
{
    write( @EndText
Syntax: wget <url>
Effect: Attempts to download internet content to your home directory.
Examples:
wget http://dead-souls.net/RELEASE_NOTES
wget http://149.152.218.102/RELEASE_NOTES -n dead-souls.net

The second syntax may be necessary if your mud does not use name
resolution and the target wbesite uses virtual hosts.
NOTE 1: This command is for text files only!
NOTE 2: You may have to manually remove some header information.

See also: resolve
EndText
    );
    return 1;
}
