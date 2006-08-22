inherit "/std/board/bboard";

void create()
{
        bboard::create();
	set_attic_dir ("/domains/Fooland/data/attic") ;
	set_save_dir("/domains/Fooland/data") ;
	set_save_file("hall");
	set_location("/domains/Fooland/hall") ;
	set ("short.text", "General purpose bulletin board") ;
}
