/*  /cmds/creator/mraces.c
 *  A tool for cres which displays a listing of all preset npc races
 *  Blitz@NM-IVD
*/
 
#include <lib.h>
#include <preset_npc_races.h>
 
mixed cmd(string str) {
  string *types;
  if( str ) types = regexp(PresetNpcTypes, "^"+str);
  else types = PresetNpcTypes;
  if( !sizeof(types) ) {
    if( !str ) return "No list at this time.";
    return "No match was found for \""+str+"\".";
  }
  types = sort_array(types, 1);
  message("help", format_page(types, 5), this_player() );
  return 1;
}
 
void help() {
  message("info",
    "Syntax: mraces\n"
    "        mraces <search prefix>\n\n"
    "A simple tool for creators which will list available 'preset' npc "
    "races.  Entering \"mraces\" along will list all races.  Entering "
    "\"mraces a\" will list all races that begin with 'a', etc.",
   this_player() );
}
 
