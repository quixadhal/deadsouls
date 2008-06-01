varargs int AddStuff(string *str, int q){
    int i,problem,z;
    problem = 1;
    for(i=0;i<sizeof(str);i++){
        object to,thing;
        if(!to=this_object()) return 0;
        if(q){
            for(z=0;z<q;z++){
                thing=new(str[i]);
                if(!thing->eventMove(to)) problem = -1;
            }
        }
        else if(!q || q ==0){
            thing=new(str[i]);
            if(!thing->eventMove(to)) problem = -1;
        }


    }
    return problem;
}

