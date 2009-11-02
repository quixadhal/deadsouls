/* Stupid blackjack table
 * works, other than the fact that the value of ace always equals 1.
 * by boy@frontiers.
 * (You can get two of the same card, like two 8 of spades.)
 * Too hard and too much work to simulate a whole deck.
 */
#include <lib.h>
inherit LIB_ITEM;
void StupidAce();
int needace;
int aceval;
int gameon;
int score;
int dscore;
int bet;
int plidle;
string playername,oldplayername;
object player,oldplayer,here;
void cleanup();
void eventDealerHit();
void YouWin();
void YouLose();

int read(string args) {
    write("The small letters read...");
    write(" %^CYAN%^Dealer stands pat at 17.");     write(" %^CYAN%^Ties go to the house.");
    write("deal <amount> : Starts the game, wagering <amount>");     write(" hit : Requests a new card");
    write(" stand pat : Ends the game");
    return 1;
}

static void create() {
    ::create();
    SetShort("a blackjack table");
    SetLong("A small mechanical blackjack table lies here, "
            "with a miniature robotic dealer "
            "built into it. It seems like you can win...err "
            "try to win a lot of money here.  There are some small "
            "printed letters on the table.");
    SetPreventGet("You cannot get that!");
    SetMass(0);
    SetId( ({"table", "blackjack table"}) );
    SetKeyName("table");
    SetItems( ([
                ({"writing","letters"}): "Some writing on the table. Try \"read "+
                "writing on table\"",
                ]) );
    SetRead( ([
                ({"writing","letters"}) : (: read :),
                ]) );
    SetDefaultRead("default", (: read :) );
    oldplayername="this_is_a_silly_string";
}
void init() {
    ::init();
    add_action("eventHit", "hit");
    add_action("eventStand", "stand");
    add_action("eventDeal", "deal");
}
int eventHit() {
    int done;
    int cardscore;
    string card, suit;
    int x;
    done = 0;
    if (gameon == 0) {
        write("A game has not started yet, type \"deal\" to start one.");
        return 1;
    }
    if(gameon == 1 && this_player()->GetName() != oldplayername) {
        write("You are not playing right now.");
        return 1;
    }
    x = random(13);
    switch (x + 1) {
        case 1 : card = "Ace";
                 break;
        case 11 : card = "Jack";
                  break;
        case 12 : card = "Queen";
                  break;
        case 13 : card = "King";
                  break;
        default : card = ""+(x + 1);
    }
    switch (random(4)) {
        case 1 : suit = "Hearts";
                 break;
        case 2 : suit = "Spades";
                 break;
        case 3 : suit = "Diamonds";
                 break;
        case 0 : suit = "Clubs";
    }
    write("%^BLUE%^You get a " + card + " of " + suit);
    if (x == 0) {
        StupidAce();
        cardscore = aceval;
        done = 1;
    }
    if (x > 9) {
        cardscore = 10;
        done = 1;
    }
    else {
        if (done == 0) {
            cardscore = x + 1;
        }
    }
    score += cardscore;
    if (score > 21) {
        if(aceval == 11) {
            aceval = 1;
            score -= 11;
            write("%^CYAN%^Your score is " + score);
        }
        else {
            YouLose();
        }
    }
    if (score == 21) {
        YouWin();
    }
    return 1;
}
int eventStand(string str) {
    int lose;

    if(str != "pat") return 0;
    if (gameon == 0) {
        write("A game has not started yet, type \"deal\" to start one");
        return 1;
    }
    if(gameon == 1 && this_player()->GetName() != oldplayername) {
        write("You are not playing right now.");
        return 1;
    }
    write("%^CYAN%^Your final score is " + score);

    if (dscore < 22) {
        write("%^BOLD%^The dealer's score is " + dscore);
    }
    else {
        write("%^BOLD%^The dealer busts");
    }
    if (score > dscore && score < 21) {
        YouWin();
    }
    else {
        if (dscore > 21) {
            YouWin();
        }
        else {
            YouLose();
        }
    }
    return 1;
}
void cleanup() {
    oldplayername = "this_is_a_silly_string";
    aceval=0;
    score = 0;
    dscore = 0;
    bet = 0;
    gameon = 0;
}
int eventDeal(string args) {
    player=this_player();
    playername=player->GetName();
    here=environment(this_object());
    if(oldplayer) plidle=query_idle(oldplayer);

    if (!args) {
        write("%^YELLOW%^Please try that again, including the amount of money you want to wager.");
        return 1;
    }
    sscanf(args, "%d", bet);
    if(!intp(bet) || bet <= 0){
        write("You must wager an amount.\n");
        return 1;
    }
    if (this_player()->GetCurrency("silver") < bet) {
        write("You don't have that many silver!");
        return 1;
    }
    if (gameon == 1 && present(oldplayername,here) && plidle < 120 ) {
        write("%^RED%^"+oldplayername+" is playing right now. There is room for only one player.");
        return 1;
    }
    if (gameon == 1 && present(oldplayername,here) && plidle > 120 ) {
        write("%^RED%^"+oldplayername+" was playing, but "+nominative(oldplayer)+" has gone idle "+
                "and the dealer deals "+objective(oldplayer)+" out.",oldplayer);
        say(oldplayername+" has been dealt out of the blackjack table for idleness.",oldplayer);
        tell_object(oldplayer,"You've been dealt out of the blackjack table for idleness.");
        cleanup();
    }
    if (gameon == 1 && !present(oldplayername,here) ) {
        write("%^RED%^"+oldplayername+" was playing, but "+nominative(oldplayer)+" has left, and the dealer deals "+
                objective(oldplayer)+" out.");
        say(oldplayername+" has been dealt out of the blackjack table because "+nominative(oldplayer)+" left.",oldplayer);
        tell_object(oldplayer,"You suddenly remember you left a blackjack game in the middle of it, "+
                "and you've probably been dealt out.");
        cleanup();
    }

    oldplayername = playername;
    oldplayer = player;
    gameon = 1;
    write("%^RED%^You ask the dealer to count you into the next game.");
    say("%^YELLOW%^" + this_player()->GetName() + " starts playing blackjack.");
    this_player()->AddCurrency("silver", -bet);
    eventDealerHit();
    eventHit();
    eventHit();
    write("%^CYAN%^Your current score is  " + score);
    return 1;
}
void eventDealerHit() {

    int cardscore;
    int x;
    while (dscore < 17) {
        x = random(13);
        if (x > 9) {
            dscore += 10;
        }
        else {
            dscore += x;
        }
    }
}
void YouLose() {
    write("Your score is " + score);
    write("%^RED%^You lose");
    say("%^YELLOW%^" + this_player()->GetName() + " loses.");
    cleanup();
}
void YouWin() {
    write("Your score is " + score);
    write("%^GREEN%^You win!");
    say("%^YELLOW%^" + this_player()->GetName() + " wins!");
    this_player()->AddCurrency("silver", 2 * bet);
    cleanup();
}
void StupidAce() {
    int temp;
    if ((score + 11) > 21) {
        aceval = 1;
    }
    else {
        aceval = 11;
    }
    temp = aceval + score;
    write("%^CYAN%^Your current score is " + temp);
}

