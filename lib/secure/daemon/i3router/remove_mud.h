// This file written completely by Tim Johnson (Tim@TimMUD)

static void remove_mud(string mudname){
    if(connected_muds[mudname]){
	Debug("Cannot remove mud ["+mudname+"] because it is still online right now.\n");
	return;
    }
    if(!mudinfo[mudname]){
	Debug("Cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
	return;
    }
    map_delete(mudinfo, mudname);
}
