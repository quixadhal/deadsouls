varargs string get_stack(int x) {
    int i, s;
    string list = "";
    string *stack0 = call_stack(0);
    string *stack1 = call_stack(1);
    string *stack2 = call_stack(2);
    string *stack3 = call_stack(3);
    for(i = 0, s = sizeof(stack1); i < s; i++){
        list +="\n"+i+":"+
            "%^RED%^" + identify(stack1[i]) + 
            "%^RESET%^, %^GREEN%^file: "+stack0[i]+
            "%^RESET%^, %^BLUE%^fun: "+stack2[i]+
            "%^RESET%^, %^MAGENTA%^origin: "+stack3[i]+
            "%^RESET%^";
    }
    if(!x){
        list = strip_colours(list);
    }
    return list;
}
