
inherit "std/vault";

void create() {
    ::create();
    set_property("light", -1);
    set_property("indoors", 1);
    set_short( "A disgusting sewer");
    set_long(
      "These sewers under Praxis are filled with slime. "
    );
    set_items(
      (["sewers" : "The disgusting tunnels of human waste under Praxis.",
	"slime" : "Nasty, nasty.",
	({"wall", "east wall"}) : "A disgusting slime-coated wall.",
	"ceiling" : "A disgusting slime-coated ceiling.",
      ]));
    set_exits(
      (["west" : "/domains/Praxis/sewer_ent", 
      ]));
}
