#include <lib.h>

inherit LIB_ITEM;
inherit LIB_CLOSE;
inherit LIB_LIGHT;
inherit LIB_CAPTURE;

int BugDies = 0;

static void create() {
    AddSave(({ "BugDies" }));
    item::create();
    close::create();
    capture::create();
    light::create();
    SetKeyName("jar");
    SetId( ({ "jar" }) );
    SetShort("a jar");
    SetLong("A jar with lid.  The lid has holes poked in "
            "the top.  It would be perfect to catch bugs in.");
    SetDestroyOnSell(1);
    SetMass(100);
    SetValue(30);
    SetClosed(1);
    SetMaxCapture(1);
    if( BugDies > 1 ) {
        set_heart_beat(2);
    }
}

static void heart_beat() {
    BugDies -= 2;
    if( BugDies < 1 ) {
        eventDarken();
    }
}

int direct_light_obj() { // They cannot use the light command
    return 0;
}

mixed CanCapture(object who, object target) {
    if( !target->id("lightning bug") ) {
        return "The jar has a fit as you try to capture something other "
            "than a lightning bug!";
    }
    if( GetLit() ) {
        return "You already have a lightning bug in there!";
    }
    if( GetClosed() ) {
        return "The jar must be open!";
    }
    return 1;
}

mixed eventCapture(object who, object target) {
    mixed tmp = capture::eventCapture(who, "lightning bug");

    if( tmp != 1 ) {
        return tmp;
    }
    target->eventDestruct();
    who->eventPrint("You capture a lightning bug in your jar!");
    environment(who)->eventPrint(who->GetName() + " captures a lightning bug "
            "with " + possessive(who) + " jar.", who);
    SetLit(1);
    BugDies = 360;
    set_heart_beat(2);
}

mixed eventDarken() {
    object env = environment();

    if( env ) {
        env->eventPrint("The %^YELLOW%^lighning bug%^RESET%^ dies.");
    }
    if( living(env) && (env = environment(env)) ) {
        env->eventPrint(possessive_noun(environment()->GetName()) +
                " lightning bug dies and " + possessive(environment()) +
                " jar goes dark.", environment());
    }
    capture::eventFree("lightning bug");
    SetLit(0);
    return 1;
}

mixed eventFree(object who, string target) {
    mixed tmp = capture::eventFree(target);
    object bug;

    if( tmp != 1 ) {
        return tmp;
    }
    if( GetClosed() ) {
        eventOpen(who, "jar");
    }
    SetLit(0);
    environment(who)->eventPrint("A lightning bug flies free of the jar!");
    bug = new("/realms/ariel/wonder/monsters/lbug");
    bug->eventMove(environment(who));
    return 1;
}

varargs mixed eventOpen(object who, string id, object tool) {
    mixed tmp = close::eventOpen(who, id, tool);

    if( tmp != 1 ) {
        return tmp;
    }
    if( sizeof(GetCaptured()) ) {
        eventFree(who, "lightning bug");
    }
    return 1;
}

string GetShort() {
    string tmp = item::GetShort();

    if( GetLit() ) {
        tmp += "\nIt is holding a lightning bug.";
    }
    if( GetClosed() ) {
        tmp += "\nIt is closed.";
    }
    else {
        tmp += "\nIt is open.";
    }
    return tmp;
}

string GetLong(string str) {
    string tmp = item::GetLong();

    if( GetLit() ) {
        tmp += "\nIt is glowing.";
    }
    if( GetClosed() ) {
        tmp += "\nIt is closed.";
    }
    else {
        tmp += "\nIt is open.";
    }
    return tmp;

}
