string get_stack() {
    string *stack;
    string list;
    stack = call_stack(2);
    list = implode(stack," ");
    return list;
}

