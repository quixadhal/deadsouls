//	/adm/simul_efun/percent.c
//	from the Dead Souls mudlib
//	returns the percent one value is of another
//	created by Descartes of Borg 15 april 1993

float percent(mixed numerator, mixed denominator) {
    float a, b;

    if(!floatp(numerator) && !intp(numerator)) return 0.0;
    else a = (floatp(numerator) ? numerator : to_float(numerator));
    if(!floatp(denominator) && !intp(denominator)) return 0.0;
    else b = (floatp(denominator) ? denominator : to_float(denominator));
    if((b < 1 && b > -1) || !b || b == 0 || b == 0.0 ) return 0.0;
    return ((a/b)*100);
}

float percent_of(mixed percent, mixed base){
    float a, b;

    a = (floatp(percent) ? percent : to_float(percent));
    b = (floatp(base) ? base : to_float(base));
    if((b < 1 && b > -1) || !b || b == 0 || b == 0.0 ) return 0.0;
    return (a * b) / 100.0;
}

