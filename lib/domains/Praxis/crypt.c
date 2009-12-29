#include <lib.h>

#define PH "/realms/drakken/clerics/"
#define SAVE_FILE "/domains/Praxis/data/cleric_crypt"
#define WHAMMER "/realms/zaknaifen/weap/whammer"
#define PHYL "/realms/zaknaifen/armor/phylactery"

inherit LIB_ROOM;

int prayers,is_hammer,is_phyl;

void create() {
    ::create();
    prayers = 0;
    is_hammer = 0;
    is_phyl = 0;
    RestoreObject(SAVE_FILE);	// restore is_hammer and is_phyl
    SetProperties(([
                "no attack": 1,
                "no steal":1,
                "no castle":1,
                "holy ground":1,
                "light"   : 2,
                "indoors" : 1,
                ]));
    SetShort("a small crypt");
    SetLong("The altar room is carved from the ancient gray stone that Praxis "
            "rests upon. Burning candles along the walls cause the shadows to "
            "dance around the room in a chaotic, yet enchanting dance. A large altar "
            "rises from the floor in the center of the room and steps carved into the "
            "stone lead upwards. ");
    SetExits( ([
                "up" : "/domains/Praxis/cleric_hall"
                ]) );
    SetItems(([
                "altar":(: this_object(), "exaaltar" :),
                "scenes" : "You see Tempos wielding his mighty %^CYAN%^warhammer%^RESET%^ in a variety of battles "
                "you recognize from your religious studies. ",
                ({"stones","stone"}):
                "The cold grey stone is creased and marked by the tools that were used "
                "to carve this room beaneath the earth. ",
                ({"shadows","shadow"}):
                "The shadows dance enchantingly across the room in time to the music "
                "of the flickering candle flames. ",
                "steps" :  "The steps are carved into the grey stone and are worn by the passage "
                "of many feet over the ages. ",
                ({"walls","wall"}):
                "The walls are hewn roughly from the stone beneath Praxis and are lined "
                "with many flickering candles. ",
                ({"candle","candles"}):
                "The candles rest in iron stands set along the wall and are made from "
                "animal fat. The candle flames flicker to the tune of the wind, creating "
                "the music by which the shadows cavort and dance. ",
                "stands" : "The stands are made of %^BLACK%^%^BOLD%^black%^RESET%^ iron and are of plain construction. "
                "Each stand holds between three to four lit candles. ",
                    ]));
}

void exaaltar() {
    string str;

    str = "The altar is made of ancient gray stone and is huge.  There are scenes "
        "of Tempos fighting mythical beasts carved all along the sides of the altar. "
        "The top of the altar is flat and has the image of a hammer traced along "
        "it in %^YELLOW%^gold%^RESET%^. ";
    if (is_hammer) str += "The %^CYAN%^warhammer%^RESET%^ lieing upon it perfectly fits into the traces. ";
    if (is_phyl) str += "A %^YELLOW%^golden%^RESET%^ phylactery partially covers the traces.";
    write(str);
}

void reset()
{
    ::reset();
    if (!present("box")) {
        //new(PH "obj/donation")->move(this_object());
    }
    prayers = 0;
}

void init()
{
    ::init();
    add_action("pray_func","pray");
}

