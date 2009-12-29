#include <lib.h>
#include <daemons.h>
inherit LIB_ROOM;

object ob;
mapping Levels = PLAYERS_D->GetLevelList();
int ReadSign();
int ReadScroll();

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Adventurers' Guild");
    SetLong("This small building is where adventurers can record their adventures and leave messages and announcements for other brave souls. This is also the place one can seek advancement, so if you deserve to be a higher level adventurer, asking the guild master for it will make it so. A scroll lists quests one may perform, and a sign contains some instructions. The Creator's Hall is south of here. A special skills training area is upstairs. A conference room is east, where folks can discuss things in privacy.");
    SetItems( ([
                ({ "list","scroll"}) : "A scroll hangs from the wall, listing "
                "some adventures you can perform which may have rewards "
                "for you.",
                ({"sign","instructions"}) : "This sign contains some "
                "general instructions on how to gain experience and "
                "attain advancement.",
                ({"building","small building","here"}) : "You are in the "
                "Adventurers' Guild.",
                ]) );
    SetExits( ([
                "north" : "/domains/town/room/vill_road2",
                "south" : "/domains/default/room/builder_hall",
                "east" : "/domains/town/room/confroom",
                "up" : "/domains/town/room/training.c",
                ]) );
    SetInventory(([
                "/domains/town/obj/bin" : 1,
                "/domains/town/obj/table" : 1,
                "/domains/town/npc/dirk" : ({60, 1})
                ]));
    SetRead( ([
                ({"list","scroll"}) : (: ReadScroll :),
                ({"instructions","sign"}) : (: ReadSign :)
                ]) );

    SetProperty("no attack", 1);
    ob = new(LIB_BOARD);
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );    
    ob->set_board_id("adv_guild_board");
    ob->set_max_posts(30);
    ob->SetShort("a dusty chalkboard");
    ob->eventMove(this_object());
    SetNoClean(1);
}

mixed ReadSign(){
    int i;
    string ret = read_file("/domains/town/txt/advancement.txt");
    ret += "\n";
    for(i=1;i<21;i++){
        ret +=  sprintf("%:-3s     %:-28s %:-12s %:16s\n", i+"",
                Levels[i]["title"], Levels[i]["xp"]+"", (Levels[i]["qp"] || "none")+"");
    }
    ret += "\nTo advance, ask the guildmaster.\nExample:\n\n";
    ret += "ask dirk to advance";
    return this_player()->eventPage(({ret}));
}

mixed ReadScroll(){
    return this_player()->eventPage("/domains/town/txt/quests.txt");
}

void init(){
    ::init();
}
