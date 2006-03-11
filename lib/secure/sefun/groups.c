int imud_privp(mixed guy) {
    if(objectp(guy)) guy = guy->GetKeyName();
    if(!stringp(guy)) return 0;
    if(member_group(guy, "INTERMUD")) return 1;
    else return 0;
}

int securep(mixed guy) {
    if(objectp(guy)) guy = guy->GetKeyName();
    if(!stringp(guy)) return 0;
    if(member_group(guy, "SECURE")) return 1;
    else return 0;
}
