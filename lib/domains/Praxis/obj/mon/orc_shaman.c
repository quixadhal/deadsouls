inherit "/lib/npc";

create() {
    ::create();
    SetKeyName("shaman");
    SetId( ({ "shaman", "orc", "orc shaman" }) );
    SetShort("The high orc shaman");
    SetAggressive( 29);
    SetLevel(13);
    SetLong("She is a powerful dark mage and the leader of the orcs of Orc Valley.\n");
    SetMorality(-515);
    SetGender("female");
    SetClass("mage");
    SetRace( "orc");
    SetHealthPoints(220);
    SetMagicPoints(350);
    SetRace("human");
    SetSkill("magic attack", 60);
    SetProperty("no bump", 1);
}
