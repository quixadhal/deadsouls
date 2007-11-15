varargs string get_stack( int x) {
    int i, s;
    string list = "";
    string *stack0 = call_stack(0);
    string *stack1 = call_stack(1);
    string *stack2 = call_stack(2);
    for(i = 0, s = sizeof(stack1); i < s; i++){
        list +="\n"+i+":"+identify(stack2[i])+"."+identify(stack1[i])+"."+identify(stack2[i]);
    }

    if(x){
        list += "\n"+ identify(previous_object(-1));
    }

    return list;
}
