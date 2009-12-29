#include <lib.h>
#include <position.h>
#include ROOMS_H
#include <vendor_types.h>
inherit LIB_ITEM;

varargs mixed eventRead(object reader, mixed str){
    object ob, playerob;
    int corpse;
    object *inv;
    object npc;
    int err;
    string basefile;
    string lang = this_object()->GetLanguage();
    object zombie;
    catch(zombie = new("/shadows/zombie"));
    if(!str || !objectp(str) || !zombie){
        return ::eventRead(reader);
    } 
    if(reader->GetLanguageLevel(lang) < 100){
        return ::eventRead(reader);
    }
    ob = str;
    if(ob->isCorpse()) corpse = 1;

    if(living(ob)) {
        write("You can't raise the living.");
        return 1;
    }

    if(base_name(ob) != LIB_CORPSE){
        write("You can only animate corpses made of meat.");
        return 1;
    }

    if(environment(ob) != environment(this_player())) {
        write(capitalize(ob->GetKeyName())+" isn't here.");
        return 1;
    }

    tell_player(this_player(),"You read the scroll, and with a roar "+
            "like thunder, "+ob->GetCapName()+" reanimates!");
    tell_room(environment(this_player()),this_player()->GetCapName()+" reads "+
            possessive(this_player())+ " scroll at "+ob->GetCapName()+" and "+
            nominative(ob)+" reanimates as you hear a thunderous roar!",
            ({ob, this_player()}) );

    basefile = ob->GetBaseFile();
    if(file_exists(basefile)){
        err = catch( npc = new(basefile) );
    }
    if(!npc){
        npc = new(LIB_SENTIENT);
        npc->SetShort("a zombie "+ob->GetRace());
        npc->SetLong("A zombie "+ob->GetRace());
    }

    npc->SetRace(ob->GetRace());
    npc->SetClass(ob->GetClass());
    npc->SetLevel(ob->GetLevel());
    npc->SetGender(ob->GetGender());
    npc->SetId( ({ ob->GetGender(), ob->GetRace(),
                ob->GetClass(), "zombie" }) );
    npc->SetAdjectives( ({ ob->GetGender(), ob->GetRace(),
                ob->GetClass(), "zombie", "undead" }) );
    npc->SetUndead(1);
    npc->SetKeyName(lower_case((ob->GetOwner()|| ob->GetRace())));

    foreach(mixed key, mixed val in ob->GetSkills()){
        npc->SetSkill(key, val["level"], val["class"]);
    }
    foreach(mixed key, mixed val in ob->GetStats()){
        npc->SetStat(key, val["level"], val["class"]);
    }
    npc->eventMove(ROOM_POD);
    npc->SetProperty("basefile", basefile);
    npc->init();
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
    npc->DisableActions(1);
    npc->SetUndead(1);
    npc->SetEncounter(0);
    npc->eventQuell();
    npc->SetUndeadType("zombie");
    ob->eventMove(ROOM_FURNACE);
    tell_room(environment(this_player()),"The scroll disintegrates into dust.");
    zombie->eventShadow(npc);
    this_object()->eventMove(ROOM_FURNACE);
    return 1;
}

void create(){
    string message = "This terrible scroll will reanimate a creature's "+
        "corpse. The resulting zombie will have little of the personality "+
        "of its original being...just the physical attributes, which will "+
        "slowly decay and inevitably fall apart. The zombie will obey any "+
        "commands spoken in a language that it understood in life. If "+
        "you are certain you must enter the dark world of necromancy, then "+
        "you can: read scroll at person";
    ::create();
    SetKeyName("scroll");
    SetId(({"scroll of raise dead","scroll of necromancy"}));
    SetAdjectives(({"terrible","raise dead","magic","necromancy","dark"}));
    SetShort("a dark scroll");
    SetLong("This is a dark scroll with some words written on it.");
    SetMass(1);
    SetDollarCost(500);
    SetVendorType(VT_TREASURE);
    SetReads( ([ "default" : message,
                ({ "words", "words written" }) : message,
                ]) );
    SetLanguage("English");
}

void init(){
    ::init();
}
