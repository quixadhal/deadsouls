#include <lib.h>
#include <daemons.h>
#include <modules.h>
#include ROOMS_H
#include <commands.h>
#include <position.h>

inherit LIB_VERB;
string *carried = ({});

static void create() {
    verb::create();
    SetVerb("resurrect");
    SetRules("OBJ", "here");
    SetErrorMessage("resurrect what?");
    SetHelp("Syntax: resurrect <OBJ>\n\n"
            "Bring back to life something that died. When used on the corpse\n"
            "of a player, it brings them back from death without skill or\n"
            "experience penalties.\n"
            "See also: zap, dest");
}

mixed can_resurrect_obj(string str) { 
    if(!creatorp(this_player())) 
        return "This command is only available to creators.";
    else return 1;
}

mixed do_resurrect_obj(object ob) {
    int corpse;
    object playerob;
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

    tell_player(this_player(),"You wave your hand, and with a flash "+
            "of light, "+ob->GetCapName()+" comes back to life!");
    tell_player(ob,capitalize(this_player()->GetKeyName())+" waves "+
            possessive(this_player())+
            " hand, and with a flash of light, you come back from the dead!");
    tell_room(environment(this_player()),this_player()->GetCapName()+" waves "+
            possessive(this_player())+
            " hand, and with a flash of light, "+ob->GetCapName()+
            " comes back to life!",
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
    return 1;
}
