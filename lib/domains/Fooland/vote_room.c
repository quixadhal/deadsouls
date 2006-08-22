/*
// File:          /domains/Fooland/vote_room.c
// Purpose:       A generic inheritable voting room
// 93-03-16       Written by Douglas Reay (Pallando @TMI and other muds)
//                This file is part of the TMI-2 mudlib distribution.
//                Please keep this header if you use any of this code.
*/

#include <config.h>
#include <mudlib.h>

inherit ROOM;

#define DEFAULT_SAVE_FILE "/domains/Fooland/data/wiz_votes"
#define TMP_FILE TMP_DIR + geteuid( this_player() ) + ".prop"
#define DEFAULT_OPTIONS ({ "endorse", "reject", "abstain" })

mapping vote_data;
mapping new_data;
string save_file;
string *default_options;

// room_create() has to be called after the restore_object in create() so that
// the room description can be changed without losing the vote data.
void room_create()
{
  set( "light", 1 );
  set( "short", "A generic voting room" );
  set( "long", @EndText
An air of profound thought infuses the atmosphere of this room, possibly
because the issues decided here have been so weighty that the very fabric
of history resonates with their clamour, or more likely due to the dubious
substances smoked here by wizards of the old school.  Whatever the cause,
if you would like to stick your muddy oar into the turbulent waters of
today's issues, the commands necessary are explained by "help room",
(provided of course the admins, who convene votes here, verify your
propositions as being worthy of voted upon).
EndText
  );
  set( "exits", ([
    "east" : "/domains/Fooland/tower1",
  ]) );
}

// To alter the save_file in a voting room inheriting this one, add a function
//   create() { save_file = <...>; ::create(); }
// See at TMI-2 /domains/Conf/room/conf_voting_room.c for an example.
void create()
{
  ::create();
  seteuid( getuid() );
  if( !save_file ) save_file = DEFAULT_SAVE_FILE;
  //restore_object( save_file );
  if( !vote_data ) vote_data = ([]);
  if( !new_data ) new_data = ([]);
  if( !default_options ) default_options = DEFAULT_OPTIONS;
  room_create();
}

// These two functions are designed to be changed by an inheriting object.
// You could for example make a guild voting room where guild members can
// vote and the guild master can verify propositions.
int is_convener()
{
  return adminp(geteuid( this_player() ));
}

// Return 0 if the player isn't allowed to vote, else return the players
// name as it should appear on the proposition.
string is_voter()
{
  return capitalize( geteuid( this_player() ) );
}

void init()
{
  add_action( "cmd_help", "help" );
  add_action( "cmd_list", "list" );
  if( is_voter() )
  {
    add_action( "cmd_vote", "vote" );
    add_action( "cmd_propose", "propose" );
  }
  if( !is_convener() ) return;
  add_action( "cmd_nlist", "nlist" );
  add_action( "cmd_verify", "verify" );
  add_action( "cmd_proposer", "proposer" );
  add_action( "cmd_unverify", "unverify" );
  add_action( "cmd_edit", "edit" );
  add_action( "cmd_options", "options" );
  add_action( "cmd_proxy", "proxy" );
}

/************************ Functions anyone can use ************************/

int cmd_help( string a )
{
  if( a != "room" ) return 0;
  write( ""+
"help room     this message\n"+
"list          lists the proposals\n"+
"list <p>      lists proposal <p>\n"+
  "" );
  if( is_voter() ) write( ""+
"vote <p> <o>  Vote option <o> on proposal <p>\n"+
"vote <p>      Withdraw your vote on proposal <p>\n"+
"propose <p>   Make a new proposal <p> (subject to convener verification)\n"+
  "" );
  if( is_convener() ) write( ""+
"nlist <p>     List the unverified proposals.\n"+
"verify <p>    Move proposal <p> to the verified list.\n"+
"verify <p> as <new_p>     Rename proposal <p>\n"+
"unverify <p>  Move proposal <p> to the unverified list.\n"+
"              (Or, if <p> is already unverified, delete it.)\n"+
"proposer <p> <name>       Set the proposer of proposal <p> to <name>\n"+
"edit <p>      Edit text of proposal <p>\n"+
"options <p> <o>,<o>,...   Set the options for proposal <p> to <o>,<o>...\n"+
"proxy <voter> votes <p> <o>   Vote for someone else.\n"+
  "" );
  return 1;
}

