/*    /daemon/unique.c
 *    from the Dead Souls LPC Library
 *    handles making objects unique
 *    created by Descartes of Borg 950529
 */

#include <lib.h>
#include <save.h>

inherit LIB_DAEMON;

private mapping Objects;

static void create() {
    daemon::create();
    Objects = ([]);
    if( file_size(SAVE_UNIQUE __SAVE_EXTENSION__) > 0 )
      unguarded( (: restore_object, SAVE_UNIQUE :) );
}

void eventTouchObject() {
    string fn;

    if( !((int)previous_object()->GetUnique()) ) return;
    fn = base_name(previous_object());
    Objects[fn] = time();
    unguarded( (: save_object, SAVE_UNIQUE :) );
}

object GetUniqueCopy(string fn, int rare) {
    object ob;
    int x;

    if( !Objects[fn] ) {
	ob = new(fn);
	if( !((int)ob->GetUnique()) ) return ob;
	else Objects[fn] = time();
	unguarded( (: save_object, SAVE_UNIQUE :) );
	return ob;
    }
    x = time() - Objects[fn];
    if( x < (86400 * rare) ) return 0;
    if( ob = new(fn) ) Objects[fn] = time();
    unguarded( (: save_object, SAVE_UNIQUE :) );
    return ob;
}
