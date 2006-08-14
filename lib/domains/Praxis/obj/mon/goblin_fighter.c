inherit "/lib/npc";

create() {
    ::create();
    SetKeyName("goblin");
    SetId( ({ "goblin", "soldier" }));
    SetShort( "a goblin soldier");
    SetAggressive( 17);
    SetLevel(5);
    SetLong( "A warrior of the Daroq goblins.");
    SetClass("fighter");
    SetRace("human");
    SetMorality(-200);
    SetRace( "goblin");
    SetHealthPoints(300);
    SetMagicPoints(200);
}
