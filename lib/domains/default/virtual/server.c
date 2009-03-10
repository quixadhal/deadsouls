#define __DIR__ "/domains/default/virtual/"

mixed compile_object(string file) {
    string *path;
    object ob;
    int x, y, z, elements;
    //tc("file: "+file);
    if(grepp(file, "/user_") || grepp(file, "/object_")){
        //tc("1");
        ob = new("/domains/default/virtual/void");
        if(!ob){
            //tc("no "+identify(file),"red");
            return "No void room available.";
        }
        //else tc("loaded "+identify(ob),"blue");
        return ob;
    }
    //tc("2");
    //if( previous_object() != master() ) return 0;
    path = explode(file, "/");
    //if( sizeof(path) != 5 ) return "Wrong size path";
    if( file_size(__DIR__ + path[3] + ".c") < 1 ) return "bad file";
    if( (elements = sscanf(path[4], "%d,%d,%d", x, y, z)) != 3 ) {  
        if( (elements = sscanf(path[4], "%d,%d", x, y)) != 2 ) return "missing comma";    
    }
    if( elements == 2 && !(ob = new(__DIR__ + path[3], x, y)) ) return "failed to compile";
    else if( elements == 3 && !(ob = new(__DIR__ + path[3], x, y, z)) ) return "failed to compile";
    return ob;
}
