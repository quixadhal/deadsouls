#include <lib.h>
#include <save.h>

inherit LIB_DAEMON;

mixed pkginst, name, category, arch, version, desc, pstamp, instdate, status, basedir, vendor, hotline, email;

static void create() {
}

mixed b(string file){
    string ret = "", foo = read_file(file);
    string *foo_arr = explode(foo, "\n\n");
    int i = 0;
    string path = path_prefix(file);
    string fname = last_string_element(file, "/");
    string thingy;
    tc("foo_arr: "+sizeof(foo_arr));
    foreach(string tmp in foo_arr){
        string tmp2, sub = "", *lines = explode(tmp, "\n");
        pkginst = name = category = arch = version = desc = pstamp = instdate = status = basedir = vendor = hotline = email = 0;
        foreach(string line in lines){
            line = trim(line);
            if(!strsrch(line, "PKGINST:")) pkginst = replace_string(line, "PKGINST:  ", "") + "^";
            if(!strsrch(line, "NAME:")) name = replace_string(line, "NAME:  ", "") + "^";
            if(!strsrch(line, "CATEGORY:")) category = replace_string(line, "CATEGORY:  ", "") + "^";
            if(!strsrch(line, "ARCH:")) arch = replace_string(line, "ARCH:  ", "") + "^";
            if(!strsrch(line, "VERSION:")) version = replace_string(line, "VERSION:  ", "") + "^";
            if(!strsrch(line, "DESC:")) desc = replace_string(line, "DESC:  ", "") + "^";
            if(!strsrch(line, "PSTAMP:")) pstamp = replace_string(line, "PSTAMP:  ", "") + "^";
            if(!strsrch(line, "INSTDATE:")) instdate = replace_string(line, "INSTDATE:  ", "") + "^";
            if(!strsrch(line, "STATUS:")) status = replace_string(line, "STATUS:  ", "") + "^";
            if(!strsrch(line, "BASEDIR:")) basedir = replace_string(line, "BASEDIR:  ", "") + "^";
            if(!strsrch(line, "VENDOR:")) vendor = replace_string(line, "VENDOR:  ", "") + "^";
            if(!strsrch(line, "HOTLINE:")) hotline = replace_string(line, "HOTLINE:  ", "") + "^";
            if(!strsrch(line, "EMAIL:")) email = replace_string(line, "EMAIL:  ", "") + "^";
        }
        foreach(string element in ({"pkginst", "name", "category", "arch", "version", "basedir", "vendor", "desc", "pstamp", "instdate", "hotline", "status", "email"})){
            mixed mx = fetch_variable(element);
            string ret2;
            if(!mx) ret2 = "NULL^";
            else ret2 = mx;
            sub += ret2;
        }
        i++;
        //tc("file: "+file+i);
        //write_file(file+i, ""+tmp);
        //tc("\n\ntmp: "+tmp);
        tc("\n\nsub: "+sub);
        ret += sub + "\n";
    }
    thingy = path+"/"+time()+"_"+fname;
    tc("thingy: "+thingy);
    write_file(thingy, ret, 1);
}
