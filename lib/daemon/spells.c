/*    /daemon/spells.c
 *    From Dead Souls LPMud
 *    created by Descartes of Borg 961027
 *    Version: @(#) spells.c 1.2@(#)
 *    Last modified: 96/10/28
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

private mapping Spells = ([]);

void eventReloadSpells();

static void create() {
    daemon::create();
    SetNoClean(1);
    eventReloadSpells();
}

void eventReloadSpells() {
    string array spells;

    Spells = ([]);
    spells = get_dir(DIR_SPELLS "/*.c");
    foreach(string spell in spells) {
	object ob = find_object(DIR_SPELLS "/" + spell);
	
	if( ob ) {
	    ob->eventDestruct();
	}
	if( ob = load_object(DIR_SPELLS "/" + spell) ) {
	    spell = ob->GetSpell();
	    if( spell ) {
		Spells[spell] = ob;
	    }
	}
    }
}

object GetSpell(string spell) {
    if( !Spells[spell] ) {
	eventReloadSpells();
    }
    return Spells[spell];
}
