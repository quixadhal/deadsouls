#include <lib.h>
#include <vendor_types.h>

inherit LIB_TELLER;


static void create() {
    ::create();
    SetKeyName("zoe");
    SetId( ({ "teller","banker","executive" }) );
    SetAdjectives( ({ "bank","executive","friendly","efficient" }) );
    SetShort("Zoe the bank teller");
    SetLevel(12);
    SetLong("Zoe is an attractive young blonde woman with "
            "a French accent. She is dressed in a conservative, "
            "executive style, and has a friendly and efficient air "
            "about her.");
    SetGender("female");
    SetMorality(40);
    SetRace("human");
    AddCurrency("silver", random(100));
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetBankName("First Village Bank");
    SetLocalCurrency("silver");
    SetLocalFee(1);
    SetOpenFee(5);
    SetExchangeFee(2);
    SetCurrencies( ({ "copper", "silver", "electrum", "gold", "platinum" }) );


}
void init(){
    ::init();
}
