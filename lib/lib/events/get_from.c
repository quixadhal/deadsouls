/*    /lib/events/get_from.c
 *    From the Dead Souls V Object Library
 *    Handles get from/put in
 *    Created by Descartes of Borg 961221
 *    Version: @(#) get_from.c 1.1@(#)
 *    Last modified: 96/12/22
 */

// abstract methods
int AddCarriedMass(int amount);
string GetShort();
// end abstract methods

mixed CanGetFrom(object who, object item) {
    if( !item ) {
	return 1;
    }
    if( environment(item) != this_object() ) {
	return "#It is not in there!";
    }
    if( (environment() != environment(this_player())) &&
	(environment() != this_player()) ) {
	return "#" + capitalize(GetShort()) + " is not in reach.";
    }
    return 1;
}

mixed CanPutInto(object who, object item) {
    object env;
    
    if( item == this_object() ) {
	return "#You cannot change the laws of physics.";
    }
    env = environment();
    if( env != this_player() && env != environment(this_player()) ) {
	return "#It is not within reach.";
    }
    return 1;
}

mixed eventGetFrom(object who, object array what) {
    object array fin = ({});
    string array shorts;
    mapping mp = ([]);
    string msg;
    int i, maxi;

    foreach(object ob in what ) {
        if( environment(ob) != this_object() ) {
            continue;
        }
	if( ob->CanGet(who) != 1 ) {
	    continue;
	}
	if( !who->CanCarry(ob->GetMass()) ) {
	    continue;
	}
	if( !ob->eventMove(who) ) {
	    who->eventPrint("You have a problem getting " +
			    ob->GetShort() + ".");
	    continue;
	}
	AddCarriedMass( -(ob->GetMass()) );
	fin += ({ ob });
    }
    what = fin;
    if( !(maxi = sizeof(what)) ) {
	return 0;
    }
    shorts = map(what, (: (string)$1->GetShort() :));
    for(i=0; i<maxi; i++) {
	mp[shorts[i]]++;
    }
    maxi = sizeof(shorts = keys(mp));
    for(i=0, msg = ""; i<maxi; i++) {
	if( mp[shorts[i]] < 2 ) {
	    msg += shorts[i] + "%^RESET%^";
	}
	else {
	    msg += consolidate(mp[shorts[i]], shorts[i]) +  "%^RESET%^";
	}
	if( i == maxi-2 ) {
	    msg += ", and ";
	}
	else if( i != maxi-1 ) {
	    msg += ", ";
	}
    }
    send_messages("get", "$agent_name $agent_verb " + msg +
		  " from $target_name.", who, this_object(), environment(who));
    return 1;
}

mixed eventPutInto(object who, object what) {
    return what->eventPut(who, this_object());
}

int inventory_accessible() {
    return 1;
}

int inventory_visible() {
    return 1;
}

mixed indirect_get_obj_from_obj(object item) {
    return CanGetFrom(this_player(), item);
}

mixed indirect_get_obj_out_of_obj(object item) {
    return indirect_get_obj_from_obj(item);
}

mixed indirect_get_obs_from_obj(object array items, object storage) {
    if( !items ) {
	return (storage == this_object());
    }
    return 1;
}

mixed indirect_get_obs_out_of_obj(object array items, object storage) {
    return indirect_get_obs_from_obj(items, storage);
}

mixed indirect_put_obj_word_obj(object what, string word, object storage) {
    if( !what ) {
	return (storage == this_object());
    }
    return CanPutInto(this_player(), what);
}

mixed indirect_put_obs_word_obj(object *items, string wrd, object storage) {
    if( !items ) {
	return (storage == this_object());
    }
    return 1;
}
