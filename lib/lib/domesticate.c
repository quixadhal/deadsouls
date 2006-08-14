private string *TrainedSkills = ({});
private int Befriendable, Trainable, Commandable;
private object Owner;

mixed direct_befriend_liv(){
    return this_object()->CanBefriend();
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

int SetCanBefriend(int i){
    if(!i) Befriendable = 0;
    else Befriendable = 1;
    return Befriendable;
}

int CanBefriend(){
    return Befriendable;
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

varargs int eventTrainNPC(object who, string what){
    if(what) what = replace_string(what, "to ", "");
    if(!who) who = this_player();
    if(!Owner) Owner = who;
    if(Owner != who) {
	write("You are not this mount's owner.");
	return 0;
    }
    if(!what || what == "" || member_array(what, TrainedSkills) != -1) return 1;
    else {
	tell_player(who,"You train "+this_object()->GetShort()+"."); 
	tell_room(environment(who),who->GetName()+" trains "+
	  this_object()->GetShort()+".", ({ who,this_object() }) );
	TrainedSkills += ({ what });
    }
    return 1;
}

varargs int eventUnTrainNPC(object who, string what){
    if(what) what = replace_string(what, "to ", "");
    if(!who) who = this_player();
    if(Owner != who) {
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
