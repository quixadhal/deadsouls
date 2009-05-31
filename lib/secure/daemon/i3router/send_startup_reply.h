// This file written mostly by Tim Johnson (Tim@TimMUD)

static void send_startup_reply(string mudname){
    // This packet will be delivered to a mud for three conditions:
    //  in response to a startup-req packet,
    //  when the router wishes the mud to connect to a different router,
    //  or when the set of routers change for some reason.

    if(undefinedp(connected_muds[mudname])) {
        //trr("PROBLEM","red");
        return;
    }

    else {
        //if(mudinfo[mudname] && mudinfo[mudname]["password"])
        //trr(mudname +" apparently exists. password: "+mudinfo[mudname]["password"],"white");
        //else //trr(mudname +" apparently exists, but I can't find a password for it.","white");
    }


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

    //trr("connected_muds[mudname]: "+identify(connected_muds[mudname]),"blue");
    //trr("mudname: "+identify(mudname),"blue");
    //trr("router_name: "+identify(router_name),"blue");
    //trr("mudinfo[mudname]: "+identify(mudinfo[mudname]),"yellow");
}
