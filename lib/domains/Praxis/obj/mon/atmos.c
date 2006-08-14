inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("atmos");
    SetId( ({ "atmos" }) );
    SetShort("High mortal Atmos, Lord of Praxis");
    SetAggressive( 0);
    SetLevel(26);
    SetLong("Atmos is the lord of Praxis and all high mortals.\n");
    SetMorality(1000);
    SetRace( "human");
    SetMaxHealthPoints(800000);
    SetHealthPoints(400000);
    SetRace("human");
}
