/*    /realms/haderach/land/cities/hartland/pawnshop.c
 *    created by Haderach@Frontiers
 *    Date: Sat Aug 31 02:37:27 1996
 */

#include <lib.h>

inherit LIB_ROOM;

static void create(){
    ::create();
}

void init(){
    ::init();
    add_action("list","list");
    add_action("show","show");
    add_action("price","price");
    add_action("appraise","appraise");
}

object *ob;
object ob2;
int list(string str){
    if(!str || str == "") str = "all";
    present("vendor")->cmdBrowse(this_player(),str);
    return 1;
}



int show(string str){
    present("vendor")->cmdShow(this_player(),str);
    return 1;
}

int price(string str){
    present("vendor")->cmdPrice(this_player(),str);
    return 1;
}

int appraise(string str){
    present("vendor")->cmdAppraise(this_player(),str);
    return 1;
} 
