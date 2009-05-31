/*    /lib/obj/dummy.c
 *    From the Dead Souls Mud Library
 *    A dummy item object that handles item descriptions and such
 *    Created by Descartes of Borg 961014
 *    Version: @(#) dummy.c 1.18@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>

inherit LIB_BASE_DUMMY;;
inherit LIB_SHADOW_HOOK;

/* ******************* dummy.c driver applies ******************** */
varargs static void create(string array id, mixed long, string array adj){
    base_dummy::create(id, long, adj);
}
