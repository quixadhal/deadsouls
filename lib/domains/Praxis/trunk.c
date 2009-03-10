inherit "/lib/std/room";

void create() {
    ::create();
    SetShort("the upper trunk");
    SetLong(
            "A very large room has been carved out of the upper trunk of the mage tree. "+
            "A small stairway leads down to the entrance of the tree, while upwards it "+
            "leads out of the tree, and into the wide canopy of the branches. ");
    SetProperties( (["light"     : 2, 
                "indoors"   : 1 ,
                "no castle" : 1]) );
    SetItems( ([
                "mage tree"   : "You cannot see the tree for the living wooden walls "+
                "surrounding you.",
                "upper trunk" : "You are in the trunk of the tree.",
                "entrance"    : "The entrance is in the room below you.",
                "canopy"      : "Above you are leafy green branches. Go up into them.",
                "room"        :  (: this_object(), "print_long" :),
                "trunk"       : "You are in the trunk of the tree.",
                "tree"        : "You cannot see the tree for the living wooden walls "+
                "surrounding you.",
                "branches"    : "Above you are leafy green branches. Go up into them.",
                "walls"       : "This room has walls. Fancy that. They're made of wood, "+
                "which makes sense since you are inside of a tree.",
                "stairway"    : "The wooden stairway creaks and groans as you put your weight "+
                "on it."]) );
    SetExits( (["down" : "/domains/Praxis/mage_hall",
                "up"   : "/domains/Praxis/branches"]) );
}

void print_long() {
    write (this_object()->GetLong());
}
void init(){
    ::init();
}
