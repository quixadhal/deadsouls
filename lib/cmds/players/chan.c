mixed cmd(string args) {
    object ob = this_player();
    int i, cre = creatorp(ob);
    string *channels;
    string command, channel;
    if(!args) return "Try: help chan";
    i = sscanf(args, "%s %s", command, channel);
    if(!channel) return "Try: help chan";
    channels = distinct_array(ob->GetChannels());
    if(command == "block"){
        if(member_array(channel, channels) == -1){
            write("You are not subscribed to that channel.");
            return 1;
        }
        if(ob->GetBlocked(channel)){
            write(channel + " is already blocked.");
        }
        else ob->SetBlocked(channel, 1);
        return 1;
    } 
    if(command == "unblock"){
        if(member_array(channel, channels) == -1){
            write("You are not subscribed to that channel.");
            return 1;
        }
        if(!ob->GetBlocked(channel)){
            write(channel + " is already unblocked.");
        }
        else ob->SetBlocked(channel, 0);
        return 1;
    }
    if(!cre){
        write("Try: help chan");
        return 1;
    }
    if(command == "add"){
        if(member_array(channel, channels) != -1){
            write("You are already subscribed to that channel.");
            return 1;
        }
        ob->AddChannel(channel);
        channels = distinct_array(ob->GetChannels());
        if(member_array(channel, channels) == -1){
            write("You fail to add that channel to yourself.");
        }
        else {
            write("You have added "+channel+" to yourself.");
        }
        return 1;
    }
    if(command == "remove"){
        if(member_array(channel, channels) == -1){
            write("You are already unsubscribed to that channel.");
            return 1;
        }
        ob->RemoveChannel(channel);
        channels = distinct_array(ob->GetChannels());
        if(member_array(channel, channels) != -1){
            write("You fail to remove that channel from yourself.");
        }
        else {
            write("You have removed "+channel+" from yourself.");
        }
        return 1;
    }
    write("Try: help chan");
    return 1;
}

string GetHelp(string str) {
    int cre = creatorp(this_player());
    string ret = "Syntax: chan block <channel>\n";
    ret += "        chan unblock <channel>\n";
    if(cre){
        ret += "        chan add <channel>\n";
        ret += "        chan remove <channel>\n";
    }
    ret += "\n";
    ret += "Displays or modifies the user's channel policy "+
        "on available channels.";
    ret += "\nSee also: lines, gag, earmuff, env, hist, channels";
    return ret;
}
