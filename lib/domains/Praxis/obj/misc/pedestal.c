inherit "/lib/std/item";

int has_ball;

void init() {
    ::init();
    add_action("place", "place");
}

void clean_up() { return; }

void create() {
    ::create();
    has_ball=0;
    SetKeyName("pedestal");
    SetId( ({ "pedestal" }) );
    SetShort("a pedestal");
    SetLong(
            "This small (gnome-height) pedestal is made from a dark, mysterious stone. "+
            "There is a small indentation on the flat top, as if a round object were "+
            "meant to be placed there. ");
    SetRead("Made by the Wilsonshire and Sons corintian pillar specialists.");
    SetMass(10000);
    SetValue(0);
}

int place(string str)
{
    string this;
    object ths;

    if (!stringp(str) || sscanf(str,"%s on pedestal", this) != 1) return 0;
    if (has_ball) {
        if (!(ths=present(this,this_player()))) return 0;
        if (!ths->id("crystal ball")) return 0;
        write("The pedestal already has a crystal ball on it. ");
        return 1;}
    if (!(ths=present(this,this_player()))) {
        notify_fail("You are not carrying "+this+". ");
        return 0;}
    if (!ths->id("crystal ball")) {
        notify_fail("That's pointless. ");
        return 0;}
    ths->remove();
    write("You place the crystal ball on the pedestal, and watch it fuse into place. ");
    say("Places a crystal ball on the pedestal. The ball is fused to the pedestal");
    SetShort("a pedestal with crystal ball.");
    SetLong(
            "This small (gnome-height) pedestral is made from a dark, mysterious "+
            "stone.  A crystal ball snugly fits into an indentation on the top. As "+
            "you peer at it, you notice barely-perceptible lines of force streaming "+
            "from the stone to the crystal ball. It seems your lessons in discerning "+
            "magic forces have paid off. ");
    add_action("view", "view");
    has_ball=1;
    return 1;
}

int view(string str)
{
    int i;
    string desc;
    object ob, env, *inv;

    write("You gaze into the crystal ball. ");
    if(this_player()->query_mp() < 5) {
        write("Your magic powers are just too weak to help you. ");
        return 1;
    }
    if(!(ob = find_living(str))
#if 0
            || creatorp(ob)
#endif
      ) {
        write("No such person in our reality. ");
        return 1;
    }
    this_player()->add_mp(-5);
    if(!(env = environment(ob))) {
        write(ob->query_cap_name()+" is lost. ");
        return 1;
    }
    write("You find "+ob->query_cap_name()+" in the crystal ball: "); 
    ob=environment(ob);
    desc = ob->GetLong()+"\n";
    inv = all_inventory(ob);
    for(i=0; i<sizeof(inv); i++) {
        if(inv[i]->query_invis()) continue;
        desc += "\n"+inv[i]->GetShort()+" "; }
    write(desc);
    return 1;
}
