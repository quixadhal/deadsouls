/*    /domains/Praxis/order.c
 *    from Nightmare IV
 *    a work order for a high mortal room
 *    created by Descartes of Borg 940701
 */

#include <lib.h>
#include <dirs.h>
//#include <security.h>

inherit LIB_ITEM;

int __Light, __Indoors;
string __Short, __Long, __Exit, __NewRoom;

static private string create_file();

void create() {
    ::create();
    SetKeyName("work order");
    SetId( ({ "order", "work order" }) );
    SetShort("a work order");
    SetLong("An order for workers to begin modifying your estate "
            "to your desires.  In order to modify your estate, go to "
            "any room in it, and type \"build <room name>\".  "
            "For example, to build your study, type: "
            "\"build the study\".  Room's as you type in the command "
            "are what people see when they enter a room in brief mode.  "
            "In this case, people would see \"The study\".  You will then be "
            "asked some questions about your room."
           );
    SetMass(10);
    SetValue(99);
    true();
}

void init() {
    ::init();
    add_action("cmd_build", "build");
}

static int cmd_build(string str) {
    if(!str) return notify_fail("Build which room?\n");
    if(file_size(ESTATES_DIRS+"/"+geteuid(this_player())) != -2)
        return notify_fail("You need an estates directory!\n");
    /*
       if(!high_mortalp(this_player())) 
       return notify_fail("Only high mortals may build!\n");
     */
    if(strsrch(file_name(environment(this_player())), 
                ESTATES_DIRS+"/"+geteuid(this_player())) != 0)
        return notify_fail("You can only build on your stuff!\n");
    __Short = str;
    message("prompt", "Is the room 0) outdoors, or 1) indoors? ", this_player());
    input_to("input_indoors");
    return 1;
}

static void input_indoors(string str) {
    int x;

    x = to_int(str);
    if(x && x != 1) {
        message("system", "That was not a valid value.", this_player());
        return;
    }
    __Indoors = x;
    message("system", "How well lit is the room?", this_player());
    message("system", "    0 is darkest, 3 brightest", this_player());
    message("prompt", "\nEnter light value: ", this_player());
    input_to("input_light");
}

static void input_light(string str) {
    int x;

    x = to_int(str);
    if(x && x != 1 && x != 2 && x != 3) {
        message("system", "Invalid light number.", this_player());
        return;
    }
    __Light = x;
    message("system", "Enter in a long description for the room.  "
            "A long description is what people see when they enter a "
            "room in verbose mode.  A line showing obvious exits is automatically "
            "appended.  In addition, if you add any smells or sounds, the "
            "default ones are automatically appended.  So do not describe sounds or "
            "smells here.", this_player());
    message("system", "Enter in the long description like mail.", this_player());
    rm(DIR_TMP+"/"+geteuid(this_player())+".estate");
    this_player()->edit(DIR_TMP+"/"+geteuid(this_player())+".estate",
            "done_edit", this_object());
}

void abort() {
    rm(DIR_TMP+"/"+geteuid(this_player())+".estate");
    message("system", "Building aborted.", this_player());
}

void done_edit(mixed *unused) {
    string str;

    if(!(str = read_file(DIR_TMP+"/"+geteuid(this_player())+".estate"))) {
        message("system", "No long!", this_player());
        return;
    }
    __Long = replace_string(str, "\n", " ");
    message("prompt", "\nIn which direction do you wish to build "+__Short+"? ",
            this_player());
    input_to("input_exit");
}

static void input_exit(string str) {
    mapping valid_exits;
    string file;

    if(member_array(str, environment(this_player())->query_exits())
            != -1) {
        message("system", "A room already exists in that direction!",
                this_player());
        return;
    }
    valid_exits = ([ "north": "south", "south": "north", "east":"west",
            "west":"east", "up":"down", "down":"up", "southeast":"northwest",
            "northwest":"southeast", "southwest":"northeast",
            "northeast":"southwest" ]);
    if(!valid_exits[str]) {
        message("system", "That is not a real direction!", this_player());
        return;
    }
    __Exit = "$"+(file=file_name(environment(this_player())))+";$"+valid_exits[str];
    __NewRoom= create_file();
    //seteuid(UID_ESTATES);
    write_file(file, "AddExit: $"+__NewRoom+";$"+str+"\n");
    write_file(__NewRoom, "SetProperty: $indoors;#"+__Indoors+"\n");
    write_file(__NewRoom, "SetProperty: $light;#"+__Light+"\n");
    write_file(__NewRoom, "SetShort: $"+__Short+"\n");
    write_file(__NewRoom, "SetLong: $"+__Long+"\n");
    write_file(__NewRoom, "AddExit: "+__Exit+"\n");
    //seteuid(getuid());
    message("system", "Room begun.  You will see your addition "
            "appear later when the work is complete.", this_player());
    this_object()->remove();
}

static private string create_file() {
    string dir, str;
    int x;

    x = sizeof(get_dir((dir = ESTATES_DIRS+"/"+geteuid(this_player())+"/")));
    while(file_exists(str = dir+"room"+x)) x++;
    return str;
}
