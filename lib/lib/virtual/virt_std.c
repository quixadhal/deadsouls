/*    /lib/std/virtual.c
 *    From DeadSouls Object Library
 *    A virtual object server
 *    Created by Descartes of Borg 970523
 *    Version: @(#) virt_std.c 1.3@(#)
 *    Last modified: 98/09/12
 */

private string recursed = "";

/****************************** events ************************************/
mixed CreateVirtualObject(string fname) {
    object daemon;
    string tmp, vname;
    int x, y, z, s;
    int i, j;

    if(fname == recursed){
        recursed = "";
        error("Recursion error in CreateVirtualObject()");    
        return 0;
    }

    i = strsrch(fname, "/", -1);
    if( i == -1 || i == 0 || i == strlen(fname)-1 ) {
	return -1;
    }
    tmp = fname[(i+1)..];
    if( sscanf(tmp, "%d,%d,%d,%d", x, y, z, s) != 4 ) {
        if( sscanf(tmp, "%d,%d,%d", x, y, z) != 3 ) {
            if( sscanf(tmp, "%d,%d", x, y) != 2 ) {
	        vname = tmp;
	    }
	    else {
	        vname = 0;
	        z = 0;
	    }
        }
        s = 0;
    }
    else {
	vname = 0;
    }
    tmp = fname[0..(i-1)];
    recursed = tmp;
    daemon = load_object(tmp);
    recursed = "";
    if(!daemon) return 0;
    if( vname ) {
	return daemon->CreateNamedObject(vname);
    }
    else {
        if(!s) return daemon->CreateGridObject(x, y, z);
        else return daemon->CreateGridObject(x, y, z, s);
    }
}

/************************** driver applies ********************************/
object compile_object(string fname) {
    return CreateVirtualObject(fname);
}
