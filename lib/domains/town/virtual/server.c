#define __DIR__ "/domains/town/virtual/"

mixed compile_object(string file) {
    string *path;
    object ob;
    int x, y;
    path = explode(file, "/");
    if( file_size(__DIR__ + path[3] + ".c") < 1 ) return "bad file";
    if( sscanf(path[4], "%d,%d", x, y) != 2 ) return "missing comma";    
    if( !(ob = new(__DIR__ + path[3], x, y)) ) return "failed to compile";
    return ob;
}
