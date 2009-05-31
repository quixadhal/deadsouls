varargs string format_page(string *items, int columns, int modifier) {
    int width, i, j, x;
    string ret;

    if(!columns) columns = 2;
    ret = "";
    if( this_player() ) width = ((int *)this_player()->GetScreen())[0];
    else width = 80;
    width = (width/columns) + modifier;
    for(i=0, x = sizeof(items); i<x; i+=columns) {
        for(j=0; j<columns; j++) {
            if(i+j >= x) break;
            ret += arrange_string(items[i+j], width);
        }
        ret += "\n";
    }
    return ret;
}

//Allows you to sort in descending columns
varargs string format_page2(string *items, int columns, int modifier){
    int length, rem, counter, height;
    int extra, sz = sizeof(items);
    string *tmp = ({});
    string *ret = ({});
    while(sz < (columns * 2)) columns--;
    if(!columns) columns = 1;
    length = sz / columns;
    height = length;
    rem = sz % columns;
    if(rem) height++;

    while(sizeof(items)){
        if(rem > 0) extra = 1;
        else extra = 0;
        tmp = ({ items[0..((length-1)+extra)] });
        ret += tmp;
        items = items[((length-1)+extra+1)..];
        rem--;
    }
    counter = 0;
    tmp = ({});
    while(height){
        foreach(mixed arr in ret){
            if(sizeof(arr) >= (counter + 1) ) tmp += ({ arr[counter] });
        }
        height--;
        counter++;
    }
    return format_page(tmp, columns, modifier);
}
