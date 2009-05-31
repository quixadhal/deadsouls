/*    /secure/sefun/this_agent.c
 *    from the Dead Souls Mud Library
 *    if 0 is the arg, returns the current agent, other wise
 *    sets a new agent and returns the old
 *    created by Descartes of Borg 951021
 */

private static object Agent;

varargs object this_agent(mixed val) {
    object old_agent;

    if( !val ) return Agent;
    old_agent = Agent;
    if( intp(val) ) Agent = 0;
    else Agent = val;
    return old_agent;
}

