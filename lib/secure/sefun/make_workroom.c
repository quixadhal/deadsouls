#include <daemons.h>

private string nom, cdir;

varargs int make_workroom(mixed dude, int cre) {
    string tdir, dir_line, bakdata;
    int creator;

    if(!dude) return 0;

    if(objectp(dude)){
        cdir = homedir(dude, cre);
        creator = (cre || creatorp(dude));
        if(sizeof(get_dir(REALMS_DIRS+"/")) ==1) creator = 1;
        nom = dude->GetKeyName();
    }

    if(stringp(dude)){
        dude = lower_case(dude);
        if(cre ||  member_array(dude, PLAYERS_D->GetCreatorList()) != -1){
            creator = 1;
            cdir = REALMS_DIRS + "/" + dude;
        }
        else {
            cdir = ESTATES_DIRS + "/" + dude[0..0] + "/" + dude;
        }
        nom = dude;
    } 

    if(unguarded( (: file_size("/realms/template/") :) ) == -1) return 4;

    if(unguarded( (: file_size(cdir+"/area") :) ) == -1){
        tdir = "/realms/template/";
        dir_line = "#define MY_DIR          \""+cdir+"\"";
        bakdata = "workroom.orig : "+cdir+"/workroom.c\n";
        mkdir(cdir);
        mkdir(cdir+"/log");
        mkdir(cdir+"/log/archive");
        mkdir(cdir+"/bak");
        mkdir(cdir+"/tmp");
        mkdir(cdir+"/area");
        mkdir(cdir+"/adm");
        mkdir(cdir+"/area/room");
        mkdir(cdir+"/area/save");
        mkdir(cdir+"/area/weap");
        mkdir(cdir+"/area/obj");
        mkdir(cdir+"/area/npc");
        mkdir(cdir+"/area/armor");
        mkdir(cdir+"/area/etc");
        mkdir(cdir+"/area/doors");
        mkdir(cdir+"/area/meals");

        if(creator){
            mkdir(cdir+"/cmds");
            mkdir(cdir+"/public_html");
            cp(tdir+"plan", cdir+"/.plan");
            cp(tdir+"evaldefs.h", cdir+"/evaldefs.h");
            cp(tdir+"profile", cdir+"/.profile");
            cp(tdir+"cmds/custom.c", cdir+"/cmds/custom.c");
            cp(tdir+"workroom.c", cdir+"/workroom.c");
            cp(tdir+"workroom.c", cdir+"/workroom.bak");
            cp(tdir+"workroom.c", cdir+"/bak/workroom.orig");
            cp(tdir+"area/obj/chest.c", cdir+"/area/obj/chest.c");
        }

        else {
            cp(tdir+"workroom_builder.c", cdir+"/workroom.c");
            cp(tdir+"workroom_builder.c", cdir+"/workroom.bak");
            cp(tdir+"workroom_builder.c", cdir+"/bak/workroom.orig");
            cp(tdir+"area/obj/builder_chest.c", cdir+"/area/obj/builder_chest.c");
        }

        cp(tdir+"adm/remote.c",cdir+"/adm/remote.c");
        write_file(cdir+"/bak/bk.db",bakdata);
        cp(tdir+"area/customdefs.part1", cdir+"/area/customdefs.h");
        write_file(cdir+"/area/customdefs.h","\n"+dir_line+"\n");
        write_file(cdir+"/area/customdefs.h",read_file(tdir+"area/customdefs.part2"));
        cp(tdir+"area/room/sample_room.c", cdir+"/area/room/sample_room.c");
        cp(tdir+"area/room/sample_two.c", cdir+"/area/room/sample_two.c");
        cp(tdir+"area/weap/sword.c", cdir+"/area/weap/sword.c");
        cp(tdir+"area/obj/table.c", cdir+"/area/obj/table.c");
        cp(tdir+"area/obj/key.c", cdir+"/area/obj/key.c");
        cp(tdir+"area/obj/case.c", cdir+"/area/obj/case.c");
        cp(tdir+"area/obj/cup.c", cdir+"/area/obj/cup.c");
        cp(tdir+"area/obj/pitcher.c", cdir+"/area/obj/pitcher.c");
        cp(tdir+"area/obj/watch.c", cdir+"/area/obj/watch.c");
        cp(tdir+"area/npc/fighter.c", cdir+"/area/npc/fighter.c");
        cp(tdir+"area/armor/chainmail.c", cdir+"/area/armor/chainmail.c");
        cp(tdir+"area/armor/leather_boot_r.c", cdir+"/area/armor/leather_boot_r.c");
        cp(tdir+"area/armor/leather_boot_l.c", cdir+"/area/armor/leather_boot_l.c");
        return 1;
    }

    else {
        //write("That person already has a homedir.");
        return 5;
    }
}
