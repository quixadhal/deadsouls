/*    /lib/history.c
 *    from the Dead Souls  Object Library
 *    handles command line histories
 *    created by Descartes of Borg 960511
 */

#include <message_class.h>
#include "include/history.h"

private int HistorySize = MIN_HISTORY_SIZE;
private static int CommandNumber = 1;
private mapping History = ([]);

static string eventHistory(string str){
    string cmd, args, old, neu;
    int len, num;
    CommandNumber = sizeof(History);

    if( str[0] == '^' ) str = "!!" + str;
    if( (len = strlen(str)) < 2 || str[0] != '!' ) return Push(str);
    if( str[0..1] == "!!" || str[0..2] == "!-1"){
        cmd = GetHistory(CommandNumber-1);
        if( str[1] == '!' ){
            if( len == 2 ) args = "";
            else args = str[2..];
        }
        else {
            if( len == 3 ) args = "";
            else args = str[3..];
        }
    }
    else {
        int x;

        if( str[1] == '-' ){
            sscanf(str, "!-%d%s", x, args);
            if( !x ) cmd = "";
            else cmd = GetHistory(CommandNumber-x);
        }
        else if( sscanf(str, "!%d%s", x, args) == 2 ){
            if( !x ) cmd = "";
            else cmd = GetHistory(x);
        }
        else {
            str = str[1..];
            cmd = GetHistory(str);
            args = "";
        }
    }
    if( !cmd || cmd == "" ){
        eventPrint("Invalid history command.", MSG_ERROR);
        return "";
    }
    if( !args || args == "" ) return Push(cmd);
    len = strlen(args);
    if( args[0] != '^' && args[0] != 's' ){
        if( len > 1 && args[0] == '\\' && (args[1] == '^' || args[1] == 's') )
            args = args[1..];
        return Push(cmd + args);
    }
    if( len < 3 ){
        if( args[0] == 's' ){
            if( len < 2 || args[1] != '/' ) return Push(cmd + args);
        }
        eventPrint("Invalid replacement syntax.", MSG_ERROR);
        return "";
    }
    if( args[0] == '^' ){
        int i;

        args = args[1..];
        i = strsrch(args, "^");
        if( i == 0 ) i = -1;
        while( i != -1 && args[i-1] == '\\' ){
            args = args[0..i-2] + args[i..];
            if( strlen(args) == i+1 ) i = -1;
            else i = strsrch(args, "^", i+1);
        }
        if( i == -1 ){
            eventPrint("Invalid replacement syntax.", MSG_ERROR);
            return "";
        }
        old = args[0..(i-1)];
        if( i == strlen(args) - 1 ) neu = "";
        else neu = args[(i+1)..];
        num = 1;
        neu = replace_string(neu, "\\^", "^");
    }
    else {
        int i;

        args = args[2..];
        i = strsrch(args, "/");
        if( i == 0 ) i = -1;
        while( i != -1 && args[i-1] == '\\' ){
            args = args[0..i-2] + args[i..];
            if( strlen(args) == i+1 ) i = -1;
            else i = strsrch(args, "/", i+1);
        }
        if( i == -1 ){
            eventPrint("Invalid replacement syntax.", MSG_ERROR);
            return "";
        }
        old = args[0..(i-1)];
        if( i == strlen(args) - 1 ) neu = "";
        else neu = args[(i+1)..];
        len = strlen(neu);
        if( len > 1 ){
            if( neu[<2..] == "/g" ){
                if( len > 2 && neu[<3] != '\\' ){
                    num = 0;
                    neu = neu[0..<3];
                }
                else if( len > 2 ) num = 1;
                else {
                    num = 0;
                    neu = "";
                }
            }
        }
        else num = 1;
        neu = replace_string(neu, "\\/", "/");
    }
    cmd = replace_string(cmd, old, neu, num);
    return Push(cmd);
}

static string Push(string cmd){
    int j, crunch;
    CommandNumber = sizeof(History);
    if(CommandNumber && History[CommandNumber-1] == cmd){
        return cmd;
    }
    if(member_array(cmd, values(History)) != -1){
        foreach(mixed key, mixed val in History){
            if(cmd == val){
                History[key] = 0;
            }
            crunch = 1;
        }
    }
    if(crunch){
        mapping newmap = ([]);
        j = 0;
        for(int i = 0; i < CommandNumber; i++){
            if(History[i]){
                newmap[j] = History[i];
                j++;
            } 
        }
        History = newmap;
        CommandNumber = sizeof(History);
        return cmd;
    }
    History[CommandNumber] = cmd;
    if(CommandNumber > HistorySize){
        mapping newmap = ([]);
        foreach(mixed key, mixed val in History){
            if(key > 0) newmap[key-1] = val;
        } 
        History = newmap;
    }
    CommandNumber = (sizeof(History) - 1);
    return cmd;
}

int GetCommandNumber(){
    return CommandNumber;
}

string GetHistory(mixed val){
    if(!this_player() || this_player() != this_object()
            || this_player()->GetForced()) return "";
    if( intp(val) ){
        if( !History[val] ) return "";
        return History[val];
    }
    else if( stringp(val) ){
        foreach(mixed key, mixed what in History){
            if( strsrch(what, val) == 0 ) return what;
        }
        return "";
    }
    else error("Invalid argument to GetHistory().\n");
}

mapping GetHistoryList(){
    if( !(master()->valid_apply(({ GetKeyName() }))) ) return ([]);
    return copy(History);
}

mapping GetCommandHist(){
    return GetHistoryList();
}

int SetHistorySize(int x){
    if( !(master()->valid_apply(({ GetKeyName() }))) )
        return HistorySize;    
    if( x == HistorySize ) return HistorySize;
    if( x > MAX_HISTORY_SIZE ) return HistorySize;
    else if( x < MIN_HISTORY_SIZE ) return HistorySize;
    return (HistorySize = x);
}

string GetLastCommand(){
    if(!this_object()->GetForced() && 
            (this_player() == this_object() || previous_object() == master())){
        return History[sizeof(History)-1];
    }
    else return "";
}

int GetMaxCommandHistSize(){
    return MAX_HISTORY_SIZE;
}

int SetMaxCommandHistSize(int i){
    SetHistorySize(i);
    return HistorySize;
}

