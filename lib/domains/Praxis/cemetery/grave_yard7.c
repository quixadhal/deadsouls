inherit "/lib/std/room";
object coffin;

void create() {
    ::create();
    SetProperty("light", 0);
    SetShort( "The grave");
    SetLong(
            "You have foolishly climbed down into the ancient grave of one of "
            "the founders of Praxis.  You are standing on the closed lower door "
            "of a coffin.  The upper door is open and gruesomely inviting.  The "
            "soil presses in around you claustrophobicly.");
    SetItems(
            (["under" : "You can't look under the coffin, you're "
             "standing on it.",
             "soil" : "The dirt walls of the grave are soft and "
             "smell of the earth.",
             "grave" : "The grave is horribly empty and sad."]) );
    SetExits( 
            ([ "up" :	"/domains/Praxis/cemetery/grave_yard2"]) );
}
void init(){
    ::init();
} 
