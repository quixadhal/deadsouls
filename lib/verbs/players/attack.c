/*    /verbs/players/attack.c
 *    from the Dead Souls Mud Library
 *    a very violent command
 *    created by Descartes of Borg 960512
 *    Version: @(#) attack.c 1.3@(#)
 *    Last Modified: 96/10/20
 */

#include <lib.h>
#include <daemons.h>
#include <position.h>
#include "include/attack.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("attack");
    SetRules("LVS", "only LVS","LVS only");
    SetSynonyms("kill", "smite", "waste", "hit", "gank");
    SetErrorMessage("Attack whom?");
    SetHelp("Syntax: attack <LIVING>\n"
            "        attack all of <LIVING>\n"
            "        attack all\n\n"
            "This command initiates combat with a living being or group "
            "of living beings using any wielded weapons or your bare hands.  "
            "Be very careful not to issue the \"attack all\" with other "
            "players in the room or you will be guilty of attempted player "
            "killing.\n"
            "See also: wimpy, ignore, target");
}

varargs mixed can_attack_liv(object target) {
    int pos = this_player()->GetPosition();

    if( this_player()->GetParalyzed() ) {
        return "You cannot move!";
    }
    if( pos == POSITION_SITTING || pos == POSITION_LYING &&
            !RACES_D->GetLimblessCombatRace(this_player()->GetRace()) ){
        return "You cannot attack in that position!";
    }
    if( this_player() && environment(this_player()) &&
            environment(this_player())->GetProperty("no attack") ) {
        return "A mystical force prevents your malice.";
    }
    return 1;
}

mixed can_attack_only_liv(object target){
    return can_attack_liv(target);
}

mixed can_attack_liv_only(object target){
    return can_attack_liv(target);
}

mixed do_attack_liv(object target) {
    return do_attack_lvs(({ target }));
}

mixed do_attack_only_liv(object target){
    return do_attack_lvs(({ target }), 1);
}

mixed do_attack_liv_only(object target){
    return do_attack_lvs(({ target }), 1);
}

varargs mixed do_attack_lvs(mixed *targets, int exclusive) {
    object *obs, *tmpobs;
    object *noattack;
    string tmp;

    if(this_player()->GetDying()) return 0;

    noattack = ({});
    tmpobs = ({});
    obs = filter(targets, (: objectp($1) && !($1->GetInvis()) :));
    targets -= ({ this_player() });

    if(!sizeof(targets)){
        write("There is nobody to attack.");
        return 1;
    }

    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(targets, (: $1 :));
        foreach(string *lines in ua) this_player()->eventPrint(lines[0]);
        return 1;
    }
    if(exclusive){
        foreach(object entity in get_livings(environment(this_player()))){
            if(member_array(entity,obs) == -1) noattack += ({ entity });
        }
        if(sizeof(noattack)) this_player()->AddNonTargets(noattack);
    }
    foreach(object subobj in obs){
        if(subobj == this_player()) continue;
        if(member_array(this_player(),subobj->GetEnemies()) != -1 &&
                member_array(subobj,this_player()->GetNonTargets()) != -1){
            write("You are already fighting "+subobj->GetName()+"!");
        }
        else {
            mixed attackable = subobj->CanAttack(this_player());
            if(intp(attackable) && attackable) tmpobs += ({ subobj });
            else if(stringp(attackable)) write(attackable);
        }
    }

    obs = tmpobs;
    if(!sizeof(obs)) return 1;

    this_player()->SetAttack(obs);
    tmp = item_list(obs);
    obs->eventPrint(this_player()->GetName() + " attacks you!");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " attacks " + tmp + "!",
            ({ this_player(), obs... }));
    this_player()->eventPrint("You advance towards " + tmp + ".");
    return 1;
}
