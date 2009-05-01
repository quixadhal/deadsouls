int LocalHostedChans(){
    validate();

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus",
                router_name, 0, "dead_souls", 0 }) );

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus",
                router_name, 0, "dead_test4", 0 }) );

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus",
                router_name, 0, "dchat", 0 }) );

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus", 
                router_name, 0, "imud_gossip", 0 }) );

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus", 
                router_name, 0, "imud_code", 0 }) );

    process_channel(1, ({ "channel-add", 5, 
                "Frontiers", "cratylus", 
                router_name, 0, "lpuni", 0 }) );

    process_channel(1, ({ "channel-add", 5,
                "Frontiers", "cratylus",
                router_name, 0, "free_speech", 0 }) );

    return 1;
}
