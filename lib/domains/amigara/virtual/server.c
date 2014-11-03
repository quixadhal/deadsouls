#define __DIR__ "/domains/amigara/virtual/"

mixed compile_object(string file) {
    string *path;
    object ob;
    int x, y, z, elements;
    if(grepp(file, "/user_") || grepp(file, "/object_")){
        ob = new("/domains/amigara/virtual/void");
        if(!ob){
            return "No void room available.";
        }
        return ob;
    }
    path = explode(file, "/");
    if( file_size(__DIR__ + path[3] + ".c") < 1 ) return "bad file";
    if( (elements = sscanf(path[4], "%d,%d,%d", x, y, z)) != 3 ) {  
        if( (elements = sscanf(path[4], "%d,%d", x, y)) != 2 ) return "missing comma";    
    }
    if( elements == 2 && !(ob = new(__DIR__ + path[3], x, y)) ) return "failed to compile";
    else if( elements == 3 && !(ob = new(__DIR__ + path[3], x, y, z)) ) return "failed to compile";
    return ob;
}
