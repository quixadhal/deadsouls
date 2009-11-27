#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object env, my_env, player;
    string coords, my_coords, ret, subret;
    int x, y, z;
    int x2, y2, z2;
    int x3, y3, z3;
    if(!sizeof(str)) str = "me";
    if(str == "me" || str == "myself") str = this_player()->GetKeyName();
    player = find_player(str);
    if(!player){
        write("That person could not be found.");
        return 1;
    }
    env = environment(player);
    if(env && living(env)) env = environment(env);
    if(!env){
        write("That player appears to lack an environment.");
        return 1;
    }
    my_env = environment(this_player());
    if(my_env && living(my_env)) my_env = environment(my_env);
    if(env == my_env){
        write(capitalize(nominative(player))+" is right here!");
        return 1;
    }
    coords = ROOMS_D->GetCoordinates(env);
    ret = capitalize(player->GetKeyName())+" is in "+
        env->GetShort()+", base name "+base_name(env);
    if(sizeof(coords) > 2){
        ret += " , coordinates: "+coords+" .";
    }
    else {
        ret += " .";
    }
    if(my_env && sizeof(my_coords = ROOMS_D->GetCoordinates(my_env))){
        if(sscanf(coords,"%d,%d,%d",x,y,z) == 3 &&
                sscanf(my_coords,"%d,%d,%d",x2,y2,z2) == 3){
            int dist;
            float deg;
            subret = "";
            deg = bearing(x2,y2,x,y);
            x3 = abs(x - x2);
            y3 = abs(y - y2);
            z3 = abs(z - z2);
            if(x3 > y3) dist = x3;
            else dist = y3;
            if(z3 > dist) dist = z3;
            if(y2 < y){
                subret += "north";
            }
            else if(y2 > y){
                subret += "south";
            }
            if(x2 < x){
                subret += "east";
            }
            else if(x2 > x){
                subret += "west";
            }
            if(sizeof(subret)){
                subret = "\n" + capitalize(nominative(player))+ " is "+
                    cardinal(dist) + " rooms away, " + subret + " of you";
                subret += " ("+deg+" degrees)";
                if(z2 < z) subret += " and above.";
                else if(z2 > z) subret += " and below.";
                else subret += ".";
            }
            else {
                subret = "\n" + capitalize(nominative(player))+ " is "+
                    cardinal(dist) + " rooms ";
                if(z2 < z) subret += "above.";
                else if(z2 > z) subret += "below.";
            }
        }
    }
    if(subret) ret += subret;
    write(ret);
    return 1;
}

string GetHelp() {
    return ("Syntax: whereis <player>\n\n"
            "Indicates the filename and coordinates (if available) "+
            "of the named player.");
}                                                    
