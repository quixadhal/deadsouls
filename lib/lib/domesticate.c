private string *TrainedSkills = ({});
private int Befriendable, Trainable, Commandable;
private object Owner = 0;
private string *Befriended = ({});

mixed direct_befriend_liv(){
    return 1;
}

mixed direct_abandon_liv(){
    return 1;
}

mixed direct_train_liv(){
    return this_object()->CanTrain();
}

mixed direct_train_liv_to_str(){
    return this_object()->CanTrain();
}

mixed direct_untrain_liv(){
    return this_object()->CanTrain();
}

mixed direct_untrain_liv_to_str(){
    return this_object()->CanTrain();
}

mixed direct_command_liv(){
    return this_object()->CanCommand();
}

mixed direct_command_liv_to_str(){
    return this_object()->CanCommand();
}

object GetMountOwner(){
    return Owner;
}

object SetMountOwner(object who){
    if(who) Owner = who;
    return Owner;
}

int SetCanBefriend(int i){
    if(!i) Befriendable = 0;
    else Befriendable = i;
    return Befriendable;
}

int CanBefriend(object who){
    int owner_char = -1;
    int player_char = -1;
    if(!Befriendable) return 0;
    if(Owner && objectp(Owner)) owner_char = Owner->GetStat("charisma")["level"];
    player_char = who->GetStat("charisma")["level"];
    if(player_char > Befriendable && player_char > owner_char) return 1;
    return 0;
}

int CanAbandon(object who){
    if(!Befriendable) return 0;
    if(Owner && Owner == who) return 1;
    else return 0;
}

int SetCanTrain(int i){
    if(!i) Trainable = 0;
    else Trainable = 1;
    return Trainable;
}

int CanTrain(){
    return Trainable;
}

int CanUnTrain(){
    return Trainable;
}

int SetCanCommand(int i){
    if(!i) Commandable = 0;
    else Commandable = 1;
    return Commandable;
}

int eventBefriend(object who){
    if(!CanBefriend(who)){
        write("You fail to befriend "+this_object()->GetName()+".");
        say(who->GetName()+" fails to befriend "+this_object()->GetName()+
                ". "+capitalize(nominative(who))+" looks very silly!");
        return 1;
    }
    write("You befriend "+this_object()->GetName()+".");
    say(who->GetName()+" befriends "+this_object()->GetName()+
            ". Awwww, it's such a cute sight to see!");
    Owner = who;
    Befriended += ({ who });
    return 1;
}

int eventAbandon(object who){
    if(!CanAbandon(who)){
        write("You can't abandon "+this_object()->GetName()+".");
        return 1;
    }
    write("You abandon "+this_object()->GetName()+".");
    say(who->GetName()+" abandons "+this_object()->GetName()+
            ". How sad!");
    Owner = 0;
    return 1;
}

varargs int eventTrainLiving(object who, string what){
    if(what) what = replace_string(what, "to ", "");
    if(!who) who = this_player();
    if(Owner != who){
        write("You are not this mount's owner.");
        return 0;
    }
    if(what && member_array(what, TrainedSkills) != -1){
        write(this_object()->GetName()+" already possesses that skill.");
        return 1;
    }
    if(!what || what == "") return 1;
    else {
        tell_player(who,"You train "+this_object()->GetShort()+"."); 
        tell_room(environment(who),who->GetName()+" trains "+
                this_object()->GetShort()+".", ({ who,this_object() }) );
        TrainedSkills += ({ what });
    }
    return 1;
}

varargs int eventUnTrainLiving(object who, string what){
    if(what) what = replace_string(what, "to ", "");
    if(!who) who = this_player();
    if(Owner != who){
        write("You are not this mount's owner.");
        return 0;
    }
    if(!what || what == ""){
        Owner = 0;
    }
    if(what &&  member_array(what, TrainedSkills) != -1){
        TrainedSkills -= ({ what });
    }
    tell_player(who,"You untrain "+this_object()->GetShort()+".");
    tell_room(environment(who),who->GetName()+" untrains "+
            this_object()->GetShort()+".", ({ who,this_object() }) );
    return 1;
}

string *GetTrainedSkilles(){
    return copy(TrainedSkills);
}

string *SetTrainedSkills(string *skills){
    if(skills) TrainedSkills = skills;
    return copy(TrainedSkills);
}

string eventCommandNPC(object who, string cmd){
    //if(this_object()->GetStat("intelligence") < 10
    return "foo";
}

varargs mixed GetBefriended(mixed who){
    if(!Befriended) Befriended = ({});
    if(!who) return copy(Befriended);
    if(stringp(who)) who = find_object(who);
    if(objectp(who)) who = file_name(who);
    if(member_array(who, Befriended) != -1) return 1;
    return 0;
} 

varargs mixed SetBefriended(mixed who){
    if(!Befriended) Befriended = ({});
    if(!who) return copy(Befriended);     
    if(stringp(who)) who = find_object(who);
    if(who && objectp(who)) who = file_name(who);
    if(who) Befriended += ({ who });
    return copy(Befriended);
}

