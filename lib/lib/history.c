/*    /lib/history.c
 *    from the Dead Souls  Object Library
 *    handles command line histories
 *    created by Descartes of Borg 960511
 */

#include <config.h>
#include <message_class.h>
#include "include/history.h"

private int HistorySize = MIN_HISTORY_SIZE;
private static int CommandNumber = 1;
private static array History;

static void create() {
    if( HistorySize < MIN_HISTORY_SIZE ) HistorySize = MIN_HISTORY_SIZE;
    if( HistorySize > MAX_HISTORY_SIZE ) HistorySize = MAX_HISTORY_SIZE;
    History = allocate(HistorySize);
}

static string eventHistory(string str) {
    string cmd, args, old, neu;
    int len, num;

    if( str[0] == '^' ) str = "!!" + str;
    if( (len = strlen(str)) < 2 || str[0] != '!' ) return Push(str);
    if( str[0..1] == "!!" || str[0..2] == "!-1") {
	cmd = GetHistory(CommandNumber-1);
	if( str[1] == '!' ) {
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
	    
	if( str[1] == '-' ) {
	    sscanf(str, "!-%d%s", x, args);
	    if( !x ) cmd = "";
	    else cmd = GetHistory(CommandNumber-x);
	}
	else if( sscanf(str, "!%d%s", x, args) == 2 ) {
	    if( !x ) cmd = "";
	    else cmd = GetHistory(x);
	}
	else {
	    str = str[1..];
	    cmd = GetHistory(str);
	    args = "";
	}
    }
    if( !cmd || cmd == "" ) {
	eventPrint("Invalid history command.", MSG_ERROR);
	return "";
    }
    if( !args || args == "" ) return Push(cmd);
    len = strlen(args);
    if( args[0] != '^' && args[0] != 's' ) {
	if( len > 1 && args[0] == '\\' && (args[1] == '^' || args[1] == 's') )
	  args = args[1..];
	return Push(cmd + args);
    }
    if( len < 3 ) {
	if( args[0] == 's' ) {
	    if( len < 2 || args[1] != '/' ) return Push(cmd + args);
	}
	eventPrint("Invalid replacement syntax.", MSG_ERROR);
	return "";
    }
    if( args[0] == '^' ) {
	int i;

	args = args[1..];
	i = strsrch(args, "^");
	if( i == 0 ) i = -1;
	while( i != -1 && args[i-1] == '\\' ) {
	    args = args[0..i-2] + args[i..];
	    if( strlen(args) == i+1 ) i = -1;
	    else i = strsrch(args, "^", i+1);
	}
	if( i == -1 ) {
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
	while( i != -1 && args[i-1] == '\\' ) {
	    args = args[0..i-2] + args[i..];
	    if( strlen(args) == i+1 ) i = -1;
	    else i = strsrch(args, "/", i+1);
	}
	if( i == -1 ) {
	    eventPrint("Invalid replacement syntax.", MSG_ERROR);
	    return "";
	}
	old = args[0..(i-1)];
	if( i == strlen(args) - 1 ) neu = "";
	else neu = args[(i+1)..];
	len = strlen(neu);
	if( len > 1 ) {
	    if( neu[<2..] == "/g" ) {
		if( len > 2 && neu[<3] != '\\' ) {
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

nomask private static string Push(string cmd) {
    int x;

    x = (CommandNumber-1) % sizeof(History);
    History[x] = cmd;
    CommandNumber++;
    return cmd;
}

int GetCommandNumber() {
    return CommandNumber;
}

private string GetHistory(mixed val) {
    if( CommandNumber == 1 ) return "";
    if( intp(val) ) {
	if( val < 1 ) return "";
	else if( val > CommandNumber -1 ) return "";
	else if( val < (CommandNumber - sizeof(History)) ) return "";
	val = (val-1) % sizeof(History);
	return History[val];
    }
    else if( stringp(val) ) {
	int i, x;
	
	x = (CommandNumber-2) % sizeof(History);
	for(i = x; i>=0; i--) {
	    if( !History[i] ) continue;
	    if( strsrch(History[i], val) == 0 ) return History[i];
	}
	for(i = sizeof(History)-1; i>x; i--) {
	    if( !History[i] ) continue;
	    if( strsrch(History[i], val) == 0 ) return History[i];
	}
	return "";
    }
    else error("Invalid argument to GetHistory().\n");
}

string *GetHistoryList() {
    if( !((int)master()->valid_apply(({ GetKeyName() }))) ) return ({});
    return copy(History);
}

int SetHistorySize(int x) {
    if( !((int)master()->valid_apply(({ GetKeyName() }))) )
      return HistorySize;    
    if( x == HistorySize ) return HistorySize;
    if( x > MAX_HISTORY_SIZE ) return HistorySize;
    else if( x < MIN_HISTORY_SIZE ) return HistorySize;
    CommandNumber = 1;
    return sizeof(History = allocate(HistorySize = x));
}
