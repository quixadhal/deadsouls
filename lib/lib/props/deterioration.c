/*    /lib/props/deterioriation.c
 *    From the Dead Souls V Object Library
 *    Handles the deterioration of objects
 *    Created by Descartes of Borg 970101
 *    Version: @(#) deterioration.c 1.1@(#)
 *    Last modified: 97/01/01
 */

private int Broken          = 0;
private int DamagePoints    = 0;
private int Deterioration   = 0;
private int MaxDamagePoints = 20000;

// abstract methods
void eventDeteriorate(int type);
// end abstract methods

int GetBroken() {
    return Broken;
}
 
int SetBroken(int x) {
    return (Broken = x);
}
 
int SetDamagePoints(int x) {
    return (MaxDamagePoints = DamagePoints = x);
}
 
int GetDamagePoints() {
    return DamagePoints;
}
 
int GetDeterioration() {
    return Deterioration;
}

string GetItemCondition() {
    if( Deterioration ) {
	return "It has worn down completely.";
    }
    else {
	return 0;
    }
}

string array GetSave() {
    return ({ "Broken", "DamagePoints", "Deterioration" });
}

// args d and l not used
int eventReceiveDamage(object agent, int type, int amt, int d, mixed array l) {
    DamagePoints -= (amt * 5);
    if( DamagePoints < 1 ) {
        Deterioration++;
        DamagePoints = MaxDamagePoints;
        eventDeteriorate(type);
    }
    return amt;
}
