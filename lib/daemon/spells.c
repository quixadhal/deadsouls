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
private mapping Prayers = ([]);

void eventReloadSpells();

static void create() {
    daemon::create();
    SetNoClean(1);
    eventReloadSpells();
}

void eventReloadSpells() {
    string array spells, prayers;

    Spells = ([]);
    spells = get_dir(DIR_SPELLS "/*.c");
    prayers = get_dir(DIR_PRAYERS "/*.c");
    foreach(string element in (prayers + spells)) {
        string spell, where;
        object ob = find_object(DIR_SPELLS "/" + element);
        if(!ob){
            ob = find_object(DIR_PRAYERS "/" + element);
            where = DIR_PRAYERS "/" + element;
        }
        else where = DIR_SPELLS "/" + element;

        if( ob ) {
            ob->eventDestruct();
        }
        if( ob = load_object(where) ) {
            spell = ob->GetSpell();
            if( spell ) {
                if(member_array(element,spells) != -1)
                    Spells[spell] = ob;
                else if(member_array(element,prayers) != -1)
                    Prayers[spell] = ob;
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

mapping GetSpells(){
    return (Spells + ([]));
}

object GetPrayer(string prayer) {
    if( !Prayers[prayer] ) {
        eventReloadSpells();
    }
    return Prayers[prayer];
}

mapping GetPrayers(){
    return (Prayers + ([]));
}

