#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;
string globalstr;

static void create() {
    verb::create();
    SetVerb("roll");
    SetRules("OBS","OBS on OBJ","OBS on STR","OBS here","STR","STR on OBJ");
    SetErrorMessage("Roll something?");
    SetHelp("Syntax: roll THING\n"
            "roll THING on THING        \n"
            "");  
}

mixed can_roll() {
    return this_player()->CanManipulate();
}

mixed can_roll_obj_here() {
    return this_player()->CanManipulate();
}

varargs mixed can_roll_obj(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return this_player()->CanManipulate();
}

varargs mixed can_roll_obj_on_obj(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return this_player()->CanManipulate();
}

varargs mixed can_roll_obj_on_str(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return this_player()->CanManipulate();
}

varargs mixed can_roll_str_on_obj(string foo, object where, string arg1, string arg2) {
    if(!where && arg2) where = get_object(arg2);
    return this_player()->CanManipulate();
}

varargs mixed can_roll_str(string foo) {
    return this_player()->CanManipulate();
}

mixed do_roll() {
    return "Roll what?";
}

varargs mixed do_roll_obj(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return arg1->eventRoll();
}

varargs mixed do_roll_obj_here(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return arg1->eventRoll();
}

varargs mixed do_roll_obj_on_obj(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return arg1->eventRoll(arg2);
}

varargs mixed do_roll_obs(mixed arg1, mixed arg2, mixed arg3, mixed arg4){
    mixed sum;
    foreach(object die in arg1){
        if(!sum) sum = die->eventRoll();
        else sum += die->eventRoll();
    }
    return arg1[0]->eventResults(sum);
}

varargs mixed do_roll_obs_here(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    return do_roll_obs(arg1, arg2);
}


varargs mixed do_roll_obs_on_obj(mixed arg1, mixed arg2, mixed arg3, mixed arg4){
    mixed sum;
    foreach(object die in arg1){
        if(!sum) sum = die->eventRoll(arg2);
        else sum += die->eventRoll(arg2);
    }
    return arg1[0]->eventResults(sum);
}

varargs mixed do_roll_obj_on_str(mixed arg1, mixed arg2, mixed arg3, mixed arg4) {
    if(arg2 == "floor" || arg2 == "ground" || arg2 == "here") arg2 = environment(this_player());
    else {
        write("That's not here.");
        return 0;
    }
    return arg1->eventRoll(arg2);
}

varargs mixed do_roll_obs_on_str(mixed arg1, mixed arg2, mixed arg3, mixed arg4){
    mixed sum;
    foreach(object die in arg1){
        if(!sum) sum = do_roll_obj_on_str(die, arg2);
        else sum += do_roll_obj_on_str(die, arg2);
    }
    return arg1[0]->eventResults(sum);
}

varargs mixed do_roll_str_on_obj(string foo, object where, string arg1, string arg2) {
    object *dice = ({});
    int d1,d2;
    mixed sum;

    globalstr = "";
    if(!stringp(foo)) return 0;
    if(sscanf(foo,"%dd%d",d1,d2) < 2){
        sscanf(foo,"d%d",d2);
    }
    if(!d2){
        write("Roll what? Are you sure you have the die?");
        return 0;
    }
    if(!d1) d1 = 1;
    globalstr = "d"+d2;
    if(arg2 && !where) where = get_object(arg2, this_player());
    dice = filter(all_inventory(this_player()), (: answers_to(globalstr, $1) :) );
    if(sizeof(dice) < d1){
        write("You seem to lack sufficient dice of that denomination.");
        return 0;
    }
    if(sizeof(dice) > d1){
        dice = dice[0..d1-1];
    }
    foreach(object die in dice){
        if(!sum) sum = die->eventRoll(where);
        else sum += die->eventRoll(where);
    }

    return dice[0]->eventResults(sum);
}

mixed do_roll_str(string foo) {
    object where;
    string s1, s2;

    if(sscanf(foo,"%s on %s",s1, s2) == 2){
        where = get_object(s2,this_player());
        foo = s1;
    }
    return do_roll_str_on_obj(foo,where);
}
