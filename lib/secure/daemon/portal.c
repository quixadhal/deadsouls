#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <save.h>

inherit LIB_DAEMON;

object blue, orange, bportal, oportal;
string oroom = "/domains/town/room/wtunnel3";
string broom = "/domains/campus/room/hazlab";
static int heart_count;

void CheckPortals(){
    mixed *tmp;
    if(catch(blue = load_object(broom)) || !blue){
        broom = ROOM_START;
        blue = load_object(broom);
    }
    if(catch(orange = load_object(oroom)) || !orange){
        oroom = ROOM_START;
        orange = load_object(oroom);
    }
    if(!bportal){
        tmp = objects( (: base_name($1) == "/secure/obj/portal_blue" &&
            clonep($1) :) );
        if(sizeof(tmp)){
            bportal = tmp[0];
            tmp -= ({ bportal });
            foreach(object foo in tmp){
                foo->eventDestruct();
                if(foo) destruct(foo);
            }
        }
    }
    if(!oportal){
        tmp = objects( (: base_name($1) == "/secure/obj/portal_orange" &&
            clonep($1) :) );
        if(sizeof(tmp)){
            oportal = tmp[0];
            tmp -= ({ oportal });
            foreach(object foo in tmp){
                foo->eventDestruct();
                if(foo) destruct(foo);
            }
        }
    }
    if(!bportal){
        catch(bportal = new("/secure/obj/portal_blue"));
    }
    if(!oportal){
        catch(oportal = new("/secure/obj/portal_orange"));
    }
    if(bportal && (!environment(bportal) || environment(bportal) != blue)){
        if(blue){
            bportal->eventMove(blue);
        }
    }
    if(oportal && (!environment(oportal) || environment(oportal) != orange)){
        if(orange){
            oportal->eventMove(orange);
        }
    }
}

void PurgePortals(){
    mixed *tmp = objects( (: base_name($1) == "/secure/obj/portal_orange" &&
        clonep($1) :) );
    if(sizeof(tmp)){
        foreach(object foo in tmp){
            if(foo != oportal){
                foo->eventDestruct();
                if(foo) destruct(foo);
            }
        }
    }
    tmp = objects( (: base_name($1) == "/secure/obj/portal_blue" &&
        clonep($1) :) );
    if(sizeof(tmp)){
        foreach(object foo in tmp){
            if(foo != bportal){
                foo->eventDestruct();
                if(foo) destruct(foo);
            }
        }
    }
}

void RegisterPortal(object env){
    string tmp = base_name(previous_object());
    if(tmp == "/secure/obj/portal_orange" && clonep(previous_object())){
        oportal = previous_object();
        orange = env;
        oroom = base_name(orange);
    }
    if(tmp == "/secure/obj/portal_blue" && clonep(previous_object())){
        bportal = previous_object();
        blue = env;
        broom = base_name(blue);
    }
    PurgePortals();
}

object GetBluePortal(){
    CheckPortals();
    return blue;
}

object GetOrangePortal(){
    CheckPortals();
    return orange;
}

void heart_beat(){
    heart_count++;
    if(!(heart_count % 30) && (!oportal || !bportal)){
        CheckPortals();
    }
    if(!(heart_count % 60)){
        PurgePortals();
    }
    if(!(heart_count % 300)){
        CheckPortals();
    }
    if(heart_count > 3600){
        heart_count = 0;
        unguarded( (: save_object(SAVE_PORTAL, 1) :) );
    }
}

void create(){
    ::create();
    SetNoClean(1);
    if( file_size( SAVE_PORTAL __SAVE_EXTENSION__ ) > 0 )
        unguarded( (: restore_object, SAVE_PORTAL, 1 :) );
    CheckPortals();
    set_heart_beat(1);
}

int eventDestruct(){
    unguarded( (: save_object(SAVE_PORTAL, 1) :) );
    return daemon::eventDestruct();
}

