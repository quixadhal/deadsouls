#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
#if GRID
    if(!str){
        write("Showgrid is "+ (this_player()->GetVisibleGrid() ? "on." : "off."));
        return 1;
    }
    if(str == "on"){
        if(this_player()->GetVisibleGrid()){
            write("You are already seeing grid coordinates.");
            return 1;
        }
        this_player()->SetVisibleGrid(1);
        if(this_player()->GetVisibleGrid()){
            write("You are now seeing grid coordinates.");
        }
        else {
            write("Fail. Coordinates are not shown.");
        }
        return 1;
    }
    if(str == "off"){
        if(!(this_player()->GetVisibleGrid())){
            write("Grid coordinate display is already disabled.");
            return 1;
        }
        this_player()->SetVisibleGrid(0);
        if(!(this_player()->GetVisibleGrid())){
            write("Grid coordinate display is now disabled.");
        }
        else {
            write("Fail. Coordinates are still displaying.");
        }
        return 1;
    }
    write("Try: help showgrid");
    return 1;
#else
    write("This feature is disabled.");
    return 1;
#endif
}

string GetHelp(){
    return ("Syntax: showgrid [ on | off ]\n\n"
            "Adds global grid coordinates for you environment "
            "to the room description, if "
            "ROOMS_D enables them and they are available."); 
}
