mixed CanApply(object applicant){
    return 1;
}

mixed direct_apply_obj_word_obj(){
    if( environment() != this_player() ){
        return "#You don't have that!";
    }
    return CanApply(this_player());
}

mixed indirect_apply_obj_word_obj(){
    object room = environment(this_player());
    if(!room || environment() != this_player() || environment() != room){
        return "#That's not available to you!";
    }
    return CanApply(this_player());
}

