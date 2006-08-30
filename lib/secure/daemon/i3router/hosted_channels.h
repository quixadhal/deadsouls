int LocalHostedChans(){
    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus",
	"yadsm", 0, "dead_souls", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus",
	"yadsm", 0, "dead_test4", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus",
	"yadsm", 0, "dchat", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus", 
	"yadsm", 0, "imud_gossip", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	mud_name(), "cratylus", 
	"yadsm", 0, "imud_code", 0 }) );

    process_channel(1, ({ "channel-add", 5, 
	"LPUni Outpost", "tacitus", 
	"yadsm", 0, "lpuni", 0 }) );

    process_channel(1, ({ "channel-add", 5,
	"Haven", "duuk",
	"yadsm", 0, "free_speech", 0 }) );

    return 1;
}
