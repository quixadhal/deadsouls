/*    /lib/props/damage.c
 *    From the Dead Souls V Object Library
 *    Things that do damage to other things
 *    Created by Descartes of Borg 970101
 *    Version: @(#) damage.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <damage_types.h>

private int Class      = 1;
private int DamageType = BLUNT;
private int MaxClass   = 1;

// abstract methods
mixed GetProperty(string key);
// end abstract methods

int GetClass() {
    int blessing = GetProperty("blessed");

    if( !intp(blessing) ) {
	blessing = 0;
    }
    return (Class + blessing);
}

int SetClass(int x) {
    if( x > MaxClass ) {
	MaxClass = x;
    }
    return (Class = x);
}

int GetDamageType() {
    return DamageType;
}

int SetDamageType(int x ) {
    return (DamageType = x);
}

int GetMaxClass() {
    return MaxClass;
}

string array GetSave() {
    return ({ "Class" });
}

int eventStrike(object ob) {
    return GetClass();
}
 
