/*    /secure/sefun/morality.c
 *    from the Dead Souls Mud Library
 *    handles moral actions
 *    created by Descartes of Borg 951016
 */

int moral_act(object agent, object target, int amount) {
    int am, tm;

    am = agent->GetMorality();
    if( target ) {
        tm = target->GetMorality();
    }
    else {
        tm = 0;
    }
    if( am < 200 && am > -200 ) {
        if( amount >= 200 ) {
            if( tm >= 200 ) amount = amount + tm;
            else if( tm <= -200 ) amount = 0;
        }
        else if( amount <= -200 ) {
            if( tm >= 200 ) amount *= tm;
            if( tm <= -200 ) amount = 0;
        }
    }
    else if( am >= 200 ) {
        if( amount >= 200 ) {
            if( tm >= 200 ) amount *=tm;
            else if( tm <= -200 ) amount = 0;
        }
        else if( amount <= -200 && tm >=200 ) amount *= tm;
    }
    else if( amount > 1000 ) amount = 75 + random(25);
    else if( amount > 100 ) amount = amount /10;
    else if( amount > -1 ) amount = 1 + amount/10;
    else if( amount > -100 ) amount = -1 + amount/10;
    else if( amount > -1000 ) amount = amount/10;
    else amount = -(75 + random(25));
    return agent->eventMoralAct(amount);
}
