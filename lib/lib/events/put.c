/*    /lib/put.c
 *    from the Dead Souls Object Library
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
    if( !environment() ) { destruct(this_object()); return 1; }
    if( environment() != this_player() ) return 0;
    if( !PreventPut ) return 1;
    if( stringp(PreventPut) && PreventPut == "PERMIT" ) return 1;
    if( intp(PreventPut) ) return 0;
    if( stringp(PreventPut) ) return PreventPut;
    if( objectp(PreventPut) ) {
	if( PreventPut == who )
	    return "You cannot put " + GetShort() + " anywhere.";
	else return 1;
    }
    else if(functionp(PreventPut)) return evaluate(PreventPut, who);
    else {
	return "It seems you're unable to do that right now.";
    }
}

varargs mixed eventPut(object who, object storage, string prep) {
    int depth;

    if(!prep || prep == "") prep = " into ";
    if(prep == " onto " && !inherits( "/lib/comp/surface", previous_object() ) ) {
	who->eventPrint("That isn't a load-bearing surface.");
	return 0;
    }

    if(prep == " into " && inherits( "/lib/comp/surface", previous_object() ) ) {
	who->eventPrint("That's a surface. Try \"put on\"");
	return 0;
    }

    if( !eventMove(storage) ) {
	who->eventPrint("There is not enough room in there!");
	return 0;
    }
    who->eventPrint("You put " + GetShort() + prep +
      (string)storage->GetShort() + ".");
    environment(who)->eventPrint((string)who->GetName() + " puts " +
      GetShort() + prep +
      (string)storage->GetShort() + ".", who);
    if(inherits("/lib/std/storage",this_object())) {
	depth = this_object()->GetRecurseDepth();
	if(depth && inherits("/lib/std/storage",storage)) storage->AddRecurseDepth(depth); 
    }

    return 1;
}

static void create() {
    PreventPut = 0;
}

mixed direct_put_obj_word_obj(object ob, string wrd, object ob2) {
    return CanPut(this_player());
}

