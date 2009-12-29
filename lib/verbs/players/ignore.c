#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("ignore");
    SetRules("LVS","");
    SetErrorMessage("Whom are you trying to ignore?");
    SetHelp("Syntax: ignore <LIVING | all>\n\n"
            "Allows one to stop attacking a living being "
            "or beings. Helpful for not wasting time on the "
            "wrong targets.\n"
            "See also: attack\n");
}

mixed can_ignore(){ return 1; }

mixed can_ignore_liv() { return 1; }

mixed do_ignore(){
    if(sizeof(this_player()->GetNonTargets())){
        string tmp;
        tmp = item_list(this_player()->GetNonTargets());
        this_player()->eventPrint("You are ignoring " + tmp + ".");
        return 1;
    }
    else {
        this_player()->eventPrint("Ignore whom?");
        return 0;
    }
}

mixed do_ignore_liv(object ob) {
    this_player()->AddNonTargets(ob);
    if(member_array(ob,this_player()->GetNonTargets()) != -1){

        this_player()->eventPrint("You are now ignoring " + ob->GetName() + ".");
        ob->eventPrint(this_player()->GetName() + " ignores you!");
        environment(this_player())->eventPrint(this_player()->GetName() +
                " ignores " + ob->GetName() + "!",
                ({ this_player(), ob }));
    }
    else this_player()->eventPrint("You are unable to ignore " + ob->GetName() + ".");
    return 1;
}

mixed do_ignore_lvs(mixed *targets) {
    object *obs;
    string tmp;

    obs = filter(targets, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(targets, (: $1 :));
        foreach(string *lines in ua) this_player()->eventPrint(lines[0]);
        return 1;
    }
    if(member_array(this_player(),obs) != -1) obs -= ({ this_player() });
    this_player()->AddNonTargets(obs);
    tmp = item_list(obs);
    obs->eventPrint(this_player()->GetName() + " ignores you!");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " ignores" + tmp + "!",
            ({ this_player(), obs... }));
    this_player()->eventPrint("You ignore " + tmp + ".");
    return 1;
}
