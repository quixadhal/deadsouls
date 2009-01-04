#include "../lpc_incl.h"
#include "../comm.h"
#include "../file_incl.h"
#include "../file.h"
#include "../backend.h"
#include "../compiler.h"
#include "../main.h"
#include "../eoperators.h"
#include "../efuns_main.h"
#include "../efun_protos.h"
#include "../simul_efun.h"
#include "../add_action.h"
#include "../array.h"
#include "../master.h"
#include "../port.h"
#include "../array.h"
#include "spec.h"

//Author Cratylus
#ifdef F_QUERY_CHARMODE
static int query_charmode (object_t * ob){
    int ret;
    if (!ob || ob->interactive == 0){
        ret = -2;
    }
    else {
        ret = (ob->interactive->iflags & 0x4);
    }
    return ret;
}

void f_query_charmode (void){
    int tmp;

    if (st_num_arg){
        tmp = query_charmode(sp->u.ob);
        free_object(&sp->u.ob, "f_query_charmode");
    } 
    else {
        tmp = -1;
    }
    put_number(tmp);
}
#endif

//Author Cratylus
#ifdef F_REMOVE_GET_CHAR
static int remove_get_char (object_t * ob){
    int ret;
    if (!ob || ob->interactive == 0){
        ret = -2;
    }
    else ret = 0;

        if (ob->interactive->input_to) {
            ret = 1;
            free_sentence(ob->interactive->input_to);
            if (ob->interactive->num_carry > 0)
                free_some_svalues(ob->interactive->carryover, ob->interactive->num_carry);
            ob->interactive->carryover = NULL;
            ob->interactive->num_carry = 0;
            ob->interactive->input_to = 0;
        }
    else {
        ret = -1;
    }
    return ret;
}

void f_remove_get_char (void){
    int tmp;

    if (st_num_arg){
        tmp = remove_get_char(sp->u.ob);
        free_object(&sp->u.ob, "f_remove_get_char");
    } 
    else {
        tmp = -3;
    }
    put_number(tmp);
}

#endif

//Author Raudhrskal
#ifdef F_SEND_NULLBYTE
void f_send_nullbyte (void){
    int tmp;
    object_t *who;
    tmp = 0;

    if (st_num_arg){
	who = sp->u.ob;
	if (!who || (who->flags & O_DESTRUCTED) || !who->interactive ||
	    (who->interactive->iflags & (NET_DEAD | CLOSING))) {
	  tmp = -2;
	}
	else {
	  tmp = 1;
	  //""is only the end-of-string zero byte.
	  add_message(who,"",1);
	  flush_message(who->interactive);
	}
        free_object(&sp->u.ob, "f_send_nullbyte");
    }
    else {
        tmp = -1;
    }
    put_number(tmp);
}
#endif
