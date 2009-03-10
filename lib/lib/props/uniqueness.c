/*    /lib/props/uniqueness.c
 *    From the Dead Souls Mud Library
 *    Allows certain objects to be made rareor unique
 *    Created by Descartes of Borg 961222
 *    Version: @(#) uniqueness.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <daemons.h>

private static int array Rarity = 0;
private static int       Unique = 0;
private static int       MaxClones = 0;

int array GetRarity(){
    return Rarity;
}

int array SetRarity(int count, int days){
    return (Rarity = ({ count, days }));
}

int GetUnique(){
    return Unique;
}

int SetUnique(int x){
    Unique = x;
    if( Unique ){
        UNIQUE_D->eventTouchObject();
        this_object()->SetRetain(0);
    }
    return Unique;
}

int GetMaxClones(){
    return MaxClones;
}

int SetMaxClones(int x){
    if(x > -1) MaxClones = x;
    else MaxClones = 0;
    if( MaxClones == 1 ){
        this_object()->SetUnique(1);
    }
    if(MaxClones) this_object()->SetRetain(0);
    return MaxClones;
}

