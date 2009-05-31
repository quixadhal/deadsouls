#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mapping Tracked = ([]);
mapping Trackers = ([]);

int TrackLiving(object ob);

void create(){
    ::create();
    if(!Tracked) Tracked = ([]);
}

varargs mapping GetTracked(mixed ob){
    if(ob) return copy(Tracked[ob]);
    return copy(Tracked);
}

int AddTrack(object trackee, object tracker){
    if(!trackee || !tracker) return 0;
    if(!living(trackee)) return 0;
    if(!Tracked[trackee]) TrackLiving(trackee);
    Tracked[trackee]["trackers"] = 
        singular_array(Tracked[trackee]["trackers"] + ({ tracker }));
    return 1;
}

int RemoveTrack(object trackee, object tracker){
    if(!trackee || !tracker) return 0;
    if(!Tracked[trackee]) TrackLiving(trackee);
    Tracked[trackee]["trackers"] =
        singular_array(Tracked[trackee]["trackers"] - ({ tracker }));
    return 1;
}

static int NotifyTrackers(object ob){
    Tracked[ob]["trackers"]->ReceiveTrackingData( 
            ([ "object" : ob, "x" : Tracked[ob]["coords"]["x"],
             "y" : Tracked[ob]["coords"]["y"],
             "z" : Tracked[ob]["coords"]["z"], ]) );
    return 1;
}

int TrackLiving(object ob){
    object env;
    mapping coords;
    int ret;
    env = room_environment(ob);
    if(env){
        coords = ROOMS_D->GetCoordinateMap(env);
    }
    if(sizeof(coords)){
        if(!Tracked[ob]) Tracked[ob] = ([ "trackers" : ({}) ]);
        Tracked[ob]["coords"] = coords;
        ret = 1;
    }
    if(Tracked[ob] && sizeof(Tracked[ob]["trackers"])) NotifyTrackers(ob);
    return ret;
}
