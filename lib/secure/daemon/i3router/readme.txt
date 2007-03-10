// This I3 router code was written mostly by Tim Johnson (Tim@TimMUD,
// Tim@The Eternal Fantasy)
// To use it, #include the server.h file or else just rename it to server.c
//
// If you use it yourself, I'd like you to mention in whatever appropriate
// (if your I3 network or your primary MUD has a web site or board for example)
// place that I wrote all this and you're welcome to list what you may have
// changed on it after downloading mine.  I'd actually prefer there to be a
// list somewhere simply because I'm curious what kinds of ideas people add.
// If it's something good, other people would probably be curious about it as
// well.
//
// I haven't done any router-to-router communications, so don't expect this to
// hook you up to the main network.  This will only be useful for private I3
// networks.
// Also, this is LPC code and needs to be on a MUD for it to run.  It works
// good on a stock TMI-2 MUD, one day I'll make a stripped-down mudlib solely
// for running my router on (without code for players to log in and such).
// Until then, just download TMI-2 and put these files on your MUD and it
// should work.
//
// -Tim (Last edited February 16, 2004)
//
// Some modifications have been made by Cratylus @ Dead Souls to get it
// working on DS muds and add some features.
// -Crat 05Sep2006
//
// Added inter-router networking support. See irn.h.
// -Crat 23Feb2007
