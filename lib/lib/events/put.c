/*    /lib/put.c
 *    from the Dead Souls V Object Library
 *    handles things which may be put into other objects
 *    created by Descartes of Borg 960114
 */

private mixed PreventPut;

// abstract methods
string GetShort();
mixed CanDrop(object who);
mixed eventMove(mixed dest);
// end abstract methods

mixed SetPreventPut(mixed val) {
    return (PreventPut = val);
}

mixed GetPreventPut() {
    return PreventPut;
}

mixed CanPut(object who) {
    mixed tmp;

    if( (tmp = CanDrop(who)) != 1 ) return tmp;
    if( environment() != this_player() ) return 0;
    if( !PreventPut ) return 1;
    if( intp(PreventPut) ) return 0;
    if( stringp(PreventPut) ) return PreventPut;
    if( objectp(PreventPut) ) {
	if( PreventPut == who )
	  return "You cannot put " + GetShort() + " anywhere.";
	else return 1;
    }
    else return evaluate(PreventPut, who);
}

mixed eventPut(object who, object storage) {
    if( !eventMove(storage) ) {
	who->eventPrint("There is not enough room in there!");
	return 0;
    }
    who->eventPrint("You put " + GetShort() + " into " +
		    (string)storage->GetShort() + ".");
    environment(who)->eventPrint((string)who->GetName() + " puts " +
                                 GetShort() + " into " +
				 (string)storage->GetShort() + ".", who);
    return 1;
}
					 
static void create() {
    PreventPut = 0;
}

mixed direct_put_obj_word_obj() {
    return CanPut(this_player());
}

