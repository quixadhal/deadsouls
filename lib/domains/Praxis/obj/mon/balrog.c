inherit "/lib/npc";

create() {
    ::create();
    SetKeyName("balrog");
    SetId( ({ "balrog" }) );
    SetShort("A wicked balrog");
    SetAggressive( 17);
    SetLevel(9);
    SetLong("He is one of the guardians of the Daroq Mountains.\n");
    SetRace("human");
    SetMorality(-200);
    SetRace( "balrog");
    SetHealthPoints(360);
    SetMagicPoints(200);
}
