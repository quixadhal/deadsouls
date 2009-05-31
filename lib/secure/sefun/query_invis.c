varargs int query_invis(object lookee, object looker) {
    if(!lookee && !looker) lookee = this_object();
    if(!looker) looker = previous_object();
    if(looker == lookee) looker = previous_object(1);
    if(!looker || !living(looker)) return 2;
    if(!living(lookee) && lookee->GetInvis()) return 1;
    if(!lookee->GetInvis()) return 0;
    if(creatorp(lookee) && lookee->GetInvis() && !creatorp(looker)) return 1;
    if(archp(lookee) && lookee->GetInvis() && !archp(looker)) return 1;
    if(living(lookee)){
        int lookeelevel, lookerlevel;
        lookeelevel = lookee->GetLevel();
        lookerlevel = looker->GetLevel();
        if(lookeelevel && lookerlevel && lookeelevel >= lookerlevel){
            return 1;
        }
    }
    return 0;
}
