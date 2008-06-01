/* This is a temporary object that holds a 
 * player's interactive link while her old body
 * is being destroyed and before her new one is
 * created. This should only be needed when the
 * player needs to be reloaded without quitting.
 */

#include <lib.h>
#include <daemons.h>

private static string keyname;

string SetKeyName(string str){
    if(base_name(previous_object()) != RELOAD_D) return "";
    keyname = str;
    return keyname;
}

string GetKeyName(){
    return keyname;
}
