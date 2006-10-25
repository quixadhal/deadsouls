#include <lib.h>
#include <daemons.h>

int x;
int y;
int copper,cents,silver,electrum,gold,dollars,platinum;
int PutCoins(object who);

int Payment(object who, int value) {
    return true(who, value);
}

int PutCoins(object who) {
    return true(who);
}
