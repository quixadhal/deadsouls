#include <daemons.h>

float currency_rate(string type) { 
    return (float)ECONOMY_D->__Query(type, "rate");
}

float currency_inflation(string type) {
    return (float)ECONOMY_D->__Query(type, "inflation");
}

int currency_mass(int x, string type) {
    return to_int((float)ECONOMY_D->__Query(type, "weight") * x);
}

int currency_value(int x, string str) {
    return to_int(x * currency_rate(str));
}

string *mud_currencies() { return (string *)ECONOMY_D->__QueryCurrencies(); }
