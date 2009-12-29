#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object target;
    int arch = adminp(this_player());
    int ret, curr;

    if(!sizeof(str)) str = "me";
    if(str == "me") str = this_player()->GetKeyName();
    if(!target = present(str, environment(this_player()))){
        if(arch && (target = find_player(str))){
            write("User found.");
        }
        else {
            write("They're not here.");
            return 1;
        }
    }
    if(living(target) && !arch && target != this_player()){
        write("You can only unanchor yourself with this command.");
        return 1;
    }
    if(!(curr = target->GetAnchored())){
        if(target != this_player()) str = nominative(target)+"'s";
        else str = "You're ";
        write(capitalize(str) + " already unanchored.");
        return 1;
    }
    ret = target->SetAnchored(0);
    if(ret == curr){
        write("Nothing happens");
        return 1;
    }
    if(target != this_player()){ 
        tell_object(target, capitalize(this_player()->GetName())+
                " unanchors you.");
    }
    else str = "yourself";
    write("You unanchor "+str+".");
    return 1;
}

string GetHelp() {
    return ("Syntax: unanchor <target>\n\n"
            "Removes a type of blocked movement from the target..\n"
            "See also: anchor");
}                                                    
