#include <daemons.h>

string user_path(string name){
    return PLAYERS_D->GetUserPath(name);
}
