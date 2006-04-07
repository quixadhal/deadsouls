// This file written completely by Tim Johnson (Tim@TimMUD)

varargs void Debug(string str, int level){
    if(find_player(DEBUGGER_GUY))
	switch(level){
    case DEB_IN:
	message("Debug", "%^BOLD%^%^WHITE%^SERVER INCOMING: %^MAGENTA%^"+str+"%^RESET%^\n",
	  find_player(DEBUGGER_GUY));
	break;
    case DEB_OUT:
	message("Debug", "%^BOLD%^%^WHITE%^SERVER OUTGOING: %^CYAN%^"+str+"%^RESET%^\n",
	  find_player(DEBUGGER_GUY));
	break;
    case DEB_INVALID:
	message("Debug", "%^BOLD%^%^WHITE%^SERVER THINKS THIS IS INVALID: %^RED%^"+str+"%^RESET%^\n",
	  find_player(DEBUGGER_GUY));
	break;
    case DEB_OTHER:
    default:
	message("Debug", "%^BOLD%^%^WHITE%^SERVER INFO: %^GREEN%^"+str+"%^RESET%^\n",
	  find_player(DEBUGGER_GUY));
	break;
    }
}
