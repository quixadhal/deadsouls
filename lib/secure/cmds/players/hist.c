#include <lib.h>
#include <daemons.h>
#include <commands.h>
#include <message_class.h>

inherit LIB_DAEMON;
string gfile;

varargs string *Prettify(string *arr, string chan){
    string *ret = ({});
    string colorchan = "", stamp, mess, junk, remotechan;
    chan = CHAT_D->GetLocalChannel(chan);
    remotechan = CHAT_D->GetRemoteChannel(chan);
    if(!chan || !(CHAT_D->GetTag(chan))){
        if(!(colorchan = CHAT_D->GetTag(remotechan))){
            colorchan = "%^B_BLACK%^YELLOW%^<"+chan+">%^RESET%^";
        }
    }
    else colorchan += "<" + chan + ">%^RESET%^";

    foreach(string line in arr){
        int i = sscanf(line,"%s.%s]%s", junk, stamp, mess);
        if(i != 3) continue;
        stamp = replace_string(stamp, ".", "/", 1);
        stamp = replace_string(stamp, ".", ":", 1);
        mess = replace_string(mess, "<"+ chan +">", colorchan, 1);
        mess = replace_string(mess, "<"+ remotechan +">", colorchan, 1);
        ret += ({ stamp + " " + mess });
    }
    return ret;
}

mixed cmd(string args) {
    string *talks, *ret = ({});
    int i, lines;
    string log_contents, location;
    gfile = "";

    if(!args){ 
        this_player()->eventPrint("Syntax: <hist [channel | say]>");
        return 1;
    }

    this_player()->eventPrint("Retrieving history...\n");

    if(!strsrch(args, "tell")){
        load_object(CMD_TELL)->cmd("hist");
        return 1;
    }

    if(!LOG_REMOTE_CHANS && 
            member_array(args, CHAT_D->GetRemoteChannels(1)) != -1){
        ret = CHAT_D->cmdLast(args);
        if(sizeof(ret)) return ret;
    }

    if(!LOG_LOCAL_CHANS && 
            member_array(args, CHAT_D->GetRemoteChannels()) == -1){
        ret = CHAT_D->cmdLast(args);
        if(sizeof(ret)) return ret;
    }

    i = sscanf(args,"%s %d", gfile, lines);
    if(i != 2){
        lines = 20;
        gfile = args;
    }
    talks = this_player()->GetTalkHistTypes();
    if(talks && member_array(gfile, talks) != -1){
        string ret2 = "Your "+gfile+" history: \n\n";
        ret2 += implode(this_player()->GetTalkHistory(gfile),"\n");
        print_long_string(this_player(), ret2);
        return 1;
    }

    if(!CHAT_D->CanListen(this_player(), gfile)){
        return "You do not have privileges to: "+gfile;
    }
    if(!CHAT_D->GetListening(this_player(), gfile)){
        return "You are not listening to channel: "+gfile;
    }
    if(gfile == "admin"){
        location = "/secure/log/admin";
    }
    else {
        location = DIR_CHANNEL_LOGS +"/"+ gfile;
    }
    if( !(log_contents = read_file(location)) ){
        return CHAT_D->cmdLast(gfile);
    }
    if(lines > 100 && !creatorp(this_player())) lines = 100;
    if(sizeof(ret = (Prettify(explode(log_contents,"\n"), gfile))) < lines){
        string *archive_array;
        archive_array = (get_dir(DIR_CHANNEL_LOGS +"/archive/") || ({}) );
        archive_array = filter(archive_array, (: !strsrch($1,gfile) :) );
        archive_array = sort_array(archive_array, -1);
        foreach(string element in archive_array){
            string junk1, tmp;
            if(sizeof(ret) >= lines) break;
            if(sscanf(element,"%s%*d.%*d.%*d-%*d.%*d", junk1) != 6) continue;
            if(truncate(junk1,1) != gfile) continue;
            tmp = read_file(DIR_CHANNEL_LOGS +"/archive/"+element);
            if(!tmp) continue;
            ret = Prettify(explode(tmp,"\n"), gfile)+ret;
        }
    }
    ret = ret[<lines..];
    this_player()->eventPage(ret, "system");
    return 1;
}

string GetHelp(){
    string ret = "Syntax: hist <CHANNEL> [number]\n"
        "        hist [ say | whisper | tell | yell | shout ]\n\n"
        "Gives you the backscroll of the specified communication.\n";
    if(!LOG_REMOTE_CHANS){
        ret += "Note that remote channel logging is not enabled on "
            "this mud, so hist for intermud channels will not work.";
    }
    return ret;
}
