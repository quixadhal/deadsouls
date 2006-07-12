#include <lib.h>
#include <privs.h>
#include <cfg.h>
#include <save.h>
#include <rooms.h>
#include <config.h>
#include <daemons.h>
#include <objects.h>
#include <commands.h>
#include <armor_types.h>
#include <damage_types.h>
#include <position.h>
inherit LIB_ITEM;
mixed eval() { return new("/domains/town/obj/ebutton1")->eventMove(environment(this_player())); }
