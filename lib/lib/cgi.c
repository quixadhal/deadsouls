#include <lib.h>

mapping ParsePost(string args){
    object ob = previous_object();
    string boundary = "--"+ ob->GetBoundary();
    string file, junk;
    string *tmp_arr;
    mapping ret = ([]);
    int start = 1;

    if(last(args,2) == "--") args = truncate(args,2);

    tmp_arr = explode(args,boundary);

    foreach(string element in tmp_arr){
        string name,data,junk1, junk2;
        string *tmp_arr2;
        sscanf(element,"%sname=\"%s\"%s",junk1, name, junk2);
        if(!name) continue;
        if(sscanf(element,"%sfilename=\"%s\"%s",junk1, file, junk2) == 3){
            ret["filename"] = file;
        }
        tmp_arr2 = explode(element,"\n");
        if(sizeof(tmp_arr2) > 1 && grepp(tmp_arr2[1],"Content-Type:")) start = 2;
        data = implode(tmp_arr2[start..],"\n");
        if(data[0..0] == "\n") data = data[1..];
        ret[name] = data;
    }
    return ret;
}