int pray_func(string str) {
    object ob, ob2, *inv;
    string limb;
    int i,j;

    ob = this_player();
    inv = all_inventory(ob);
    for (i=0; i<sizeof(inv); i++) {
        if ((!str || strlen(str) == 0) && inv[i]->id("hammer of tempos") && is_hammer == 0) {
            tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                    nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that "+ob->query_cap_name()+" carries his %^CYAN%^warhammer%^RESET%^ and takes it away! "
                    "As the shadow disappears the %^CYAN%^warhammer%^RESET%^ appears on the altar.",ob);
            tell_object(ob,"You mutter a prayer in front of the altar. As you "
                    "do so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and repeat the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that you carry his %^CYAN%^warhammer%^RESET%^ and takes it away! "
                    "As the shadow disappears the %^CYAN%^warhammer%^RESET%^ appears on the altar.");
            is_hammer = 1;
            SaveObject(SAVE_FILE);	// save is_hammer
            inv[i]->destruct();
            return 1;
        } else if ((!str || strlen(str) == 0) && inv[i]->id("hammer of tempos") && is_hammer == 1) {
            tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                    nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that "+ob->query_cap_name()+" carries his %^CYAN%^warhammer%^RESET%^ and takes it away!",ob);
            tell_object(ob,"You mutter a prayer in front of the altar. As you "
                    "do so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and repeat the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that you carry his %^CYAN%^warhammer%^RESET%^ and takes it away!");
            inv[i]->destruct();
            return 1;
        }
        if ((!str || strlen(str) == 0) && inv[i]->id("phylactery") && is_phyl == 0) {
            tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                    nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that "+ob->query_cap_name()+" carries his phylactery and takes it away! "
                    "As the shadow disappears the phylactery appears on the altar.",ob);
            tell_object(ob,"You mutter a prayer in front of the altar. As you "
                    "do so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and repeat the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that you carry his phylactery and takes it away! "
                    "As the shadow disappears the phylactery appears on the altar.");
            is_phyl = 1;
            SaveObject(SAVE_FILE);	// save is_phyl
            inv[i]->destruct();
            return 1;
        } else if ((!str || strlen(str) == 0) && inv[i]->id("phylactery") && is_phyl == 1) {
            tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                    nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that "+ob->query_cap_name()+" carries his phylactery and takes it away! ",ob);
            tell_object(ob,"You mutter a prayer in front of the altar. As you "
                    "do so, the shape of a demigod appears on the altar. "
                    "You feel the might of the shadow and repeat the prayer as you notice that "
                    "the shadow is an incarnation of Tempos himself! "
                    "Tempos notices that you carry his phylactery and takes it away!");
            inv[i]->destruct();
            return 1;
        }
    }
    if (is_hammer) {
        j = 0;
        for (i=0; i<sizeof(inv); i++)
            if (inv[i]->id("hammer of tempos")) {
                j = 1;
                break;
            }
        if (str == "for warhammer" && j == 0 && ob->query_alignment() >= 1000 && ob->query_level() > 5) {
            ob2 = new(WHAMMER);
            if (ob2) {
                tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                        nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                        "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                        "the shadow is an incarnation of Tempos himself! "
                        "Tempos notices the %^CYAN%^warhammer%^RESET%^ on top of the altar and hands it to "+ob->query_cap_name()+"!",ob);
                tell_object(ob,"You mutter a prayer in front of the altar. As you "
                        "do so, the shape of a demigod appears on the altar. "
                        "You feel the might of the shadow and repeat the prayer as you notice that "
                        "the shadow is an incarnation of Tempos himself! "
                        "Tempos notices his %^CYAN%^warhammer%^RESET%^ on top of the altar and hands it to you!");
                tell_room(this_object(),"The shadow disappears as the prayer ends.",({}));
                is_hammer = 0;
                SaveObject(SAVE_FILE);	// save is_hammer
                ob2->move(ob);
                return 1;
            }
        }
    }
    if (is_phyl) {
        j = 0;
        for (i=0; i<sizeof(inv); i++)
            if (inv[i]->id("phylactery")) {
                j = 1;
                break;
            }
        if (str == "for phylactery" && j==0 && ob->query_alignment()>=800 && ob->query_level()>5 && ob->query_skill("faith")>50) {
            ob2 = new(PHYL);
            if (ob2) {
                tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                        nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                        "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                        "the shadow is an incarnation of Tempos himself! "
                        "Tempos notices the phylactery on top of the altar and hands it to "+ob->query_cap_name()+"!",ob);
                tell_object(ob,"You mutter a prayer in front of the altar. As you "
                        "do so, the shape of a demigod appears on the altar. "
                        "You feel the might of the shadow and repeat the prayer as you notice that "
                        "the shadow is an incarnation of Tempos himself! "
                        "Tempos notices his phylactery on top of the altar and hands it to you!");
                tell_room(this_object(),"The shadow disappears as the prayer ends.",({}));
                is_phyl = 0;
                SaveObject(SAVE_FILE);	// save is_phyl
                ob2->move(ob);
                return 1;
            }
        }
    }
    if (prayers < 7) {
        tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                "You feel the might of the shadow and take part in his pleasure with the prayer as you notice that "
                "the shadow is an incarnation of Tempos himself!",ob);
        tell_object(ob,"You mutter a prayer in front of the altar. As you "
                "do so, the shape of a demigod appears on the altar. "
                "You feel the might of the shadow and repeat the prayer as you notice that "
                "the shadow is an incarnation of Tempos himself!");
        tell_room(this_object(),"The shadow disappears as the prayer ends.",({}));
        if (random(100) <(15+ob->query_base_stats("charisma"))) ob->add_alignment(random(3)+10-prayers);
    } else {
        limb = ob->return_limb();
        tell_room(this_object(),ob->query_cap_name()+" mutters a prayer in front of the altar. As "+
                nominative(ob)+" does so, the shape of a demigod appears on the altar. "
                "You feel the might of the shadow and the fear he installs in your heart! "
                "The shadow again is an incarnation of Tempos himself, but this time he is annoyed of being "
                "disturbed that often! With a swing of his hammer he attacks "+ob->query_cap_name()+
                "! "+ ( is_hammer ?
                    " This time the hammer turns out to be real!\nTempos destroys "+ob->query_cap_name()+" utterly in the "+limb+
                    " with his %^CYAN%^warhammer%^RESET%^!"
                    : "Luckily for "+objective(ob)+ " the hammer is a shadow also, and therefore the damage is minimal."
                    ) ,ob);
        tell_room(this_object(),"Tempos decides "+ob->query_cap_name()+" is not "
                "worth to bother with anyway and disappears.",ob);
        tell_object(ob,"You mutter a prayer in front of the altar. As you "
                "do so, the shape of a demigod appears on the altar. "
                "You feel the might of the shadow and the fear he installs in your heart! "
                "The shadow again is an incarnation of Tempos himself, but this time he is annoyed of being "
                "disturbed that often! With a swing of his hammer he attacks you!"
                + ( is_hammer ? 
                    " This time the hammer turns out to be real!\nTempos destroys you utterly in the "+limb+
                    " with his %^CYAN%^warhammer%^RESET%^!"
                    : " Luckily for you the hammer is a shadow also, and therefore the damage is minimal."
                  ) );
        ob->do_damage(limb,5+random(prayers)+is_hammer*25);
        tell_object(ob,"Tempos decides you are not worth to bother with anyway and disappears.");
    }
    prayers++;
    return 1;
}
