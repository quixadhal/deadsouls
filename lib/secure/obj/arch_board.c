#include <lib.h>
#include <privs.h>

inherit LIB_SECURE_BOARD;

void create(){
    ::create();

    SetKeyName("chalkboard");
    SetId(({ "board", "chalkboard" }));
    set_board_id("admin_board");
    SetShort("The Arch Board");
    SetLong("This is the Arch board. You know how to use it.");
}

void init(){
    ::init();
}

    void validate(){
        if( !this_player() || !archp(this_player()) ) 
            error("Illegal attempt to access arch board: "+get_stack()+" "+identify(previous_object(-1)));
    }

int cmd_read(string str) {
    validate();
    return ::cmd_read(str);
}

int cmd_post(string str) {
    validate();
    return ::cmd_post(str);
}

void continue_post(string subj, string file) {
    validate();
    return ::continue_post(subj, file);
}

void end_post(string subj, string mail) {
    validate();
    return ::end_post(subj, mail);
}

int cmd_followup_and_respond(string str) {
    validate();
    return ::cmd_followup_and_respond(str);
}


void continue_followup(mapping post, string subj, string file) {
    validate();
    return ::continue_followup(post, subj, file);
}

void continue_mail(mapping post, string subj, string file) {
    validate();
    return ::continue_mail(post, subj, file);
}

int cmd_remove(string str){
    validate();
    return ::cmd_remove(str);
}

int cmd_edit(string str){
    validate();
    return ::cmd_edit(str);
}

void end_edit(string subj, int num) {
    validate();
    return ::end_edit(subj, num);
}

string GetExternalDesc() {
    validate();
    return ::GetExternalDesc();
}




