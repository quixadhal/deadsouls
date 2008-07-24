varargs int CanSwivel(mixed who, mixed what){
    return this_player()->CanManipulate();
}

mixed direct_swivel_obj(object target){
    return CanSwivel(this_player(), target);
}

mixed direct_swivel_wrd_obj(object target){
    return CanSwivel(this_player(), target);
}

mixed direct_swivel_str(string str){
    return CanSwivel(this_player(), remove_article(lower_case(str)));
}

mixed direct_swivel_str_on_obj(string str, object target){
    return CanSwivel(this_player(), remove_article(lower_case(str)));
}

varargs mixed eventSwivel(object who, mixed what){
    return 1;
}
