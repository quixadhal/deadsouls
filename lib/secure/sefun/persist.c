/*    /secure/sefun/persist.c
 *    from the Dead Souls LPC Library
 *    handles efuns for persistance support
 *    created by Beek@Dead Souls 950529
 */

#include <daemons.h>

object unique(string fn, int rare) {
    return (object)UNIQUE_D->GetUniqueCopy(fn, rare);
}

int query_reset_number() { return (int)master()->GetResetNumber(); }
