/*    /lib/help.c
 *    From the Dead Souls Object Library
 *    Provides an interface to the help system for objects that want it
 *    Created by Descartes of Borg 960116
 *    Version: @(#) help.c 1.2@(#)
 *    Last modified: 96/12/17
 */

private static mapping Help = ([ "default" : "No help has been provided." ]);

string GetHelp(string topic){
    if( !topic || !Help[topic] ){
        topic = "default";
    }
    return Help[topic];
}

static mapping SetHelp(mixed val){
    if( stringp(val) ){
        val = ([ "default" : val ]);
    }
    return (Help = val);
}
