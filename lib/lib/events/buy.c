/*    /lib/events/buy.c
 *    From the Dead Souls V Object Library
 *    Things that get bought from
 *    Created by Descartes of Borg 970103
 *    Version: @(#) buy.c 1.1@(#)
 *    Last modified: 97/01/03
 */

mixed CanSell(object who, string what) {
    return 1;
}

int direct_buy_str_from_liv(string str) {
    return CanSell(this_player(), remove_article(lower_case(str)));
}
