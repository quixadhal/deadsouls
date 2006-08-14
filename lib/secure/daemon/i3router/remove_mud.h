// This file written completely by Tim Johnson (Tim@TimMUD)

static void validate();

varargs void remove_mud(string mudname, int forced){
    validate();
    if(connected_muds[mudname] && !forced){
	trr("Cannot remove mud ["+mudname+"] because it is still online right now.\n");
	return;
    }
    if(!mudinfo[mudname]){
	trr("Cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
	return;
    }
    map_delete(mudinfo, mudname);
    if(connected_muds[mudname]) map_delete(connected_muds, mudname);
}
