/*
//      get_object() and get_objects()
//      Created by Pallando@Ephemeral Dale   (92-06)
//      Created by Watcher@TMI  (92-09-27)
//      Revised by Watcher and Pallando (92-12-11)
//      Re-written by Pallando (92-12-18)
//      get_objects() added by Pallando@Tabor (93-03-02)
//      changed to use get_path() by Pallando@Dead Souls (93-05-28)
//
//      Use all possible methods to locate an object by the inputed 
//      name and return the object pointer if located.
// Ideas for future expansion (please, anyone feel free to add them if they
//                             have the time)
//   "wizards" - the subset of "users" who are wizards.
//   check the capitalized and lower_case version of str
//   check wizard's home directories.
//   :c - suffix indicating the children() of the previous object's base name.
//   :s - shadow of an object
//   :>func - the object returned by the base object->func() (useful for things
//            like referencing the monster attacking someone.
*/

varargs object get_object( string str, object player )
{
  object what;
  mixed tmp;

  // Prevent wizards finding things they shouldn't.

  if( !str ) return 0;
  if( !player || !living( player ) ) player = this_player();
  if( sscanf( str, "@%s", tmp )         &&
    ( tmp = get_object( tmp, player ) ) &&
    ( what = environment( tmp )       )    )
    return what;
  if( player )    //  Check existance of this_player()
  {
    if( str == "me" ) return player;
    if( what = present( str, player ) ) return what; // Inventory check
    if( what = environment( player ) )               // Environment check
    {
      if (str == "here" || str == "env" || str == "environment")
        return what;
      if( what = present( str, what ) ) return what;
    }
  }
 
  // Call might be made by a room so make a previous_object() check
  // first just to be sure
 
  if( what = present( str, previous_object() ) )  return what;
 
  //  Check to see if a living object matches the name

  if( what = find_player( str ) ) return what;
  if( what = find_living( str ) ) return what;
 
  //  Search for a matching file_name, completing path with 
  //  user's present path

  if( player )
  {
//  this option removed because Dead Souls doesn't support cwf
//  if( str == "cwf" ) str = (string)player-> query( "cwf" );
    str = absolute_path( (string)player-> get_path(), str );
  }
 
  //  Make sure the object is loaded into memory, if it exists

  catch( call_other( str, "???" ) );
 
  //  Finally return any object found matching the requested name

  return find_object( str );
}

// Created by Pallando@Tabor (93-03-02)
// player - as per get_object()
// no_arr - if specified, only 0 or an object will be returned,
//          otherwise an array of objects may also be returned.
// str - eg
//   "pallando" - returns the object, /lib/user#123
//   "pallando:i" - returns pallando's inventory
//   "pallando:e" - returns pallando's environment
//   "pallando:e:d:12" - returns the 12th object in the deep inventory of
//                       the room that pallando is in.
//   "caractacus:e:lady" - finds a lady of the court of King Caractacus 8-)
//   "users:rod" - searches the inventories of all users for a rod.
//   "users:e:guard" - searches the environments of all users for a guard.
varargs mixed get_objects( string str, object player, int no_arr )
{
  mixed base, tmp, ret;
  object what;
  int i, s;
// Hmm.  i and s do several jobs here.  It would be clearer to use different
// variables (with longer names) for each job.
// Is it worth slowing the function (using more memory) to do this?


  if( !str ) return 0;
  s = strlen( str );
  i = s;
  while( i-- && ( str[i..i] != ":" ) ); // a reverse sscanf
  if( ( i > 0 ) && ( i < ( s - 1 ) ) ) // of form "%s:%s"
  {
    base = get_objects( str[0..(i-1)], player );
    str = str[(i+1)..s];
    if( !base ) return 0;
    if( !pointerp( base ) ) base = ({ base });
    s = sizeof( base );
    ret = ({ });
    if( str == "e" )
    {
      while( s-- )
        if( tmp = environment( base[s] ) )
          ret += ({ tmp });
    } else if( str == "i" ) {
      while( s-- )
        if( tmp = all_inventory( base[s] ) )
          ret += ( pointerp( tmp ) ? tmp : ({ tmp }) );
    } else if( str == "d" ) {
      while( s-- )
        if( tmp = deep_inventory( base[s] ) )
          ret += ( pointerp( tmp ) ? tmp : ({ tmp }) );
    } else if( sscanf( str, "%d", i ) ) {
      if( ( i > -1 ) && ( i < s ) ) return base[i];
      else return 0;
    } else {
    // This is the location to add more syntax options if wanted such as
    // ith item in jth base object, all such items in all base objects, etc
      while( s-- )
        if( what = present( str, base[s] ) )
          return what;
      return 0;
    }
    switch( sizeof( ret ) )
    {
      case 0: return 0;
      case 1: return ret[0];
    }
    return( no_arr ? ret[0] : ret );
  }
  if( str == "users" )
  {
    ret = users();
    if( !no_arr ) return ret;
    if( sizeof( ret ) ) return ret[0];
    return 0;
  }
  return get_object( str, player );
}

/*
  NB

  It would be fairly simple to combine these two functions into one
varargs object get_object( string str, object player, int arr_poss )
  which will only return a single object unless the array_possible flag
  is passed.

  I have chosen not to do this however, since some muds may not wish to
  use the more complicated search routines and keeping get_objects() as
  a seperate simul_efun makes it easier to disable.
*/
