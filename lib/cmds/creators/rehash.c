/*   /cmds/creator/rehash.c
 *   This allows creators to refresh command lookup tables
 *   Blitz@NM-IVD
*/
 
#include <lib.h>
#include <daemons.h>
 
inherit LIB_DAEMON;
 
mixed cmd(string str) {
  if( !sizeof(str) ) return "Syntax: rehash <path>\n";
  if( file_size(str) != -2 )
    return str+": Path not found.";
  CMD_D->eventRehash(str);
  message("system", str+": Rehashed.", this_player() );
  return 1;
}
 
void help() {
  message("help",
    "Syntax: rehash <path>\n\n"
    "This command allows creators to refresh the specified path's "
    "command parsing lookup tables.",
   this_player() );
}
 
