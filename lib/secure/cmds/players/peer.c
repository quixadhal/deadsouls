/*   /secure/cmds/player/peer.c
 *   Peer into an adjacent room wihout actually entering
 *   Blitz@Dead SoulsIV
 */
 
#include <lib.h>
 
inherit LIB_DAEMON;
 
string DescribeItems(mixed var);
string DescribeLiving(mixed var);
 
mixed cmd(string str) {
  int i;
  string file;
  object env, *livings, *items;
 
  if( !sizeof(str) ) return "Syntax: peer <direction>";
  env = environment(this_player());
  if( !file = (string)env->GetExit(str) ) file = (string)env->GetEnter(str);
  if( !sizeof(file) )
    return "You cannot peer that way.";
  if( i = effective_light(this_player()) > 6 )
    return "It is too bright to do that.";
  else if( i < 0 )
    return "It is too dark to attempt that.";
  if( env->GetDoor(str) && ((string)env->GetDoor(str))->GetClosed() ) {
    message("my_action", sprintf("%s is blocking your view %s.",
      ((string)env->GetDoor(str))->GetShort(str), str),
    this_player() );
    return 1;
  }
  if( !unguarded((: file_exists, file + ".c" :)) ||
      (!env = load_object(file)) ) {
    message("my_action", "It is not safe to peer "+str+"!", this_player() );
    return 1;
  }
  if( (int)env->GetProperty("light") < 0 ||
      (int)env->GetProperty("light") > 6 ) {
    message("my_action", "You cannot see "+str+" very well.", this_player());
    return 1;
  }
  items = filter(all_inventory(env),
           (: !(int)$1->GetInvis(this_player()) :) );
  items = items - (livings = filter(items, (: living :)));
  message("my_action", "%^GREEN%^"
    "Peering "+str+" you see...",
  this_player() );
  message("other_action",
    (string)this_player()->GetCapName()+" peers "+str+".",
  environment(this_player()), this_player() );
  message("room_description",
    ("\n"+(string)env->GetLong(0)+"\n" || "\nA void.\n"),
  this_player() );
  if( sizeof(items) )
    message("room_inventory",
      "%^MAGENTA%^" + DescribeItems(items) + "%^RESET%^\n",
    this_player() );
  if( sizeof(livings) )
    message("room_inventory",
      "%^BOLD%^%^RED%^" + DescribeLiving(livings) + "%^RESET%^",
      this_player() );
  return 1;
}
 
string DescribeItems(mixed var) {
  mapping m = ([ ]);
  string *shorts, ret;
  int i, max;
 
  if( !arrayp(var) ) return "";
  i = sizeof( shorts = map(var, (: $1->GetShort() :)) );
  while(i--) {
    if( !sizeof(shorts[i]) ) continue;
    if( m[ shorts[i] ] ) m[ shorts[i] ]++;
    else m[ shorts[i] ] = 1;
  }
  i = max = sizeof( shorts = keys(m) );
  ret = "";
  for(i=0; i<max; i++) {
    if( m[ shorts[i] ] < 2 ) ret += shorts[i];
    else ret += consolidate(m[shorts[i]], shorts[i]);
    if( i == (max - 1) ) {
      if( max>1 || m[ shorts[i] ] > 1 ) ret += " are here.";
      else ret += " is here.";
    }
    else if( i == (max - 2) ) ret += ", and ";
    else ret += ", ";
  }
  return capitalize(ret);
}
 
string DescribeLiving(mixed var) {
  mapping m = ([ ]);
  string *shorts, ret;
  int i;
  if( !arrayp(var) ) return "";
  i = sizeof( shorts = map(var, (: $1->GetShort() :)) );
  while(i--) {
    if( !sizeof(shorts[i]) ) continue;
    if( m[ shorts[i] ] ) m[ shorts[i] ]++;
    else m[ shorts[i] ] = 1;
  }
  ret = "";
  i = sizeof( shorts = keys(m) );
  while(i--) if( m[ shorts[i] ] > 1 )
    ret += (consolidate(m[shorts[i]], shorts[i]) + "\n");
    else ret += (shorts[i] + "\n");
  return ret;
}
 
void help() {
  message("help",
    "Syntax: peer <direction>\n\n"
    "Allows you to look into an adjacent room without actually "
    "entering it.  Note that light and doorways affect what you "
    "see.\n\n",
  this_player() );
}
