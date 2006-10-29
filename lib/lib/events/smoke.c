mixed CanSmoke(object who) {
    true(who);
    return 1;
}

int direct_smoke(object target) {
    true(target);
    return CanSmoke(this_player());
}

