/* Please note this info from options.h
 * REF_RESERVED_WORD: If this is defined then the word 'ref' can be
 *   used to pass arguments to functions by reference.  Example:
 *
 * void inc(int ref x) {
 *     x++;
 * }
 *
 * ... y = 1; inc(ref y); ...
 *
 * A side effect is that 'ref' cannot be a variable or function name.
 *
 * Note: ref must be used in *both* places; this is intentional.  It protects
 * against passing references to routines which don't intend to return values
 * through their arguments, and against forgetting to pass a reference
 * to a function which wants one (or accidentally having a variable modified!)
 */

#include <lib.h>

inherit LIB_ITEM;
mixed globalvar;

static void create() {
    item::create();
    SetKeyName("object");
    SetId( ({ "example", "sample" }) );
    SetAdjectives( ({ "example","sample","pass by reference" }) );
    SetShort("a pass by reference example object");
    SetLong("This object allows you to test passing by reference. "
            "When you: \n\n call example->StartPass(1)\n\n\tYou can see that "
            "not only has the local variable been changed, but the "
            "global variable too, even though there has been no assignment "
            "operation to change globalvar to localvar's new value.\n"
            "\tThis pass by reference is accomplished by using the \"ref\" "
            "keyword. If you need to pass an integer or a string by reference, "
            "study how it is done here to understand the process.");
    SetNoCondition(1);
    SetMass(50);
    SetBaseCost(1);
}
void init(){
    ::init();
}

mixed PassByRef(mixed ref arg){
    if(intp(arg)) arg++;
    else if(stringp(arg)) arg += "localvar";
    else if(mapp(arg)) arg[identify(this_object())] = this_object();
    else if(arrayp(arg) && sizeof(arg)){
        if(stringp(arg[0])) arg += ({ "localvar" });
        else if(intp(arg[0])) arg += ({ 1 });
        else if(objectp(arg[0])) arg += ({ this_object() });
    }
    return arg;
}

mixed StartPass(mixed localvar){
    globalvar = localvar;
    write("localvar: "+identify(localvar));
    write("globalvar: "+identify(globalvar));
    flush_messages();
    localvar = PassByRef(ref globalvar);
    write("localvar: "+identify(localvar));
    write("globalvar: "+identify(globalvar));
    return 1;
}
