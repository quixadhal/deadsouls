/*    /lib/props/uniqueness.c
 *    From the Dead Souls V Object Library
 *    Allows certain objects to be made rareor unique
 *    Created by Descartes of Borg 961222
 *    Version: @(#) uniqueness.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <daemons.h>

private static int array Rarity = 0;
private static int       Unique = 0;

int array GetRarity() {
    return Rarity;
}

int array SetRarity(int count, int days) {
    return (Rarity = ({ count, days }));
}

int GetUnique() {
    return Unique;
}

int SetUnique(int x) {
    Unique = x;
    if( Unique ) {
	UNIQUE_D->eventTouchObject();
    }
    return Unique;
}
