/*    /lib/events/drink.c
 *    From the Dead Souls V Object Library
 *    Handles drinking in the form of OBJ and SUBSTANCE from OBJ
 *    Created by descartes of Borg 961221
 *    Version: %A%
 *    Last modified: %D%
 */

varargs mixed CanDrink(object who, string substance) {
    return 1;
}

varargs mixed eventDrink(object who, string substance) {
    return 1;
}

mixed direct_drink_obj() {
    return CanDrink(this_player());
}

mixed direct_drink_from_obj() {
    return direct_drink_obj();
}

mixed direct_drink_str_from_obj(string str) {
    return CanDrink(this_player(), remove_article(lower_case(str)));
}
 
