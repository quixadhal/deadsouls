/*    /lib/fish.c
 *    from the Dead Soulsr2 Object Library
 *    fish objects which get caught
 *    created by Descartes of Borg 951009
 */


#include <lib.h>
#include "include/fish.h"

inherit LIB_OBJECT;

static private int Mass, Fight;
static private string Food;

static void create() {
    object::create();
    Mass = 0;
    Food = 0;
    Fight = 0;
}

int eventCatch(object who) { return 1; }

int SetFight(int x) { return (Fight = x); }

int GetFight() { return Fight; }

string SetFood(string str) { return (Food = str); }

string GetFood() { return Food; }

int SetMass(int x) { return (Mass = x); }

int GetMass() { return Mass; }
