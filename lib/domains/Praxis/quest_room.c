inherit "/lib/std/room";

#define POINTS_NEEDED   100
#define QUEST_DIR	"/domains/Praxis/quests/"

void init() {
    ::init();
    if(!archp(this_player())) this_player()->destruct();
}

void create() {
    string *quest_ls;
    int i;
    ::create();
    SetProperty("light", 1);
    SetShort( "Quest room");
    SetLong( "Quests are stored here.");
    quest_ls = get_dir(QUEST_DIR);
    if(!sizeof(quest_ls)) return;
    for(i=0; i<sizeof(quest_ls); i++) {
        new(QUEST_DIR+quest_ls[i])->move(this_object());
    }
}

void list_quests(object tp, int x) {
    object *inv;
    int i;
    string *quest_list;

    inv = all_inventory(this_object());
    if(!sizeof(inv)) {
        tell_object(tp, "There are no quests.");
        return;
    }
    quest_list = ({});
    for(i=0; i<sizeof(inv); i++) {
        if(member_array(inv[i]->query_name(), tp->query_quests()) == -1) quest_list += ({ inv[i] });
    }
    if(!x) {
        tell_object(tp, "There are "+sizeof(quest_list)+" quests you "
                "have not solved. They are these quests:\n");
        for(i=0; i<sizeof(quest_list); i++) {
            tell_object(tp, (i+1)+": "+present(quest_list[i], this_object())->GetShort()+"\n");
        }
    }
    else {
        if(x > sizeof(quest_list)) {
            tell_object(tp, "No such quest.\n");
            return;
        }
        tell_object(tp, present(quest_list[x-1], this_object())->GetLong());
    }
}

int query_quest_points(string str) {
    int x;
    object ob;

    ob = present(str);
    x = ob->query_quest_points();
    return x;
}

int check_quests(object tp, int lev) {
    int qp;
    string *list;

    qp= tp->query_quest_points();
    if(lev>=5) if(qp < POINTS_NEEDED/4) return 0;
    else if(lev >= 10) if(qp < POINTS_NEEDED/2) return 0;
    else if(lev >= 15) if(qp < (3*POINTS_NEEDED)/4) return 0;
    else if(lev > 19) if(qp < POINTS_NEEDED) return 0;
    return 1;
}

void clean_up() { return; }


