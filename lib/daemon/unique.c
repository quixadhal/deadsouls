/*    /daemon/unique.c
 *    from the Dead Souls LPC Library
 *    handles making objects unique
 *    created by Descartes of Borg 950529
 */

#include <lib.h>
#include <save.h>

inherit LIB_DAEMON;

private mapping Objects;
static string SaveFile;

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_UNIQUE);
    Objects = ([]);
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile);
    }
}

void eventTouchObject() {
    string fn;

    if( !(previous_object()->GetUnique()) ) return;
    fn = base_name(previous_object());
    Objects[fn] = time();
    SaveObject(SaveFile);
}

object GetUniqueCopy(string fn, int rare) {
    object ob;
    int x;

    if( !Objects[fn] ) {
        ob = new(fn);
        if( !(ob->GetUnique()) ) return ob;
        else Objects[fn] = time();
        SaveObject(SaveFile);
        return ob;
    }
    x = time() - Objects[fn];
    if( x < (86400 * rare) ) return 0;
    if( ob = new(fn) ) Objects[fn] = time();
    SaveObject(SaveFile);
    return ob;
}

mapping GetUniques(){
    return copy(Objects);
}

mapping ResetUniques(){
    Objects = ([]);
    SaveObject(SaveFile);
}