int cmd_list( string a )
{
  string *names, option, *voters, ret;
  mapping proposal;
  int i, j;

  names = keys( vote_data );
  if( !sizeof( names ) )
  {
    write( "There are no current proposals.\n" );
    return 1;
  }
  if( !a )
  {
    ret = "";
    for( i = sizeof( names ) ; i ; i-- )
    {
      proposal = vote_data[names[(i-1)]];
      ret += names[(i-1)] + "\tproposed by " + proposal["proposer"] + "\n";
      ret += proposal["text"];
      ret += wrap( "Valid options: " + implode( proposal["options"], ", " ) );
    }
    this_player()-> more( explode( ret, "\n" ) );
    return 1;
  }
  proposal = vote_data[a];
  if( !mapp( proposal ) )
  {
    write( wrap( "Current proposals are: " + implode( names, ", " ) ) );
    return 1;
  }
  write( a + "\tproposed by " + proposal["proposer"] + "\n" );
  write( proposal["text"] );
  write( wrap( "Valid options: " + implode( proposal["options"], ", " ) ) );
  if( !sizeof( keys( proposal["votes"] ) ) )
  {
    write( "No votes yet cast on it.\n" );
  } else {
    i = sizeof( proposal["options"] );
    if( i > 1 ) write( "Total votes cast = " +
      sizeof( keys( proposal["votes"] ) ) + "\n" );
    for( ; i ; i-- )
    {
      option = proposal["options"][(i-1)];
      voters = proposal["cast"][option];
      j = sizeof( voters );
      write( "Option: "+option+" - "+j+" vote"+((j-1)?"s":"")+"\n" );
      if( j ) write( "  "+implode(voters,"\n  ")+"\n" );
    }
  }
  return 1;
}

/************************ Functions voters can use ************************/

// The optional second argument is used by cmd_proxy()
varargs int cmd_vote( string a, string voter )
{
  string name, option, cast;
  string *names;
  mapping proposal;

  if( !a )
    { notify_fail( "Syntax: vote <proposal> <option>\n" ); return 0; }
  if( sscanf( a, "%s %s", name, option ) < 2 )
    { name = a; option = 0; }
  names = keys( vote_data );
  if( !sizeof( names ) )
  {
    write( "There are no current proposals.\n" );
    return 1;
  }
  if( -1 == member_array( name, names ) )
  {
    write( wrap( "The current proposals are: " + implode( names, ", " ) ) );
    return 1;
  }
  proposal = vote_data[name];
  if( !voter ) voter = is_voter();
  if( !option )
  {
    if( cast = proposal["votes"][voter] )
    {
      proposal["cast"][cast] -= ({ voter });
      map_delete( proposal["votes"], voter );
      vote_data[name] = proposal;
      save_object( save_file );
      write( "You uncast your vote on proposal " + name + "\n" );
      return 1;
    }
    write( "You have no vote cast on proposal " + name + "\n" );
    return 1;
  }
  if( -1 == member_array( option, proposal["options"] ) )
  {
    write( wrap( "The valid options are: " +
      implode( proposal["options"], ", " ) ) );
    return 1;
  }
  if( cast = proposal["votes"][voter] )
    proposal["cast"][cast] -= ({ voter });
  proposal["votes"][voter] = option;
  proposal["cast"][option] += ({ voter });
  vote_data[name] = proposal;
  save_object( save_file );
  write( "You vote " + option + " on proposal " + name + "\n" );
  return 1;
}

