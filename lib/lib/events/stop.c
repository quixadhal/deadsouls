varargs mixed CanStop(object who, string what, mixed args){
    if(who != this_object()) return 0;
    if(what == "fishing"){
        int stop_err;
        object ob = environment(who);
        stop_err = ob->CanStop(this_player(), "fishing");
        return (stop_err || "You aren't fishing!");
    }
    if(what == "fighting"){
        mixed *enemies = this_object()->GetEnemies();
        //mixed *hostiles = this_object()->GetHostiles();
        mixed *hostiles = ({});
        if(!sizeof(enemies) && !sizeof(hostiles)){
            return "You're not mad at anybody!";
        }
        return 1;
    }
    return 1;
}

varargs mixed eventStop(object who, string what, mixed args){
    if(who != this_object()) return 0;
    if(what == "fishing"){
        object ob = environment(who);
        return ob->eventStop(this_player(), "fishing");
    }
    if(what == "fighting"){
        mixed *enemies = this_object()->GetEnemies();
        mixed *hostiles = this_object()->GetHostiles();
        foreach(object enemy in enemies){
            if(!enemy) continue;
            write("You are no longer fighting "+enemy->GetShort()+".");
            this_object()->RemoveEnemy(enemy);
        }
        //foreach(object hostile in hostiles){
        //if(!hostile) continue;
        // if(member_array(hostile, enemies) == -1)
        //  write("You are no longer hostile toward "+hostile->GetShort()+".");
        //   this_object()->RemoveEnemy(hostile);
        //}
        this_object()->AddNonTargets(enemies);
        //this_object()->AddNonTargets(hostiles);
        return "You have stopped being aggressive.";
    }
    return 0;
}

