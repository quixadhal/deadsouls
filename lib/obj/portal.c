/*  /verbs/spells/obj/portal.c
 *  created by Blitz@Dead Souls 960417
 *  used with the portal spell
 */
 
#include <lib.h>
#include "include/portal.h"
 
inherit LIB_ITEM;
 
static int LifeSpan = 0;
static string Caster = "Blitz",
              LinkRoom;
 
static void create() {
    item::create();
    SetKeyName("portal");
    SetId("portal");
    SetAdjectives("pulsating", "shimmering");
    SetShort("%^YELLOW%^a shimmering portal%^RESET%^");
    SetLong((: eventShowPortal :));
    SetPreventGet("The portal is pure magic and lacks substance.");
    SetPreventPut("The portal is pure magic and lacks substabce.");
    SetMass(0);
    SetValue(0);
}
 
string eventShowPortal() {
    string str;
    str = "A shimmering, magical portal that pulsates different "
          "shades of white and blue. It was created by Talima "
          "for her faithful servent " + Caster + ". ";
    if( sizeof(LinkRoom) ) {
        object ob = load_object(LinkRoom);
        if( !ob ) return "There are problems with this portal.";
        str += "Gazing into its fitful depths, you can barely see "
               "an occasional glimpse of " + ob->GetShort() + ".";
    }
    else str += "You see nothing in its depths.";
    return str;
}
 
int SetLink(mixed dest) {
    if( stringp(dest) ) {
        dest = load_object(dest);
        if( !dest ) return 0;
    }
    else if( !objectp(dest) ) error("Invalid argument 1 to SetLink!");
    LinkRoom = base_name(dest);
    return 1;
}
 
string GetLink() { return LinkRoom; }
 
int SetLifeSpan(int x) {
    if( LifeSpan ) return 0;
    call_out((: eventDestruct :), x);
    return (LifeSpan = x);
}
 
int GetLifeSpan() { return LifeSpan; }
 
string SetCaster(string str) { return (Caster = str); }
 
string GetCaster() { return Caster; }
 
mixed CanEnter(object who, string what) {
    if( !sizeof(LinkRoom) ) return "The portal is non-functional.";
    else return 1;
}
 
int eventEnter(object who) {
    if( !who ) return 0;
    who->eventPrint("%^RED%^%^BOLD%^You step into the portal "
                    "and feel reality shift around you.");
    who->eventMoveLiving(LinkRoom,
        "%^GREEN%^$N steps into the portal and disappears.",
        "%^GREEN%^$N steps out of a shimmering portal.");
    return 1;
}
 
int eventDestruct() {
    object env = environment();
    if( env ) {
        env->eventPrint("A shimmering portal winks out of existance.");
    }
    ::eventDestruct();
}
