/*    /secure/sefun/value.c
 *    from the Dead Souls LPC Library
 *    data conversion simuls
 *    created by Descartes of Borg 950509
 */

#include "sefun.h"

mixed convert_value(string val) {
    if( !val || val == "" ) return 0;
    switch( val[0] ) {
        case '(':
	    if( strlen(val) == 1 ) error("Badly formed value: " + val + "\n");
	    switch( val[1] ) {
