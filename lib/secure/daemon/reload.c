#include <lib.h>
#include <config.h>

inherit LIB_DAEMON;
mapping Reloadees = ([]);

static void create() {
    daemon::create();
    set_heart_beat(1);
}

varargs int eventReload(mixed what, int when){
//tc("received thing","cyan");
if(!when) when = time();
else when += time();
if(!what) return 0;
if(stringp(what)){
if(last(what,2) == ".c") what = trim(what,2);
what = find_object(what);
if(!what) return 0;
}
if(!objectp(what)) return 0;
if(Reloadees[what]) return 0;
Reloadees[what] = when;
}

void heart_beat(){
foreach(mixed key, mixed val in Reloadees){
if(time() >= val){
map_delete(Reloadees,key);
//tc("bink: "+identify(key));
reload(key);
}
}
}

mapping GetReloads(){
return copy(Reloadees);
}
