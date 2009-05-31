#include <lib.h>
string *query_custom_path(){
    string custom_path;
    if(!creatorp(this_player())) return ({  DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS }) ;
    custom_path = "/realms/"+this_player()->GetKeyName()+"/cmds";
    return ({ custom_path });
}

string query_custom_command(string str){
    string custom_path;
    if(!str) str = "";
    custom_path = "/realms/"+this_player()->GetKeyName()+"/cmds";
    if( file_size(custom_path) != -2 ) return "";
    if( file_size(custom_path+"/"+str+".c") < 1 ) return "";
    return custom_path+"/"+str+".c";
}
