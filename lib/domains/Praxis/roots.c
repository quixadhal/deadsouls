inherit "/lib/std/room";

void create() {
    object pedestal, box;
    object for_test_only;

    ::create();
    SetShort("among the roots of the tree");
    SetLong(
            "Here, among the huge, stalactite-like roots of the fabled mages' tree, a "+
            "large room has been excavated. All moisture is magically funneled to the "+
            "roots, so the air here is very dry. Apart from the tree itself (and the "+
        "occasional mage), no life dares intrude upon the silence of this place. "+
            "In the center of this room, a small pedestral juts up from the ground, while, "+
            "Close to the north wall, a donations box reminds you of your financial "+
            "responsibility to your class. A small stairway to the east leads up into "+
            "the heart of the mage tree. ");
    SetProperties( (["light"     : 2, 
                "indoors"   : 1 ,
                "no castle" : 1]) );
    SetItems( ([
                "room"       :  (: this_object(), "print_long" :),
                "tree"       : "You are among the roots of the tree.",
                "north wall" : "Near the north wall of the tree there's a donation box",
                "wall"       : "Near the north wall of the tree there's a donation box",
                "roots"      : "These enormous roots supply nourishment for the immense mage "+
                "tree. According to legend, the roots reach the very core of "+
                "the world, effectively making the mage tree as permanent as "+
                "the Earth. ",
                "stairway"   : "The stairway has been carved out of the roots.  It seems "+
                "unsturdy, but as soon as you step on it you are reassured, "+
                "for the stairway partakes of the infinite stability of the "+
                "tree itself. "]) );
    SetExits( (["up" : "/domains/Praxis/mage_hall"]) );
    //box = new("/realms/nialson/mages/donation");
    //box->move(this_object());
    pedestal = new("/domains/Praxis/obj/misc/pedestal");
    pedestal->move(this_object());
#if 0
    for_test_only = new("/domains/Praxis/obj/magic/ball");
    for_test_only->move(this_object());
#endif
}

void print_long() {
    write (this_object()->GetLong());
}
void init(){
    ::init();
}
