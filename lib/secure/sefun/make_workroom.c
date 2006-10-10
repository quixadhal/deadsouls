string nom;

int make_workroom(mixed dude) {
    string cdir, tdir, dir_line, bakdata;

    if(!dude) return 0;

    if(objectp(dude) && !nom = dude->GetKeyName()) return 3;
    if(stringp(dude) && !nom = lower_case(dude)) return 2;

    if(unguarded( (: file_size("/realms/template/") :) ) == -1) return 4;

    if(unguarded( (: file_size("/realms/"+nom) :) ) == -1){
	cdir = "/realms/"+nom;
	tdir = "/realms/template/";
	dir_line = "#define MY_DIR          \"/realms/"+nom+"\"";
        bakdata = "workroom.orig : /realms/"+nom+"/workroom.c\n";
	mkdir(cdir);
	mkdir(cdir+"/cmds");
	mkdir(cdir+"/log");
	mkdir(cdir+"/bak");
	mkdir(cdir+"/tmp");
	mkdir(cdir+"/area");
	mkdir(cdir+"/area/room");
	mkdir(cdir+"/area/weap");
	mkdir(cdir+"/area/obj");
	mkdir(cdir+"/area/npc");
	mkdir(cdir+"/area/armor");
	mkdir(cdir+"/area/etc");
	mkdir(cdir+"/area/doors");
	mkdir(cdir+"/area/meals");
	cp(tdir+"workroom.c", cdir+"/workroom.c");
	cp(tdir+"workroom.bak", cdir+"/workroom.bak");
	cp(tdir+"bak/workroom.orig", cdir+"/bak/workroom.orig");
	//cp(tdir+"bak/bk.db", cdir+"/bak/bk.db");
        write_file(cdir+"/bak/bk.db",bakdata);
	cp(tdir+"plan", cdir+"/.plan");
	cp(tdir+"profile", cdir+"/.profile");
	cp(tdir+"cmds/custom.c", cdir+"/cmds/custom.c");
	cp(tdir+"customdefs.part1", cdir+"/customdefs.h");
	write_file(cdir+"/customdefs.h","\n"+dir_line+"\n");
	write_file(cdir+"/customdefs.h",read_file(tdir+"customdefs.part2"));
	cp(tdir+"area/room/sample_room.c", cdir+"/area/room/sample_room.c");
	cp(tdir+"area/room/sample_two.c", cdir+"/area/room/sample_two.c");
	cp(tdir+"area/weap/sword.c", cdir+"/area/weap/sword.c");
	cp(tdir+"area/obj/table.c", cdir+"/area/obj/table.c");
	cp(tdir+"area/obj/key.c", cdir+"/area/obj/key.c");
	cp(tdir+"area/obj/case.c", cdir+"/area/obj/case.c");
	cp(tdir+"area/obj/cup.c", cdir+"/area/obj/cup.c");
	cp(tdir+"area/obj/chest.c", cdir+"/area/obj/chest.c");
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
