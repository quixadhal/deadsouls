varargs string format_page(string *items, int columns, int modifier) {
    int width, i, j, x;
    string ret;

    if(!columns) columns = 2;
    ret = "";
    if( this_player() ) width = ((int *)this_player()->GetScreen())[0];
    else width = 80;
    width = (width/columns) + modifier;
    //tc("items: "+identify(items)+", width: "+width+", columns: "+columns);
    for(i=0, x = sizeof(items); i<x; i+=columns) {
        for(j=0; j<columns; j++) {
            if(i+j >= x) break;
            ret += arrange_string(items[i+j], width);
        }
        ret += "\n";
    }
    return ret;
}