int cmd_propose( string a )
{
  string tmp;

  if( !a ) { notify_fail( "Syntax: propose <name>\n" ); return 0; }
  if( -1 != member_array( a, keys( vote_data ) + keys( new_data ) ) )
    { notify_fail( "There is already a proposal named "+a+"\n" ); return 0; }
  notify_fail( "You may not have spaces in the proposal name.\n" );
  if( sscanf( a, "%*s %s", tmp ) ) return 0;
  write( "Enter the text of your proposal.\n" );
  this_player()-> edit( TMP_FILE, "callback_propose", this_object(), a );
  return 1;
}

// This function is used by cmd_propose()
int callback_propose( string a )
{
  mapping proposal;
  string text;

  proposal = ([]);
  text = read_file( TMP_FILE );
  rm( TMP_FILE );
  if( !text ) { notify_fail( "No proposal entered.\n" ); return 0; }
  proposal["text"] = text;
  proposal["proposer"] = is_voter();
  proposal["options"] = default_options;
  // This duplicates the get_options() incase someone's input_to is interupted.
  new_data[a] = proposal;
  write( "Your proposal will be added when someone verifies it.\n" );
  save_object( save_file );
  write( "Press <return> for the options to be "+
    implode( default_options, ", " ) + "\n" +
    "Otherwise, enter your new options, seperating each one with a comma\n"+
    "New options = " );
  input_to( "get_options", ({ 0, a }) );
  return 1;
}

// This function initalises a proposal to accept votes on its options,
// If any votes have already been cast then these are cleared.
// Used by get_options() and cmd_verify()
mapping init_votes( mapping proposal )
{
  int i;

  proposal["votes"] = ([]);
  proposal["cast"] = ([]);
  if( !proposal["options"] ) proposal["options"] = default_options;
  for( i = sizeof( proposal["options"] ) ; i ; i-- )
    proposal["cast"][( proposal["options"][(i-1)] )] = ({ });
  return proposal;
}

// This function is used by callback_propose() and cmd_options()
int get_options( string opts, mixed args )
{
  string *options;
  mapping proposal;

  if( opts ) options = explode( opts, "," );
  else options = default_options;
  proposal = ( args[0] ? vote_data[(args[1])] : new_data[(args[1])] );
  proposal["options"] = options;
  proposal = init_votes( proposal );
  if( args[0] ) vote_data[(args[1])] = proposal;
  else new_data[(args[1])] = proposal;
  write( wrap( "The options for proposal " + args[1] + " are: " +
    implode( options, ", " ) ) );
  save_object( save_file );
  return 1;
}

/************************ Functions conveners can use ************************/

int cmd_nlist( string a )
{
  if( a && !undefinedp( new_data[a] ) )
    write( wrap( "Unverified proposal " + a + " =\n" +
      dump_variable( new_data[a] ) ) );
  else if( sizeof( keys( new_data ) ) )
    write( wrap( "Names of unverified proposals: " + 
      implode( keys( new_data ), " " ) ) );
  else write( "There are no unverified proposals.\n" );
  return 1;
}

int cmd_verify( string a )
{
  mapping proposal;
  string name, new_name;
  int i;

  if( !a )
  {
    notify_fail( "Syntax: verify <name>[ as <new_name>]\n" );
    return 0;
  }
  if( sscanf( a, "%s as %s", name, new_name ) < 2 )
    { name = a; new_name = a; }
  if( !new_data[name] )
  {
    if( vote_data[name] && ( name != new_name ) )
    {
      vote_data[new_name] = vote_data[name];
      map_delete( vote_data, name );
      write( "Proposal " + name + " renamed as " + new_name + "\n" );
      save_object( save_file );
      return 1;
    }
    notify_fail( "I do not recognise " + name + " as the name of an "+
      "unverified proposal.\n" );
    return 0; 
  }
  proposal = init_votes( new_data[name] );
  vote_data[new_name] = proposal;
  map_delete( new_data, name );
  write( wrap( "ADDING PROPOSAL " + new_name + "\n" + identify( proposal ) ) );
  save_object( save_file );
  return 1;
}

