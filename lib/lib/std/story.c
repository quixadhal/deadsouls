/*    /lib/std/story.c
 *    from the Dead Souls V Object Library
 *    handles objects and NPCs telling stories
 *    created by Corvar
 *    Version: 
 *    Last Modified: 
 */
 
private int AlreadyTellingTale;
private mapping Tales;
private string TalesDir;
private string NoTaleMessage;

void PickTale( string taletotell );
void TellTale( int part );
string SetTalesDir(string dir);
string GetTalesDir();
mapping SetTale(mapping tale);
mapping GetTale();
array GetTaleKeys();
string SetNoTaleMessage(string notalemessage);
string GetNoTaleMessage();
int GetTellingTale();

static void create() {
  AlreadyTellingTale = 0;
  TalesDir = "";
  Tales = ([ ]);
}

/* One of the following two must be used.  SetTalesDir(string dir) will set a
   directory which will contain tale files.  Each tale file should consist of
   the following:
   action#delay      where action is what you want the NPC to say, and delay
                     is how long before the next action.
   Example:
say this is the first part of the tale#5
emote this is the second part of the tale which is really long so that it will span a line.  Note that there is no line feed in the middle of this line#4
speak last line#1
   End Example.

   If using SetTale(mapping tale) instead the mapping should be like the
   following
   SetTale( ([ 1 : ({"say this is the first part of the tale",5}),
      2 : ({"emote this is.....",4}), 3 : ({"speak last line",1}) ]) );
*/

string SetTalesDir(string dir) {
  TalesDir = dir;
  return TalesDir;
}

mapping SetTale(mapping tale) {
  Tales = tale;
  return Tales;
}

string SetNoTaleMessage(string notalemessage) {
  NoTaleMessage = notalemessage;
  return NoTaleMessage;
}

string GetTalesDir() { return TalesDir; }
mapping GetTale() { return Tales; }
array GetTaleKeys() { return keys( Tales ); }
string GetNoTaleMessage() { return NoTaleMessage; }
int GetTellingTale() { return AlreadyTellingTale; }

void PickTale( string taletotell ) {
    array msg;
    string file, *files;
    int y;

    if( ! AlreadyTellingTale ) {
      int x = 0;
      if( TalesDir ) {
	files = get_dir( GetTalesDir() );
	if(taletotell){
	  y = member_array(taletotell,files);
	  if( y != -1 )
	    file = taletotell;
	  else {
	    environment(this_object())->eventPrint(NoTaleMessage);
	    return;
	  }
	}
	else 
	  file = files[random(sizeof(files))];
	AlreadyTellingTale = 1;
	Tales = ([]);
	msg = explode( read_file( TalesDir + file ), "\n" );
	foreach( string str in msg ) {
	  Tales[x++] = explode( str, "#" );
	}
      }
      if( !Tales ) return; 
      call_out( (: TellTale :), 1, 0 );
    }
    
}


void TellTale( int part ) {
    if( part == sizeof( keys( Tales ) ) ) {
        AlreadyTellingTale = 0;
        return;
    }
    if( living(this_object()) ) this_object()->eventForce( Tales[part][0] );
    else environment(this_object())->eventPrint( Tales[part][0] );
    call_out( (: TellTale :), to_int( Tales[part][1] ), part + 1 );
    return;
}
