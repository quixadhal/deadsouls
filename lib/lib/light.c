/*    /lib/light.c
 *    from the Dead Souls V LPC Library
 *    a standard lighting object
 *    created by Descartes of Borg 960512
 *    Version: @(#) light.c 1.2@(#)
 *    Last modified: 96/11/03
 */

#include <vendor_types.h>

private int Lit = 0;

// abstract methods
string GetKeyName();
// end abstract methods

static void create() {
    this_object()->AddSave(({ "Lit" }));
    this_object()->SetVendorType(VT_LIGHT);
}

int GetLit() {
    return Lit;
}

static int SetLit(int x) {
    return (Lit = x);
}

string GetShort() {
    /*  string str;

    if(!(str = item::GetShort()) || str == "") return str;
    return sprintf("%s%s", str, (GetLit() ? " (lit)" : ""));
    */
    if( GetLit() ) return " (lit)";
    else return "";
}

mixed CanLight(object who) {
    if( GetLit() ) {
	return "#It is already lit.";
    }
    return 1;
}

mixed direct_light_obj() {
    return CanLight(this_player());
}

mixed eventDarken() {
    object env;
    
    if( env = environment() ) {
	env->eventPrint("The " + GetKeyName() + " goes dark.");
	if( living(env) ) {
	    environment(env)->eventPrint(possessive_noun(env) + " " +
					 GetKeyName() + " goes dark.", env);
	}
    }
    SetLit(0);
    return 1;
}

varargs mixed eventLight(object who, object tool) {
    SetLit(1);
    return 1;
}

