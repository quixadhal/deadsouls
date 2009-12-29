/*    /verbs/items/get.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 950113
 *    Version: @(#) get.c 1.5@(#)
 *    Last Modified: 96/11/12
 */

#include <lib.h>

inherit LIB_VERB;
int yes;

static void create() {
    verb::create();
    SetVerb("get");
    SetRules("OBS OBJ", "WRD from OBJ", "WRD out of OBJ", "WRD WRD from OBJ", "WRD WRD out of OBJ",
            "OBS", "OBS out of OBJ", "OBS from OBJ");
    SetSynonyms("take");
    SetErrorMessage("Get what?  Or perhaps get something from somewhere?");
    SetHelp("Syntax: <get ITEM>\n"
            "        <get ITEM from OBJECT>\n"
            "        <get ITEM out of OBJECT>\n"
            "        <get AMOUNT CURRENCY from pile>\n"
            "        <get AMOUNT CURRENCY out of pile>\n\n"
            "This allows you to get items in the same room as you, or "
            "contained inside other items so that you are carrying them.  "
            "In addition, you can specify partial amounts of currency to "
            "pick up from a pile lying around.\n\n"
            "Synonyms: take\n\n"
            "See also: drop, give, put");
}

mixed eventCheckLight(object who) {
    int light;

    if( (light = who->GetEffectiveVision()) < 2 ) {
        if( 100 + (10*light) < random(100) )
            return "You fumble around in the darkness.";
        else return this_player()->CanManipulate();
    }
    else if( light > 5 ) {
        if( 100 - (10*light) < random(100) )
            return "You fumble around in the blinding light.";
        else return this_player()->CanManipulate();
    }
    else return this_player()->CanManipulate();
}

mixed can_get_obj(string verb) {
    return eventCheckLight(this_player());
}

varargs mixed can_get_obj_out_of_obj(mixed args...) {
    mixed ret = eventCheckLight(this_player());
    object ob;
    int which;
    if(!(args[3] && ob = to_object(args[3])))
        return ret;
    else {
        if(ob->GetClosed()){
            return "The "+remove_article(ob->GetShort())+" is closed.";
        }
    }
    return ret;
}

mixed can_get_obj_obj(string verb, string rule, mixed item, object container) {
    return can_get_obj_out_of_obj(verb, rule, item, container);
}

mixed can_get_obs_obj(string verb,string rule,mixed *item,object container){
    return can_get_obj_out_of_obj(verb, rule, item, container);
}

mixed can_get_obj_from_obj(mixed args...) {
    return can_get_obj_out_of_obj(args...);
}

mixed can_get_wrd_wrd_out_of_obj(mixed args...) {
    mixed ret = eventCheckLight(this_player());
    object ob;
    if(sizeof(args) > 5)
        if(args[5]) ob = to_object(args[5]);
        else if(args[3]) ob = to_object(args[3]);

    if(ob && ob->GetClosed()){
        return "The "+remove_article(ob->GetShort())+" is closed." ;
    }
    return ret;
}

mixed can_get_wrd_wrd_from_obj(mixed args...) {
    return can_get_wrd_wrd_out_of_obj(args...);
}

mixed can_get_wrd_out_of_obj(mixed args...) {
    mixed ret = eventCheckLight(this_player());
    object ob;
    if(args[3]) ob = to_object(args[3]);

    if(ob && ob->GetClosed()){
        return "The "+remove_article(ob->GetShort())+" is closed." ;
    }
    return ret;
}

mixed can_get_wrd_from_obj(mixed args...) {
    return can_get_wrd_out_of_obj(args...);
}

mixed do_get_obj(object ob) {
    return ob->eventGet(this_player());
}

mixed do_get_obj_out_of_obj(object ob, object storage) {
    if(!ob) return "No object";
    if(!(environment(ob) == storage)){
        ob = present(ob->GetKeyName(), storage);
        if(!ob){
            write("That's not in there.");
            return "";
        }
    }
    return storage->eventGetFrom(this_player(), ({ ob }));
}

mixed do_get_obj_from_obj(object ob, object storage) {
    return do_get_obj_out_of_obj(ob, storage);
}

mixed do_get_obj_obj(object ob, object storage) {
    return do_get_obj_out_of_obj(ob, storage);
}

mixed do_get_obs(mixed *targs) {
    object *obs;
    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be taken.");
        return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
        mapping messages = unique_mapping(targs, (: $1 :));

        foreach(string msg in keys(messages)) {
            this_player()->eventPrint(msg);
        }
        return 1;
    }
    foreach(object item in obs) {

        if( item->CanGet(this_player()) != 1 ) continue;
        item->eventGet(this_player());
    }
    return 1;
}

mixed do_get_obs_out_of_obj(mixed *targs, object storage) {
    object *obs;
    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be taken.");
        return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
        mapping messages = unique_mapping(targs, (: $1 :));

        foreach(string msg in keys(messages)) {
            this_player()->eventPrint(msg);
        }
        return 1;
    }
    return storage->eventGetFrom(this_player(), obs);
}

mixed do_get_obs_from_obj(mixed *obs, object storage) {
    return do_get_obs_out_of_obj(obs, storage);
}

mixed do_get_obs_obj(mixed *obs, object storage) {
    return do_get_obs_out_of_obj(obs, storage);
}

mixed do_get_wrd_wrd_from_obj(mixed args...) {
    string num, curr;
    mixed pile;
    object ob1, ob2;

    num = args[0];
    curr = args[1];
    pile = args[2];

    ob1 = to_object(num+" "+curr);
    ob2 = to_object(implode(args[6..]," "));

    if(ob1 && ob2) return do_get_obj_from_obj(ob1, ob2);

    return pile->eventGetMoney(this_player(), to_int(num), curr);
}

mixed do_get_wrd_wrd_out_of_obj(mixed args...) {
    return do_get_wrd_wrd_from_obj(args);
}

mixed do_get_wrd_from_obj(mixed args...) {
    string num, curr;
    mixed pile;
    object ob1, ob2;

    num = args[0];
    curr = args[1];
    pile = args[2];

    ob1 = to_object(num+" "+curr);
    ob2 = to_object(implode(args[6..]," "));

    if(ob1 && ob2) return do_get_obj_from_obj(ob1, ob2);

    return pile->eventGetMoney(this_player(), to_int(num), curr);
}

mixed do_get_wrd_out_of_obj(mixed args...) {
    return do_get_wrd_from_obj(args);
}
