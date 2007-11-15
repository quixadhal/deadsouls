#include <lib.h>

int direct_learn_str_from_liv(){ return 1;}
int direct_learn_to_str_from_liv(){ return 1;}

int CanLearn(){
    return 1;
}

int eventLearn(object who, string what){
    if(!CanLearn()){
        write("You can't learn that.");
        return 0;
    }
    who->eventTeach(this_player(),what);
    return 1;
}
