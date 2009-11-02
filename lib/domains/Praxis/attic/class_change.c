#define addi(x,y) AddItem(x,y)
#define adde(x,y) AddExit(y) 

inherit "/lib/std/room";

void create() {    

    SetAmbientLight(30);
    SetShort("Magical Mind Altering Altar");
    SetLong(
            "This room is clean and has almost brand new furniture in it.  A strange\n"+
            "tilted altar is in the middle of the room.  Tables with wheels, which are\n"+
            "covered with magical devices, have been pushed up against the walls.\n");
    addi("furniture",
            "The furniture is all made of oak.\n");
    addi("altar",
            "The altar is made out of wood and is tilted at a strange angle.\n");
    addi("tables",
            "The tables are covered with various wands, rings, orbs, magical herbs, and\n"+
            "some things you cannot even identify.\n");
    addi("table",
            "Each table is made out of sturdy oak and can be wheeled around the room.\n");
    addi("devices","The devices are very peculiar.\n");
    addi("device",
            "One device catches your eye.  It is squirting water down a drain.\n");
    addi("water","The water looks cool and refreshing.\n");
    addi("drain","The drain is on the floor.\n");
    addi("floor","The floor is well swept granite.\n");
    addi("walls","The stone walls are bare of decorations.\n");
    addi("wall","All the walls are the same.\n");
    addi("stone","All the stone in this room is granite.\n");
    addi("granite","The granite is grey and rough.\n");
    //new("/obj/mon/wizard")->move(this_object());
}

void init()
{
    ::init();
    add_action("changing","change");
    add_action("helping","help");
}

int helping(string str)
{
    if(str != "changing") return 0;
    write(
            "The cost for changing from one class to another is half your experience and\n"+
            "five-thousand gold pieces.\n\n"+
            "To change your class type 'change class to <type>' where type is the class\n"+
            "you wish to change to.  The wizard will then operate on you to make the\n"+
            "adjustments.  It won't hurt, really, not much anyway.\n");
    return 1;
}

int changing(string str)
{
    int i;

    if(!present("mad wizard"))
    {
        write("The mad wizard is not in to fix you up.\n");
        return 1;
    }
    if(!str)
    {
        write("How can the wizard operate if you do not give "+
                "instructions?\n");
        say("The mad wizard bonks "+this_player()->query_cap_name()+
                " over the head.\n");
        return 1;
    }
    //wtf does this do?
    //str=extract(str, 9);
    if(str == this_player()->query_class())
    {
        write("Mad Wizard says: yer even crazier than I am!\n");
        return 1;
    }
    switch(str)
    {
        case "fighter":
            case "cleric":
            case "rogue":
            case "mage":
            case "monk":
            i=1;
        break;
        default: i=0;
    }
    if(!i)
    {
        write("Mad Wizard says: what kind of class is that?\n");
        return 1;
    }
    if( this_player()->query_money("gold") < 5000)
    {
        write("Mad Wizard says: You need more money than that.\n");
        return 1;
    }
    this_player()->add_money("gold", - 5000);
    this_player()->add_exp( - (this_player()->query_exp() / 2) );
    this_player()->set_class(str);
    write("The wizard picks up a wand off of a table and zaps you!\n");
    say("The wizard zaps "+this_player()->query_cap_name()+" with a wand.\n");
    return 1;
}
