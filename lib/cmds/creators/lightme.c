#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int level;
    if(!str){
        write("Your current radiant light is: "+this_player()->GetRadiantLight()+".");
        return 1;
    }
    if(sscanf(str,"%d",level) != 1){
        write("Please specify a numerical light level.");
        return 1;
    }
    if(level < 0 || level > 100){
        write("Please enter a light level between 0 and 100");
        return 1;
    }
    this_player()->SetRadiantLight(level);
    write("Your current radiant light is: "+this_player()->GetRadiantLight()+".");
    return 1;
}

string GetHelp(){
    return "Syntax: lightme [NUMBER]\n\n"+
        "Reports or changes the amount of light your body radiates.";
}
