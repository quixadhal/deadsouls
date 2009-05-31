#include <lib.h>
#include <vendor_types.h>
inherit LIB_PIPE;

void create(){
    ::create();
    SetKeyName("magic pipe");
    SetId( ({"sigpipe","magical pipe","pipe of insight"}) );
    SetAdjectives( ({"magic","magical","kalinash's", "sig", "smoking"}) );
    SetShort("a magic pipe");
    SetLong("This is a handsome pipe for smoking, made of ivory. There "
            "is some sort of inscription written on it.");
    SetMass(5);
    SetBaseCost("silver", 300);
    SetVendorType(VT_TREASURE);
    SetRadiantLight(1);
    SetMaxFuel(300);
    SetItems( ([
                "inscription" : "An inscription on the pipe you can read.",
                ]) );
    SetRead( ([
                "default" : "Try: read inscription on pipe",
                "inscription" : "Kalinash's Pipe of Insight",
                ]) );
    SetLanguage("common");
    SetFuelAmount(50);
    SetRefuelable(1);
    SetFuelType("smokable material");
}

void init(){
    ::init();
}

varargs mixed eventLight(object who, object tool){
    SetBaseCost("silver", 1);
    return ::eventLight(who, tool);
}

mixed eventSmoke(object who, object what){
    if(!GetLit()){
        write("It is not lit!");
        return 1;
    }
    write("You smoke your "+remove_article(GetShort())+".");
    say(who->GetName()+" smokes from "+possessive(who)+" "
            +remove_article(GetShort())+".");
    if((((time() - GetLastPuff()) > 10) && random(100) < 25) ||
            creatorp(this_player())){
        object bonus = new(LIB_BONUS);
        object *kbonuses = filter(all_inventory(this_player()),
                (: stringp($1->GetBonusName()) && 
                 $1->GetBonusName() == "kpipe_bonus" :) );
        if(sizeof(kbonuses) > 9) return bonus->eventDestruct();
        bonus->SetBonusName("kpipe_bonus");
        bonus->SetStats( ([ 
                    "intelligence" : 1,
                    "wisdom" : 1,
                    "strength" : -1,
                    "durability" : -1,
                    ]) );
        bonus->SetBonusDuration(300);
        if(bonus->eventMove(who)){
            write("You feel you have a slightly better understanding"+ 
                    " of the world.");
        }
    }    
    lastpuff = time();
    eventDecreaseFuel(1);
    if( !GetFuelAmount() ) eventBurnOut();
    return 1;
}

int eventBreak(){
    return 0;
}
