#include <lib.h>
#include <position.h>
#include ROOMS_H
#include <vendor_types.h>
inherit LIB_ITEM;

varargs mixed eventRead(object reader, mixed str){
    object ob, playerob;
    int corpse;
    string lang = this_object()->GetLanguage();
    if(!str || !objectp(str)){
        return ::eventRead(reader);
    } 
    if(reader->GetLanguageLevel(lang) < 100){
        return ::eventRead(reader);
    }
    ob = str;
    if(ob->isCorpse()) corpse = 1;

    if(interactive(ob)) playerob = ob;

    if( ob->isPlayer() ) playerob = ob->GetPlayerob();

    if( ob->isPlayer() && !playerob ){
        write("You cannot resurrect a player that isn't logged on.");
        return 1;

    }
    if((playerob && !playerob->GetGhost()) || living(ob)) {
        write("You can't resurrect the living.");
        return 1;
    }

    if(base_name(ob) != LIB_CORPSE){
        write("You can only resurrect flesh-based creatures.");
        return 1;
    }

    if(environment(ob) != environment(this_player())) {
        write(capitalize(ob->GetKeyName())+" isn't here.");
        return 1;
    }

    tell_player(this_player(),"You read the scroll, and with a flash "+
            "of light, "+ob->GetCapName()+" comes back to life!");
    tell_player(ob,capitalize(this_player()->GetKeyName())+" reads "+
            possessive(this_player())+ " scroll at you, and "+
            "you come back from the dead!");
    tell_room(environment(this_player()),this_player()->GetCapName()+" reads "+
            possessive(this_player())+ " scroll at "+ob->GetCapName()+" and "+
            nominative(ob)+" comes back to life!",
            ({ob, this_player()}) );
    if(playerob){
        object *inv;
        playerob->eventRevive(1);
        playerob->eventMove(environment(this_player()));
        inv = all_inventory(ob);
        if(sizeof(inv)) inv->eventMove(playerob);
        inv = all_inventory(ob);
        if(sizeof(inv)) inv->eventMove(environment(this_player()));
        ob->eventMove(ROOM_FURNACE);
        playerob->eventDescribeEnvironment();
    }
    else {
        object *inv;
        object npc;
        int err;
        string basefile = ob->GetBaseFile();
        err = catch( npc = new(basefile) );
        if(!npc){
            npc = new(LIB_SENTIENT);
            npc->SetRace(ob->GetRace());
            npc->SetClass(ob->GetClass());
            npc->SetLevel(ob->GetLevel());
            npc->SetGender(ob->GetGender());
            npc->SetKeyName(lower_case((ob->GetOwner()|| ob->GetRace())));
            npc->SetShort((ob->GetLivingShort()|| "A "+ob->GetRace()));
            npc->SetLong((ob->GetLivingLong() || "A "+ob->GetRace()));
        }
        foreach(mixed key, mixed val in ob->GetSkills()){
            npc->SetSkill(key, val["level"], val["class"]);
        }
        foreach(mixed key, mixed val in ob->GetStats()){
            npc->SetStat(key, val["level"], val["class"]);
        }
        npc->eventMove(ROOM_POD);
        npc->ResetCurrency();
        if(sizeof(all_inventory(npc))){
            all_inventory(npc)->eventMove(ROOM_FURNACE);
        }
        inv = all_inventory(ob);
        if(sizeof(inv)) inv->eventMove(npc);
        inv = all_inventory(ob);
        if(sizeof(inv)) inv->eventMove(environment(this_player()));
        foreach(string element in ob->GetMissingLimbs()){
            npc->RemoveLimb(element);
        }
        if(ob){
            mapping oldequipped = ob->GetEquipped();
            string *oldkeys = keys(oldequipped);
            foreach(object thing in all_inventory(npc)){
                if(member_array(file_name(thing),oldkeys) != -1){
                    object oldob = oldequipped[file_name(thing)]["object"];
                    string *where = oldequipped[file_name(thing)]["where"];
                    if(objectp(oldob)){
                        if(oldob->CanEquip(npc, where)){
                            oldob->eventEquip(npc, where);
                        }
                    }
                }
            }
        }
        npc->SetPosition(POSITION_LYING);
        npc->eventMove(environment(this_player()));
        ob->eventMove(ROOM_FURNACE);
    }
    tell_room(environment(this_player()),"The scroll disintegrates into dust.");
    this_object()->eventMove(ROOM_FURNACE);
    return 1;
}

void create(){
    string message="To bring someone back to life, the way they were before "+ 
        "death took them, and with no penalties to their experience or "+
        "abilities, then go to their corpse, and: read scroll at person";
    ::create();
    SetKeyName("scroll");
    SetId(({"scroll of resurrection"}));
    SetAdjectives(({"resurrection","magic"}));
    SetShort("a scroll");
    SetLong("This is a scroll with some words written on it.");
    SetMass(1);
    SetDollarCost(1000);
    SetVendorType(VT_TREASURE);
    SetReads( ([ "default" : message,
                ({ "words", "words written" }) : message,
                ]) );
    SetLanguage("Edhellen");
}

void init(){
    ::init();
}
