/*    /daemon/statistics.c
 *    from the Dead Souls V Object Library
 *    Keeps track of RAM intensive data on players
 *    created by Descartes of Borg 951217
 *    Version: @(#) statistics.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <dirs.h>
#include "include/statistics.h"

inherit LIB_DAEMON;

mixed eventKill(object target) {
    mapping kills;
    object pl;
    string nom, file;

    if( !userp(pl = previous_object()) ) return 0;
    nom = (string)pl->GetKeyName();
    file = DIR_KILLS "/" + nom[0..0];
    if( file_size(file) != -2 ) {
	unguarded( (: mkdir, file :) );
    }
    file = file + "/" + nom;
    if( file_size(file) < 1 ) {
	kills = ([]);
    }
    else kills = restore_variable(unguarded((: read_file, file :)));
    nom = (string)target->GetShort() || (string)target->GetKeyName();
    if( !kills[nom] ) kills[nom] = 1;
    else kills[nom]++;
    unguarded((: rm, file :));
    unguarded((: write_file($(file), save_variable($(kills))) :));
    return 1;
}

mapping GetKills(string who) {
    string file;
    
    who = convert_name(who);
    file = DIR_KILLS "/" + who[0..0] + "/" + who;
    if( unguarded((: file_size, file :)) < 1 ) return ([]);
    else return restore_variable(unguarded((: read_file, file :)));
}
