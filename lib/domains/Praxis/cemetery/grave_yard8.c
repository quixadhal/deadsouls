inherit "/lib/std/room";

void create() {
    ::create();
    SetProperty("light", 1);
    SetShort( "Shadowwolf's grave");
    SetLong(
      "You are in the northern reaches of the Praxis cemetery.  Old, "
      "long-abandoned graves spread out around you.  An eerie silence "
      "has settled over the area, as if it's awaiting something horrible. "
      "You see a grave here, with an antique headstone.");
    SetItems(
      (["cemetery" : "All about you stretch the silent graves.  "
        "You feel like you are being watched.",
        "grave" : "The grave is old, and grass-covered.",
        "writing" : "The engraving says: Here lies the mysterious "
        "Shadowwolf.  He was decisively executed by mudders when "
        "it was discovered he hung out with Knights who say NI.",
        "headstone" : "The headstone is of grey marble.  You might "
        "be able to read the writing on it."]) );
    SetSkyDomain("town");
    SetExits( 
      (["southeast" : "/domains/Praxis/cemetery/grave_yard6",
        "south" : "/domains/Praxis/cemetery/grave_yard2"]) );
}
void reset() {
    ::reset();
    if (!present("ghost"))
        new("/domains/Praxis/cemetery/mon/ghost8")->move(this_object());
}

void init() {
    ::init();
    add_action("read","read");
}

int read(string str) {
    if(str == "writing" || str == "headstone") {
        write (::GetLong("writing"));
        return 1;
    }
    notify_fail("Read what?\n");
    return 0;
}
