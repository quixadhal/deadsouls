/*   /secure/sefun/to_object.c
 *   from the Dead Soulsr2.4 Library
 *   recreated by Blitz@Dead Souls
 *
 *   Purpose: Parse passed string argument and
 *            return a matching object pointer
 *            if one is found.
 *
 *   Usage Examples:
 *     sword           (1st sword in inv or env)
 *     sword@here      (find sword in local env)
 *     sword#4         (find 4th sword)
 *     sword#2@foo     (find 2nd sword on player foo)
 *     sword@bag#2@foo (find 1st sword in foo's 2nd bag)
 *     ---------------
 *     /path/filename  (find or load filename)
 *     /path/file#999  (find unique cloned object)
 *     %foo            (explicitly find player foo)
 *     $foo            (explicitly find npc foo)
 *     @foo            (return foo's environment)
 *     
 *     tokens: me, here, sefun
 */

#include <global.h>
 
object to_object(mixed var) {
    object env, ob, *obs;
    string tmp, where;
    int i, char;
 
    if( objectp(var) ) return var;
    else if( !stringp(var) || !sizeof(var) ) return 0;
    if( sscanf(var, "%s@%s", tmp, where) == 2 ) {
        var = tmp;
        if( !env = to_object(where) ) return 0;
        if( !sizeof(var) ) return environment(env);
    }
    switch(var) {
        case "here" : return environment(this_player());
        case "me" : return this_player();
        case "sefun" : return find_object(SEFUN);
    }
    if( sscanf(var, "%s#%d", tmp, i) == 2 ) {
        if( ob = find_object(var) ) return ob;
        else var = tmp;
    }
    else i = 1;
    if( i < 1 ) return 0;
    if( member_array(var[0], ({ '/', '$', '%', }) ) > -1 ) {
      char = var[0];
      var = var[1..];
    }
    switch( char ) {
        case '/' :
          if( var[<2..] != ".c" ) var += ".c";
          if( !file_exists(var) ) return 0;
          return load_object(var);
        break;
        case '%' :
          if( !(ob = find_player(var)) ) return 0;
          else return ob;
        break;
        case '$' :
          if( env )  {
              obs = filter(all_inventory(env),
                    (: living($1) && !interactive($1) && $1->id($(var)) :));
              if( sizeof(obs) < i ) return 0;
              else return obs[i - 1];
          }
          if( (ob = find_living(var)) && !interactive(ob) ) return ob;
          else return 0;
        break;
        default:
          if( !env ) {
	      env = this_player();
	  }
          if( i > 1 ) {
              obs = filter(all_inventory(env), (: $1->id($(var)) :));
              if( sizeof(obs) < i ) {
                  if( !env = environment(env) ) {
		      ob = 0;
		  }
		  else {
		      obs = filter(all_inventory(env), (: $1->id($(var)) :));
		      if( sizeof(obs) < i ) {
			  ob = 0;
		      }
		      else {
			  ob = obs[i - 1];
		      }
		  }
              }
	      else {
		  ob = obs[i-1];
	      }
	      if( ob ) {
		  return ob;
	      }
          }
	  else {
	      ob = present(var, env);
	      if( ob ) return ob;
	      else if( env = environment(env) ) {
		  ob = present(var, env);
		  if( ob ) return ob;
	      }
	      if( ob = find_player(var) ) return ob;
	      if( ob = find_living(var) ) return ob;
	      if( !this_player() ) {
		  return 0;
	      }
	      var = absolute_path(this_player()->query_cwd(), var);
	      if( ob = load_object(var) ) {
		  return ob;
	      }
	  }
	  return 0;
    }
    return 0;
}
