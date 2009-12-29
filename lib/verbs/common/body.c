/*    /cmds/player/body.c
 *    from the Dead Souls LPC Library
 *    gives player a report based on limb damage
 *    created by Blitz@Dead Souls
 */



#include <lib.h>
#include <rounds.h>

inherit LIB_VERB;

varargs void eventCheckBody(object ob, object receiver);

static void create() {
    verb::create();
    SetVerb("body");
    SetRules("", "LIV");
}

static string *DamageDegree = ({
        "is in critical condition!",
        "is battered beyond recognition.",
        "is severely wounded",
        "is terribly damaged.",
        "is in bad shape.",
        "is hurting.",
        "has a few bruises.",
        "is in decent shape.",
        "is in very good shape.",
        "is in excellent shape.",
        });

int livings_are_remote() { return 1; }

mixed can_body() {
    return 1;
}

    mixed can_body_liv() {
        if( !creatorp(this_player()) )
            return "Try: help body";
        return 1;
    }

mixed do_body() {
    message("other_action", this_player()->GetName()+" checks "+
            reflexive(this_player())+" for injuries.",
            environment(this_player()), this_player() );
    eventCheckBody(this_player());
    return 1;
}

mixed do_body_liv(object ob) {
    eventCheckBody(ob, this_player());
    return 1;
}

varargs void eventCheckBody(object ob, object receiver) {
    string ret, name;
    string *limbs;
    mapping mp;
    int i, damage;
    int *key;

    mp = ([ ]);
    if( !ob ) return;
    if( !receiver ) receiver = ob;
    i = sizeof(limbs = ob->GetLimbs());
    while(i--) {
        damage = to_int( percent( ob->GetHealthPoints(limbs[i]),
                    ob->GetMaxHealthPoints(limbs[i]) ));
        if( !mp[damage] ) mp[damage] = ({ limbs[i] });
        else mp[damage] += ({ limbs[i] });
    }
    i = sizeof(key = sort_array(keys(mp), 1));
    name = (ob == receiver ? "Your" : capitalize(possessive(ob)));
    ret = possessive_noun(ob->GetCapName()) + " bodily damage "
        "report:\n\n";
    foreach(damage in key) {
        string str;
        string color;
        int x;
        i = sizeof(limbs = mp[damage]);
        while(i--) {
            switch(damage) {
                case 0..3: color = "%^BOLD%^%^RED%^"; break;
                case 4..10: color = "%^RED%^";        break;
                case 11..20: color = "%^YELLOW%^";    break;
                default: color = "";
            }
            if( damage > 97 )
                str = name +" "+ limbs[i] + " is in perfect condition.";
            else if( damage < 4 )
                str = name+" "+ limbs[i] +" is about to fall off!";
            else {
                x = damage / 10;
                if(x>9) x = 9;
                str = name +" "+ limbs[i] +" "+ DamageDegree[x];
            }
            if( creatorp(receiver) )
                ret += sprintf("%s%-45s %s(%d / 100%%)%s\n", color, str, 
                        "", damage,"%^RESET%^");
            else ret += (color + str + "%^RESET%^\n");
        }
    }
    if( i = sizeof(limbs = ob->GetMissingLimbs()) ) {
        ret += "\n"+(ob == receiver ?
                "You are missing " : ob->GetName()+" is missing ");
        switch(i) {
            case 0: break;
            case 1: ret += "a "+limbs[0]+"."; break;
            case 2: ret += "a "+limbs[0]+" and a "+limbs[1]+"."; break;
            default:  ret += "a "+implode(limbs[0..(i-2)], ", ")+" and "
                      "a "+limbs[i-1]+".";
        }
    }
    receiver->eventPage(explode(ret, "\n"), "info");
    return;
}

    string GetHelp(string str) {
        if( creatorp(this_player()) )
            return "Syntax: body [LIVING]\n\n"
                "This command will display the current limb damage "
                "statistics of the living object named.";
        else
            return "Syntax: body\n\n"
                "This command will display your current limb damage "
                "statistics.  The limbs will be displayed in order "
                "of the most damaged to the least.";
    }
