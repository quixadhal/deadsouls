mixed indirect_show_liv_obj(object target){
    if( environment() != this_player() ){
        return "#You cannot show what is not yours.";
    }
    return 1;
}

mixed direct_show_obj_to_liv(){
    if( environment() != this_player() ){
        return "#You cannot show what is not yours.";
    }
    return 1;
}

mixed direct_show_obj_liv(){
    return direct_show_obj_to_liv();
}
