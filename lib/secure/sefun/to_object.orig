// file:    to_object.c (simul_efun)
// mudlib:  CynoMUD-II
// author:  Truilkan
// date:    1992 September 17
// purpose: Converts a string into the object it represents.  This object
//  is intended to be used as a parser by those wizard commands that need
//  to refer to objects by various means (such as dest and trans).

// to_object() returns the object that a given string references.
// strings are of the form 'object' or 'object@environment' where environment
// can also be of the form 'object@environment'.  object may also be of
// the form 'e(object)' which refers to the environment of object.
//
// object may optionally be prefixed with one of the following symbols:
//
//  !  (what follows names an object that is not an NPC or a player)
//  %  (what follows names a player)
//  $  (what follows names an NPC)
//  #  (what follows is an ordinal value, e.g. #3, #4, etc)
//  /  (what follows is a filename of an object)
//
// object may also be one of the words "here" or "me" where here refers
// to the environment of this player and "me" refers to this_player().
//
// example usage:

/*
   object obj;

  // find the object which has an id of "key" and is in player truilkan's
  // inventory.
  //
  obj = to_object("!key@%truilkan");
  //
  // find the orc NPC which is in the bag which is in the church 
  //
  obj = to_object("$orc@!bag@/room/church");
  //
  //  Destroy the 4th object in the inventory of the environment of this
  //  player (0 is the first object :)
  // 
  to_object("#3@here")->eventDestruct();
*/

static object
e(object p, int outside)
{
   if (outside) {
      return p ? environment(p) : 0;
   } else {
      return p;
   }
}

object
to_object(mixed target)
{
   int ch;
   string err;

   if (objectp(target)) { /* already an object */
      return target;
   } else if (stringp(target)) { /* is a string */
      string str, thing, env_desc, temp, path;
      object env, p, *list;
      int len, outside, is_object, place, which;

      outside = 0;
      place = 0;
      env = 0;
      str = (string)target;
      if (sscanf(str,"e(%s)",temp) == 1) {
         str = temp;
         outside = 1;
      }
      if (sscanf(str,"%s@%s", thing, env_desc) == 2) {
         str = thing;
         if (!(env = to_object(env_desc))) {
            return 0;
         }
         place = 1;
      }
      if (str == "here") {
         return e(environment(this_player()), outside);
      }
      if (str == "me") {
         return e(this_player(), outside);
      }
      if (!env) {
         env = this_player();
      }
      len = strlen(str);
      ch = str[0];
      if (ch == '/' || ch == '$' || ch == '#' || ch == '%' || ch == '!') {
         if (len < 2) {
            return 0;
         }
         if (ch != '/') {
            str = str[1..len-1];
            len--;
         }
      }
      p = 0;
      is_object = 0;
      switch (ch) {
      case '/' : /* filename */
         if(file_size(str + ".c") < 0) return 0; /* patch by val */
         return e(load_object(str), outside);
      case '#' : /* nth object */
         list = all_inventory(env);
         sscanf(str, "%d", which);
         return e(list[which], outside);
      case '$' : /* monster */
         p = find_living(str);
         if (!p || interactive(p)) {
            return 0; /* p non-existent or was a player */
         }
         break;
      case '%' :  /* player */
         p = find_player(str);
         if (!p) {
            return 0;
         }
         break;
      case '!' :
         is_object = 1;
         break;
      }
      if (!is_object) {
         if (!p) {
            p = find_player(str);
         }
         if (!p) {
            p = find_living(str);
         }
      }
      if (!p) {
         p = present(str, env);
      }
      if (p && (!place || present(p, env))) {
         return e(p, outside);
      }
      path = (string)this_player()->resolve_path(str);
      if(!path) return 0;
/* patch added by valodin to avoid searching for bad path */
      return e(load_object(path), outside);
   }
   return 0;
}
