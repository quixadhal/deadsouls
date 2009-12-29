/*    /secure/cmds/creator/call.c
 *    from the Foundation II LPC Library
 *    allows you to call functions in objects ad hoc
 *    created by Descartes of Borg 950430
 */

#include <privs.h>
#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    mixed *arg_arr;
    function f;
    object target;
    mixed val;
    string arg_targ, arg_func, err;
    int i, maxi;

    if(!member_group(previous_object(), PRIV_SECURE)) {
        log_file("adm/call", query_privs(previous_object())
                +" ("+ctime(time())+"): call "+args+"\n");
    }
    if( !args || args == "" ) return "Call ob->func(arg1, arg2)\n";
    if( sscanf(args, "%s->%s(%s", arg_targ, arg_func, args) != 3 )
            return "Call ob->func(arg1, arg2)\n";
            args = trim(args);
            if( args != ")" ) args = args[0..<2];
            else args = "";
            if( !(target = to_object(arg_targ)) )
                return "Cannot identify any object as \"" + arg_targ + "\".";
            if( !function_exists(arg_func, target) )
                return "The function " + arg_func +"() is not in " +
                    identify(target) + "\n";
            f = (: call_other, target, arg_func :);
            if( args == "" ) {
                err = catch(val = evaluate(f));
                if( err) {
                    message("error", identify(target) + " -> " + arg_func + "()",
                            this_player());
                    message("error", "Error in execution: " + err, this_player());
                    return 1;
                }
                else {
                    message("system", identify(target) + " -> " + arg_func + "() = " +
                            identify(val), this_player());
                    return 1;
                }
            }
            arg_arr = convert_string("({"+args+"})");
            err = catch(val = call_other(target, ({ arg_func }) + arg_arr));
            args = identify(target) + " -> " + arg_func + "( ";
            for(i = 0, maxi = sizeof(arg_arr); i<maxi; i++) {
                args += identify(arg_arr[i]);
                if( i < maxi - 1 ) args += ", ";
            }
            if( err ) {
                message("error", args + ")", this_player());
                message("error", "Error in execution: " + err, this_player());
                return 1;
            }
            args += " ) = " + identify(val);
            message("system", args, this_player());
            return 1;
}

string GetHelp(){
    return ("Syntax: call object->function(args)\n\n"
            "Examples:\n\tcall me->GetName()\n"
            "\tcall \"/realms/descartes/workroom\"->GetProperty(\"light\")\n\n"
            "Allows you to call any function in any object with any set of "
            "arguments from the command line.  The syntax is identical to "
            "that you would use inside LPC code, except that you can refer "
            "to objects by their names or by pronouns in addition to the "
            "usual manners.\n"
            "See also: eval, gauge");
}
