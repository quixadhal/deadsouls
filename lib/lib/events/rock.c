varargs int CanRock(mixed who, mixed what){
    return this_player()->CanManipulate();
}

mixed direct_rock_obj(object target){
    return CanRock(this_player(), target);
}

mixed direct_rock_wrd_obj(object target){
    return CanRock(this_player(), target);
}

mixed direct_rock_str(string str){
    return CanRock(this_player(), remove_article(lower_case(str)));
}

mixed direct_rock_str_on_obj(string str, object target){
    return CanRock(this_player(), remove_article(lower_case(str)));
}

varargs mixed eventRock(object who, mixed what){
    return 1;
}
