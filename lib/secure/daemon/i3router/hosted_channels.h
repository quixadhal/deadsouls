int LocalHostedChans(){
    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus",
	"yatmim", 0, "dead_souls", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus",
	"yatmim", 0, "dead_test4", 0 }) );

    //process_channel(1, ({ "channel-add", 5,
    //mud_name(), "cratylus",
    //"yatmim", 0, "dead_test5", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus", 
	"yatmim", 0, "imud_gossip", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus", 
	"yatmim", 0, "imud_code", 0 }) );

    process_channel(1, ({ "channel-add", 5, 
	"LPUni Outpost", "tacitus", 
	"yatmim", 0, "lpuni", 0 }) );

    return 1;
}
