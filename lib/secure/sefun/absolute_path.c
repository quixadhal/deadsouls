/*    /adm/sefun/absolute_path.c
 *    from Foundation II
 *    returns the full path of a string based on a relative path
 *    created by Huthar@Portals as resolv_path.c
 *    modifications by Pallando@Dead Souls 930526
 *    changed to absolute_path() by Descartes of Borg 940501
 *    features added and speed doubled by Pallando, 940531
 */

string absolute_path(string curr, string newp) {
    int i, len;
    string *tmp;
    string name, rest;

    if(curr && (curr == "cwd") && this_player())
      curr = (string)this_player()->query_cwd();
    if(!newp || newp == "" || newp == ".") return curr;
    if( (newp == "here") && this_player() )
    {
        return file_name(environment(this_player())) + ".c";
    }
    len = strlen( newp );
    switch( newp[0..0] )
    {
        case "~":
	    if( newp == "~" || newp == "~/" )
		newp = user_path( (string)this_player()-> GetKeyName() )[0..<2];
            else if( newp[1..1] == "/" )
		newp = user_path( (string)this_player()-> GetKeyName() ) +
		  newp[2..len];
            else if( sscanf( newp, "~%s/%s", name, rest ) == 2 )
		newp = user_path( name ) + rest;
	    else
		newp = user_path( newp[1..len] )[0..<2];
	    break;
        case "^":
	    newp = "/domains/" + newp[1..len];
	    break;
	case "/":
	    break;
	default:
            if( curr != "/" ) newp = curr + "/" + newp;
            else newp = curr + newp;
    }

    if( -1 == strsrch( newp, ".." ) ) return newp;

    if( newp[<1] != '/' ) newp += "/";
    tmp = explode(newp,"/");
    if (!tmp) tmp = ({"/"});
    for(i = 0; i < sizeof(tmp); i++)
        if(tmp[i] == "..") {
            if(sizeof(tmp) > 2) {
                tmp = tmp[0..(i-2)] + tmp[(i+1)..(sizeof(tmp)-1)];
                i -= 2;
            } else {
                tmp = tmp[2 ..(sizeof(tmp)-1)];
                i = 0;
            }
        }
     newp = "/" + implode(tmp,"/");
     if(newp == "//") newp = "/";
     return newp;
}
