/*    /d/standard/obj/mon/receptionist.c
 *    from Nightmare IV
 *    receptionist for the Nightmare Inn
 *    created by Descartes of Borgie and Lassondra of Sunshine@Nightmare 940424
 */

#include <lib.h>

inherit LIB_NPC;

#define ROOMS ([\
101: ([ "name": "regular", "cost":8 ]),\
        102: ([ "name": "regular", "cost":8 ]),\
        103:([ "name":"regular", "cost":8 ]),\
        104:([ "name":"regular", "cost":8 ]),\
        105:(["name":"deluxe", "cost":12 ]),\
        106:(["name":"deluxe", "cost":12 ]),\
        107:([ "name":"deluxe", "cost":12]),\
        108:(["name":"deluxe", "cost":12]),\
        109:(["name":"honeymoon suite", "cost":20]),\
        110:(["name":"honeymoon suite", "cost":20]),\
        ])

void create() {
    npc::create();
    SetKeyName( "linfield");
    SetId( ({ "linfield", "receptionist" }) );
    SetShort( "Linfield, the receptionist" );
    SetLong( "Linfield is the dignified receptionist for the "
            "Nightmare Inn. He stands tall behind the counter, "
            "waiting to help the next customer. He wears dark "
            "pants and blazer, with a white shirt and a blue "
            "and red striped cravate.");
    SetLevel(15);
    SetGender( "male" );
    SetRace( "human" );
    SetHealthPoints(700);
    SetClass( "mage" );
    SetSkill( "magic attack", 60);
    SetSkill( "melee", 70);
    SetCurrency( "gold", 50);
}

void init() {
    npc::init();
    add_action("cmd_rent", "rent");
    add_action("cmd_list", "list");
}

int cmd_list(string str) {
    write( "%^GREEN%^   -=       The Nightmare Inn    =-%^RESET%^");
    write( "\n");
    write( "%^GREEN%^     There are 3 types of rooms.%^RESET%^");     
    write( "%^GREEN%^     Regular: %^RESET%^"+currency_value(ROOMS[101]["cost"], "gold") );
    write( "%^GREEN%^     Deluxe: %^RESET%^" +currency_value(ROOMS[106]["cost"], "gold") ); 
    write( "%^GREEN%^     Honeymoon suite: %^RESET%^" +currency_value(ROOMS[109]["cost"], "gold") );
    write( "%^GREEN%^     To rent a room, type <rent 'room(like deluxe)'>%^RESET%^");
    return 1;
}

int cmd_rent(string str) {
    object ob;
    int *arr;
    int cost, i, x;

    if(str != "regular" && str != "deluxe" && str != "honeymoon suite")
        return notify_fail("Rent what sort of room?\n");
    i = sizeof(arr = keys(ROOMS));
    x = -1;
    while(i--) {
        if(ROOMS[arr[i]]["name"] == str &&
                !(environment(this_object())->query_occupied(arr[i]))) {
            x = arr[i];
            break;
        }
    }
    if(x == -1)
        return notify_fail("No such room is available right now.\n");
    cost = currency_value(ROOMS[x]["cost"], "gold");
    if(cost > this_player()->query_money("gold")) {
        this_object()->eventForce("speak You do not have enough money "
                "for one of those rooms!");
        return 1;
    }
    this_player()->AddCurrency(-cost, "gold");
    environment(this_object())->set_occupied(x);
    message("my_action", sprintf("You rent room %d of the Nightmare Inn.",
                x), this_player());
    message("other_action", sprintf("%s rents a room at the Nightmare Inn.",
                this_player()->query_cap_name()), environment(this_object()),
            ({ this_player() }) );
    ob = new(LIB_ITEM);
    ob->SetKeyName("hotel key");
    ob->SetShort("a hotel key");
    ob->SetLong(sprintf("The key to room %d at the Nightmare Inn.", x));
    ob->SetMass(50);
    ob->SetValue(100);
    ob->SetRead(sprintf("The Nightmare Inn.\nRoom %d.\n", x));
    ob->SetId( ({ "key", "hotel key", 
                environment(this_object())->query_key_id(x) }) );
    if(ob->move(this_player())) {
        message("my_action", "You drop your key!", this_player());
        message("other_action", sprintf("%s drops %s key!",
                    this_player()->query_cap_name(), possessive(this_player())),
                environment(this_object()), ({ this_player() }) );
        ob->move(environment(this_object()));
    }
    return 1;
}