int cmd_proposer( string a )
{
  string name, proposer;

  if( !a || sscanf( a, "%s %s", name, proposer ) <2 )
  {
    notify_fail( "Syntax: proposer <proposal> <proposer>\n" );
    return 0; 
  }
  if( !vote_data[name] ) 
  {
    notify_fail( name + " is not a verified proposal.\n" );
    return 0; 
  }
  vote_data[name]["proposer"] = proposer;
  save_object( save_file );
  write( "Ok.\n" );
  return 1;
}

int cmd_unverify( string a )
{
  if( !a ) { notify_fail( "Syntax: unverify <proposal_name>\n" ); return 0; }
  if( new_data[a] )
  {
    map_delete( new_data, a );
    save_object( save_file );
    write( "The unverified proposal " + a + " now deleted.\n" );
    return 1;
  }
  if( !vote_data[a] )
  {
    notify_fail( "No such proposal: " + a + "\n" );
    return 0;
  }
  new_data[a] = vote_data[a];
  map_delete( vote_data, a );
  save_object( save_file );
  write( "Proposal " + a + " moved to unverified list.\n" );
  return 1;
}

int cmd_edit( string a )
{
  mapping proposal;

  if( !a ) { notify_fail( "Syntax: edit <p>\n" ); return 0; }
  proposal = vote_data[a];
  if( !proposal ) { notify_fail( "No such proposal.\n" ); return 0; }
  rm( TMP_FILE );
  write_file( TMP_FILE, proposal["text"] );
  write( "Edit the text of the proposal:\n" );
  this_player()-> edit( TMP_FILE, "callback_edit", this_object(), a );
  return 1;
}

// This function is used by cmd_edit()
int callback_edit( string a )
{
  string text;

  text = read_file( TMP_FILE );
  rm( TMP_FILE );
  if( !text )
  {
    write( "Unverifying proposal.\n" );
    new_data[a] = vote_data[a];
    map_delete( vote_data, a );
    save_object( save_file );
    return 1;
  }
  vote_data[a]["text"] = text;
  save_object( save_file );
  write( "Ok.\n" );
  return 1;
}

int cmd_options( string a )
{
  string proposal_name, options;
  int veri;

  if( !a || ( sscanf( a, "%s %s", proposal_name, options ) < 2 ) )
  {
    notify_fail( "Syntax: options <p> <o1>,<o2>,<o3>,...\n" );
    return 0;
  }
  if( !undefinedp( vote_data[proposal_name] ) ) veri = 1;
  else if( !undefinedp( new_data[proposal_name] ) ) veri = 0;
  else { notify_fail( proposal_name + " is not a proposal.\n" ); return 0; }
  get_options( options, ({ veri, proposal_name }) );
  return 1;
}

int cmd_proxy( string a )
{
  string voter, vote;

  if( !a || ( sscanf( a, "%s votes %s", voter, vote ) < 2 ) )
  {
    notify_fail( "Syntax: proxy <voter> votes <proposal> <option>\n" );
    return 0;
  }
  return cmd_vote( vote, voter );
}

/************************ Functions debuggers can use ************************/

mixed query_vote_data( mixed a )
{
  if( !is_convener() ) return;
  if( a ) return vote_data;
  write( "vote_data = " + dump_variable( vote_data ) + "\n" );
  // If you don't have the dump_variable() simul_efun then don't worry
  // ... it does much the same as identify()
  return 1;
}

mixed query_new_data() { if( is_convener() ) return new_data; }
void set_new_data( mapping a ) { if( is_convener() ) new_data = a; }
void set_vote_data( mapping a ) { if( is_convener() ) vote_data = a; }

// NB If you had confidence about not losing data due to crashes, you could
// delete all the save_object() calls and put one in the remove() function.
