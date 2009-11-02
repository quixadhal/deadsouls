#include <lib.h>
#include <rounds.h>

inherit LIB_VERB;

varargs void eventSmoke(object who, object what, string id);

static void create() {
    verb::create();
    SetVerb("smoke");
    SetRules("OBJ", "from OBJ");
    SetErrorMessage("Smoke what?");
    SetHelp("Syntax: <smoke THING>\n"
            "        <smoke from THING>\n\n"
            "Allows you to smoke a smokable item.\n"
            "See also: pack");
    SetSynonyms("puff");
}

mixed can_smoke_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_smoke_from_obj(string verb) { return this_player()->CanManipulate(); }

    mixed do_smoke_obj(object ob) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventSmoke, this_player(), ob :),
                    ROUND_OTHER);
        else eventSmoke(this_player(), ob);
        return 1;
    }

    mixed do_smoke_from_obj(object ob, string id) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventSmoke, this_player(), ob, id :),
                    ROUND_OTHER);
        else eventSmoke(this_player(), ob, id);
        return 1;
    }

varargs void eventSmoke(object who, object what, string id) {
    return what->eventSmoke(who, what);
}
