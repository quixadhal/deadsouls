// This file written completely by Tim Johnson (Tim@TimMUD)

static void send_startup_reply(string mudname){
    // This packet will be delivered to a mud for three conditions:
    //  in response to a startup-req packet,
    //  when the router wishes the mud to connect to a different router,
    //  or when the set of routers change for some reason.
    if(!connected_muds[mudname]) return; // This shouldn't be needed.
    write_data(connected_muds[mudname], ({
	"startup-reply",
	5,
	router_name,
	0,
	mudname,
	0,
	router_list,
	mudinfo[mudname]["password"]
      }) );
}
