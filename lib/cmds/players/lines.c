// /cmd// From Dead Souls
// A utility to help wizards and players keep track of what lines they're
// on, and to them turn all off and on with one command.
// by Gregon@Dead Souls
 
 
int cmd(string str) {
 
string *channels;
int i;
 
channels = distinct_array((string *)this_player()->GetChannels());
 
  if(!str) {
   for(i=0; i<sizeof(channels); i++) {
    if(this_player()->GetBlocked(channels[i]))
      message("info","You are blocking "+channels[i]+".",this_player());
    else message("info","You are not blocking "+channels[i]+".",this_player());
   }
   return 1;
  }
 
  if(str=="on"){
      for(i=0; i<sizeof(channels);i++){
      if(this_player()->GetBlocked(channels[i]))
        this_player()->SetBlocked(channels[i]);
     }
     return 1;
   }
 
  if(str=="off"){
      for(i=0; i<sizeof(channels);i++){
      if(!this_player()->GetBlocked(channels[i]))
        this_player()->SetBlocked(channels[i]);
     }
     return 1;
  }
 
}		
 
void help(){
   message("help","Syntax: lines -or- lines [on|off]\n\n" 
           "With no argument this command will display the status "
           "of the lines to which you have access.  With the argument on|off "
           "it will turn all of the lines on or off.",this_player());
}
