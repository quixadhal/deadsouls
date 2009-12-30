// Tim Johnson
// Started on May 1, 2004.
// Use this however you want.

#ifndef VERSION
#define VERSION "Tim's LPC IMC2 client - Jan 30, 2005"
#endif

string tmpstr;

// Mode decides what kind of packet is expected
#define NO_UIDS
#define MODE_CONNECTED 1
#define MODE_WAITING_ACCEPT 2
#define MODE_CONNECT_ERROR 3 // Not used yet, I need to see what the hub sends.
#define MODE_HUB_DOWN 4 // Not used yet either.

#ifndef STREAM
#define STREAM 1
#endif
#ifndef EESUCCESS
#define EESUCCESS         1     /* Call was successful */
#endif
#ifndef THIS_PLAYER
#define THIS_PLAYER this_player()
#endif
#ifndef FIND_PLAYER
#define FIND_PLAYER(x) find_player(x)
#endif
#ifndef GET_CAP_NAME
#define GET_CAP_NAME(x) replace_string(x->GetName()," ","")
#endif
#ifndef GET_NAME
#define GET_NAME(x) convert_name(x->GetName())
#endif
#ifndef IMC2_MSG
#define IMC2_MSG(x,y) foreach(tmpstr in explode(x,"\n")){ message("IMC2",tmpstr,y); }
#endif
#ifndef GET_GENDER
#define GET_GENDER(x) x->GetGender()
#endif
#ifndef ADMIN
#define ADMIN(x) archp(x)
#endif

#define HTML_LOCATION "http://dead-souls.net/"

// Other things that could be #define'd...
// INVIS(x) !visible(x)
// TELL_BOT "/u/t/timbot/imc2_invalidtells.c"
// TELL_BOTS ([ "timbot" : "/u/t/timbot/imc2_tellbot.c" ])
// CHAN_BOT "/u/t/timbot/imc2_chans.c"
// CHAN_BOTS ([ "ichat" : "/u/t/timbot/imc2_ichat.c" ])
// USER_EXISTS(x) user_exists(x)

// Debugging
//#define DEB_IN  1
//#define DEB_OUT 2
//#define DEB_PAK 3
//#define DEB_OTHER 0

int socket_num;
int mode;
mapping ping_requests; // Keeps track of who sent a ping request.
// Ping requests aren't labelled with names, so replies are destined to this MUD
// with no idea why, unless we keep track.
string buf=""; // Buffer for incoming packets (aren't always sent 1 at a time)

// Variables
string hub_name, network_name;
string server_pass, server_version;
mapping chaninfo;
mapping localchaninfo; // (["chan": ([ "perm":1, "name":"something", "users":({ }) ]) ])
mapping mudinfo;
mapping genders;
mapping tells;
int sequence;

// Prototypes :)
void create();
void remove();
string pinkfish_to_imc2(string str);
string imc2_to_pinkfish(string str);
string escape(string str);
string unescape(string str);
mapping string_to_mapping(string str);
string main_help();

private void send_packet(string sender, string packet_type, string target, string destination, string data);
private void send_text(string text);
private void got_packet(string info);
private void start_logon();
private varargs void send_is_alive(string origin);
private void channel_in(string fromname, string frommud, mapping data);
private void tell_in(string sender, string origin, string target, mapping data);
private void beep_in(string sender, string origin, string target, mapping data);
private void who_reply_in(string origin, string target, mapping data);
private void whois_in(string fromname, string frommud, string targ, mapping data);
private void whois_reply_in(string targ,string fromname,string frommud,mapping data);
private void ping_reply_in(string sender,string origin,string target,mapping data);
private void chanwho_reply_in(string origin, string target, mapping data);
private void send_keepalive_request();
private int chan_perm_allowed(object user, string chan);
private string localize_channel(string str);
private void chan_who_in(string fromname, string frommud, mapping data);
private void send_ice_refresh();
private void resolve_callback(string address, string resolved, int key);

//private varargs void Debug(mixed msg, int x){
// Add stuff in here if you want to see messages.
//}

// Functions for users to change.
int can_use(object user){ return 1; } // Is this person allowed to use IMC2 at all?  This function determines if tells can be sent to the person and such.
int level(object ob){
    // Outgoing packets are marked with the user's level.
    // This function figures it out.
    // If you have different ways of ranking, make this function convert them to what IMC2 uses.
    // IMC2 uses: Admin=5, Imp=4, Imm=3, Mort=2, or None=1
    if(ADMIN(ob)) return 5; // Admin
    //TMI-2: if(wizardp(ob)) return 3;
    //TMI-2: if(userp(ob)) return 2;
    //Discworld: if(ob->query_creator()) return 3;
    if(this_player()) return 2;
    return 1; // None
}

string chan_perm_desc(int i){
    // Given the permission level assigned locally to a channel, return a short
    // string describing what the number means.  The number means nothing
    // outside of this MUD.  Also, they are independant of each other, and
    // so you can do groups without having to always do subgroups of higher
    // ones or anything like 'levels'.  BACKLOG_WEB_LEVEL is the only one of
    // significance, as it's the only one that the web backlog thing works on.
    switch(i){
    case 2: return "arch";
    case 1: return "creator";
    case 0: return "public";
    }
    return "invalid";
}

int chan_perm_allowed(object user, string chan){
    // Using the permission level assigned locally to a channel,
    // return 1 if user is allowed to use the channel, 0 if not.
    switch(localchaninfo[chan]["perm"]){
    case 2: if(archp(user)) return 1; return 0;
    case 1: if(creatorp(user)) return 1; return 0;
    case 0: return 1;
    }
}


// Shouldn't have to change anything beyond this point.

private int read_callback(object socket, string info){
    // string str, *strs;
    string a,b; int done=0;
    if(!sizeof(info)) return 0;
    //Debug(save_variable(info),DEB_IN);
#ifdef DATA_LOG
    log_file(DATA_LOG,"SERVER: "+save_variable(info)+"\n");
#endif
    buf += info;
    // The hub groups packets, unfortunately.
    switch(mode){
    case MODE_WAITING_ACCEPT: // waiting for Hub to send autosetup
        if(sscanf(info, "autosetup %s accept %s\n"+CARRIAGE_RETURN,
            hub_name, network_name)==2){
            //Debug("Connected, hub is "+hub_name+", network is "+network_name);
            mode = MODE_CONNECTED;
            send_is_alive("*");
            send_keepalive_request();
            send_ice_refresh();
        }
        else if(sscanf(info, "PW %s %s version=%d %s\n",
            hub_name, server_pass, server_version, network_name)==4){
            mode = MODE_CONNECTED;
            send_is_alive("*");
            send_keepalive_request();
            send_ice_refresh();
        }
        else{ // Failed login sends plaintext error message.
            //Debug("Failed to connect... "+info);
        }
        buf=""; // clear buffer
        break;
    case MODE_CONNECTED:	
        while(!done){
            if(sscanf(buf,"%s\n"+CARRIAGE_RETURN+"%s",a,b)==2){ // found a break...
                got_packet(a);
                buf=b;
            }
            else{ // no break...
                done = 1;
            }
        }
        /*
                                strs = explode(info,"\n"+CARRIAGE_RETURN);
                                foreach(str in strs){
                                        got_packet(str);
                                }
                                buf="";
        */
        break;
    }
    return 1;
}

private void got_packet(string info){
    string str;
    string a,b;
    int i;
    string sender, origin, route, packet_type, target, destination, strdata;
    int sequence;
    mapping data;
    object who;
    if(!sizeof(info)) return;
    //Debug(save_variable(info),DEB_PAK);

    str = info;
    // messages end with newlines or sometimes just a space
    sscanf(str, "%s\n^", str);
    sscanf(str, "%s"+CARRIAGE_RETURN+"^", str);
    sscanf(str, "%s ^", str);
    sscanf(str, "%s ^", str);
    if(sscanf(str, "%s %d %s %s %s %s",
        a, sequence, route, packet_type,
        b, strdata)==6){ // matches
        if(sscanf(b,"%s@%s",target,destination)!=2){
            // Could be MUD instead of Name@MUD or *@MUD
            target="*"; destination=b;
        }
        if(sscanf(a,"%s@%s",sender,origin)!=2){
            sender="*"; origin=a;
        }
        data = string_to_mapping(strdata);
        //			Debug("sender="+sender);
        //			Debug("origin="+origin);
        //			Debug("sequence="+sequence);
        //			Debug("route="+route);
        //			Debug("packet_type="+packet_type);
        //			Debug("data="+save_variable(data));
        if(!mudinfo[origin]) mudinfo[origin] = ([ ]);

        switch(packet_type){
        case "is-alive": // For making a MUD list.
            if(!mudinfo[origin]) mudinfo[origin] = ([ ]);
            // example of info:
            // versionid=\"IMC2 AntiFreeze CL-2 SWR 1.0\" url=none md5=1
            //mudinfo[origin]["version"]="blah";
            mudinfo[origin]+=data;
            mudinfo[origin]["online"]=1;
            //Debug("handled is-alive for mud "+origin);
            break;
        case "close-notify": // Someone disconnected.
            if(!mudinfo[data["host"]]) mudinfo[data["host"]] = ([]);
            mudinfo[data["host"]]["online"]=0;
            break;
        case "keepalive-request": // Request for is-alive.
            send_is_alive(origin);
            break;
        case "ice-msg-b": // Broadcast channel message.
            channel_in(sender, origin, data);
            break;
        case "tell": // Tells or emotes.
            tell_in(sender, origin, target, data);
            break;
        case "who-reply":
            who_reply_in(origin,target,data);
            break;
        case "whois": // Like I3's locate
            whois_in(sender,origin,target,data);
            break;
        case "whois-reply":
            whois_reply_in(target,sender,origin,data);
            break;
        case "beep":
            beep_in(sender, origin, target, data);
            break;
        case "ice-update": // Reply from ice-refresh.
            chaninfo[data["channel"]]=data;
            break;
        case "wHo": // Drop-through
        case "who":
            send_packet("*","who-reply",sender,origin,
              "text="+escape(pinkfish_to_imc2(WHO_STR)));
            break;
        case "ice-destroy": // Deleting channel.
            map_delete(chaninfo,data["channel"]);
            break;
        case "user-cache": // User info, like I3's ucache service.
            if(!genders[origin]) genders[origin]=([ ]);
            genders[origin][sender]=data["gender"];
            break;
        case "user-cache-reply": // Reply with user info
            if(!genders[origin]) genders[origin]=([ ]);
            genders[origin][data["user"]]=data["gender"];
            break;
        case "user-cache-request": // Request for user info
            sscanf(data["user"],"%s@%*s",str);
            who = FIND_PLAYER(lower_case(str));
            if(who
#ifdef INVIS
              && !INVIS(who)
#endif
            ){
                switch(GET_GENDER(who)){
                case "male" : i=0; break;
                case "female" : i=1; break;
                default : i=2; break;
                }
                send_packet("*","user-cache-reply",sender,
                  origin,sprintf("gender=%d",i));
            }
            break;
        case "ping":
            send_packet("*","ping-reply",sender,origin,
              sprintf("path=\"%s\"",route));
            break;
        case "ping-reply":
            ping_reply_in(sender,origin,target,data);
            break;
        case "ice-msg-r": // Relayed channel message.
            channel_in(sender, origin, data);
            break;
        case "ice-chan-whoreply": // Tell who's listening to a channel.
            chanwho_reply_in(origin,target,data);
            break;
        case "emote": // Channel creation/destruction message... anything else?
            IMC2_MSG(NETWORK_ID+" announces: "+data["text"]+"\n",
              filter_array(users(), (: ADMIN($1) :)));
#ifdef ANNOUNCE_LOG
            log_file(ANNOUNCE_LOG,ctime(time())+": "+data["text"]+"\n");
#endif
            break;
        case "ice-chan-who": // Check who's listening to a channel.
            chan_who_in(sender,origin,data);
            break;
        case "channel-notify":
            // Don't care about this.  Useful only if you care when
            // people on other MUDs start/stop listening to a channel.
            // example: Someone@SomeMUD 1087076772 SomeMUD channel-notify *@*  channel=Hub01:ichat status=0
            break;
            // The following packets shouldn't be incoming.
        case "ice-cmd": // Remote channel administration
        case "remote-admin": // For controlling the hub
        case "ice-refresh": // Request data about channels
        case "ice-msg-p": // Private channel message
            //Debug("This packet isn't supposed to be incoming: "+packet_type);
            break;
        default:
#ifdef UNKNOWN_DATA_LOG
            log_file(UNKNOWN_DATA_LOG,"Unknown packet: "+escape(info)+"\n\n");
#endif
            //Debug("Unlisted packet type: "+packet_type);
            break;
        }
    }
    else{
        buf += info;
        //Debug("Doesn't match incoming pattern, so putting on buffer: "+str);
#ifdef BAD_PACKET
        log_file(BAD_PACKET,"Doesn't match incoming pattern: "+str+"\n");
#endif
    }
}

private int close_callback(object socket){
    // Connection was closed.
#ifdef DATA_LOG
    log_file(DATA_LOG,"DISCONNECTED\n");
#endif
    socket_close(socket_num);
    //	socket->remove();
    create();
    return 1;
}
private void send_text(string text){
    // Send a literal string.
    // Almost everything should use the send_packet function instead of this.
#ifdef DATA_LOG
    log_file(DATA_LOG,"CLIENT: "+save_variable(text)+"\n");
#endif
    //Debug(save_variable(text), DEB_OUT);
    //	Debug("writing to socket: "+socket_num);
    socket_write(socket_num,text);
    //	imc2_socket->send(text);
    return;
}

void create(){ 
#ifndef NO_UIDS
    seteuid(getuid());
#endif
#ifdef DATA_LOG
    log_file(DATA_LOG,"Creating IMC2 object at "+ctime(time())+".\n");
#endif
    if(sizeof(get_dir(SAVE_FILE+".o"))) RestoreObject(SAVE_FILE);
    if(!mudinfo) mudinfo = ([ ]);
    if(!chaninfo) chaninfo = ([ ]);
    if(!localchaninfo) localchaninfo = ([ ]);
    if(!genders) genders = ([ ]);
    if(!tells) tells=([ ]);
    ping_requests=([ ]);
    mode = MODE_WAITING_ACCEPT;
#ifdef HOSTIP
    // We already know the IP, go straight to the connecting, just do callback as if it found the IP.
    resolve_callback(HOSTIP,HOSTIP,1);
#else
    if(!resolve(HOSTNAME, "resolve_callback")){
        //Debug("Addr_server is not running, resolve failed.");
#ifdef DATA_LOG
        log_file(DATA_LOG,"Addr_server is not running, resolve failed.\n");
#endif
        remove();
        return;
    }
#endif
    //Debug("creating IMC2 object");
}

void remove(){
    // This object is getting destructed.
    if(!socket_num) socket_close(socket_num);
    //	if(imc2_socket) imc2_socket->remove();
#ifdef DATA_LOG
    log_file(DATA_LOG,"IMC2 OBJECT REMOVED\n");
#endif
    SaveObject(SAVE_FILE);
    destruct(this_object());
}

private void resolve_callback( string address, string resolved, int key ) {
    // Figured out what the IP is for the address.
    int error;
    //Debug("Resolved to: "+resolved);
    write_file("/tmp/imc2.log","address: "+address);
    write_file("/tmp/imc2.log","resolved: "+resolved);
    write_file("/tmp/imc2.log","key: "+key);

    //socket_num = socket_create(STREAM, "close_callback");
    socket_num = socket_create(STREAM, "read_callback", "close_callback");
    if (socket_num < 0) {
#ifdef DATA_LOG
        log_file(DATA_LOG,"socket_create: " + socket_error(socket_num) + "\n");
#endif
        trr("socket_create: " + socket_error(socket_num) + "\n");
        return;
    }
    //Debug("Created socket descriptor " + socket_num);

    error = socket_connect(socket_num, resolved+" "+HOSTPORT, "read_callback", "write_callback");
    if (error != EESUCCESS) {
#ifdef DATA_LOG
        log_file(DATA_LOG,"socket_connect: " + socket_error(error) + 
            "\nGURU MEDITATION #DEADBEEF.FEEDFACE\n");
#endif
        //Debug("socket_connect, error="+error+": " + socket_error(error) + "\n");
        socket_close(socket_num);
        return;
    }
}

private void write_callback(){
    start_logon();
}

private void start_logon(){
    //Debug("Gonna try logging in, sending the PW thing...");
#if NOT_AUTO
    send_text(sprintf("PW %s %s version=%d\n",
#else
        send_text(sprintf("PW %s %s version=%d autosetup %s\n",
#endif
            MUDNAME,
            CLIENT_PW, 2
#ifndef NOT_AUTO
            , SERVER_PW
#endif
          ));
        buf="";
        /* For invitation-only networks.
                send_text(sprintf("PW %s %s version=%d %s\n",
                        MUDNAME,
                        CLIENT_PW, 2, "hub03"
                ));
        */
        sequence=time();
    }

      string escape(string str){
          str=replace_string(str,"\\","\\\\");
          str=replace_string(str,"\"","\\\"");
          str=replace_string(str,"\n","\\n");
          str=replace_string(str,CARRIAGE_RETURN,"\\"+CARRIAGE_RETURN);
          if(sizeof(explode(str," "))!=1) str = "\""+str+"\"";
          return str;
      }

      string unescape(string str){
          string a,b=str,output="";
          while(sscanf(b,"%s\\%s",a,b)==2){
              output += a;
              if(sizeof(b)){
                  switch(b[0]){
                  case 34 : output += "\""; break; // '\"' makes warnings.
                  case '\\' : output += "\\"; break;
                  case 'n' : output += "\n"; break;
                  case 'r' : output += CARRIAGE_RETURN; break;
                  }
              }
              b=b[1..];
          }
          output += b;
          output = replace_string(output,"\n"+CARRIAGE_RETURN,"\n");
          if((sizeof(explode(output," "))==1) && sscanf(output,"\\\"%*s\\\"") ) output = "\""+output+"\"";
          return output;
      }

      mapping string_to_mapping(string str){
          // Picks first element off of string and then repeats?
          mapping out=([]);
          int in_quote=0;
          int i, j;
          string what,data,rest;

          rest = str;

          while(sizeof(rest)>0){
              sscanf(rest, "%s=%s", what, rest);
              /*
                              write("what="+what+", rest="+rest+"\n");
              */
              // At this point, what is the key, rest is value plus rest.
              if(rest[0]==34){ // value is in quotes, tons of fun!
                  // find first quote without a backslash in front?
                  /*
                                          write("rest begings with a quote\n");
                  */
                  i = 1;
                  while(((rest[i]!=34) || (rest[i-1]==92)) && (i<sizeof(rest))){ // 34 = ", 92 = \
                      // While this is not a quote, or if this is an escaped quote, keep looking.
                      i++;
                  }
                  // now are 1 space past quote
                  //			write("i="+i+"\n");
                  data=rest[1..(i-1)]; // skip opening and closing quotes
                  rest=rest[(i+2)..]; // skip past space
                  //			write("new data="+data+"\n");
                  //			write("new rest="+rest+"\n");
                  // Data is now what was in the quotes... now to un-escape the data...
                  out[what]=unescape(data);
              }
              else{ // value is not in quotes, tons of actual non-sarcastic fun!
                  // just split it at the first space
                  if(sscanf(rest,"%s %s",data,rest)!=2){ // break at first space
                      data = rest;
                      rest = "";
                  }
                  if((sscanf(data,"%d",i)==1) && (sprintf("%d",i)==data)) // is just number
                      out[what]=i;
                  else // not just a number
                      out[what]=data;
              }
          }
          return out;
      }

      private void send_packet(string sender, string packet_type, string target, string destination, string data){
          // Sends a packet in the format that IMC2 uses.
          sequence++;
          send_text(sprintf("%s@%s %d %s %s %s@%s %s\n",
              sender,	MUDNAME, sequence, MUDNAME, packet_type,
              target, destination, data
            ));
      }

      void send_keepalive_request(){
          // Ask all the muds to tell us that they're alive.
          string mud;
          // Mark all the muds as offline until they respond.
          foreach(mud in keys(mudinfo)){
              mudinfo[mud]["online"]=0;
          }
          send_packet("*","keepalive-request","*","*","");
      }

      varargs void send_is_alive(string origin){
          // Sends an is-alive packet to whoever requested it, or else broadcasts it.
          send_packet("*","is-alive","*",(origin ? origin : "*"),
            sprintf("versionid=\"%s\" networkname=%s url=%s",
              VERSION, network_name, URL));
      }

      private int chan_listening(object user, string chan){
          // Tells if a user is listening to a channel or not.
          if(!localchaninfo[chan]) return 0;
          if(member_array(GET_NAME(user),localchaninfo[chan]["users"])==-1) return 0;
          return chan_perm_allowed(user,chan);
      }

      void channel_in(string fromname, string frommud, mapping data){
          // Handles an incoming channel message.
          object user, *usrs;
          string msg,blmsg;
          string sender;
          string local;
          int emote=0;
          int sz;

          /*
          tell_object(find_player("tim"),("data="+identify(data)+"\n"));
          */

          sender=fromname+"@"+frommud;
          if(data["sender"]) sender=data["sender"];
          if(data["realfrom"]) sender=data["realfrom"];
          if(intp(data["text"])) data["text"]=sprintf("%d",data["text"]);
          // the data processing function makes "1" into a number,
          // which can be a problem when a person says just a number on a channel :P
          local=localize_channel(data["channel"]);
          if(data["emote"]) emote=data["emote"];
#ifdef CHANNEL_BOT
          // Can define CHANNEL_BOT to something with got_chan(channel,from,text,emote)
          // It will be given all channel messages.  Be aware of this.
          call_other(CHANNEL_BOT,"got_chan",data["channel"],sender,data["text"],emote);
#endif
#ifdef CHANNEL_BOTS
          if(CHANNEL_BOTS[local]){
              call_other(CHANNEL_BOTS[local],"got_chan",local,sender,data["text"],emote);	
          }
          else if(CHANNEL_BOTS[data["channel"]]){
              call_other(CHANNEL_BOTS[data["channel"]],"got_chan",data["channel"],sender,data["text"],emote);
          }
#endif
          if(!local) return; // Channel not used locally, ignore.
          usrs = ({ });
          data["text"]=imc2_to_pinkfish(data["text"]);
          foreach(user in users())
              if(chan_listening(user,local)) usrs += ({ user });
          if(emote==2){
              msg=sprintf("%s- [%%^GREEN%%^%s%%^RESET%%^] %%^CYAN%%^(from %s)%%^RESET%%^ %s",
                NETWORK_ID,local,sender,data["text"]);
              blmsg=sprintf("%s- [%s] (from %s) %s",
                NETWORK_ID,local,sender,data["text"]);
          }
          else if(emote){
              msg=sprintf("%s- [%%^GREEN%%^%s%%^RESET%%^] %%^CYAN%%^%s%%^RESET%%^ %s",
                NETWORK_ID,local,sender,data["text"]);
              blmsg=sprintf("%s- [%s] %s %s",
                NETWORK_ID,local,sender,data["text"]);
          }
          else{
              msg=sprintf("%s- %%^CYAN%%^%s%%^RESET%%^ [%%^GREEN%%^%s%%^RESET%%^]: %s",
                NETWORK_ID, sender,local,data["text"]);
              blmsg=sprintf("%s- %s [%s]: %s",
                NETWORK_ID, sender,local,data["text"]);
          }
          IMC2_MSG(msg+"\n",usrs);
          localchaninfo[local]["backlog"] += ({ ctime(time())[4..18]+" "+blmsg });
          sz = sizeof(localchaninfo[local]["backlog"]);
          if(sz>BACKLOG_SIZE)
              localchaninfo[local]["backlog"]=localchaninfo[local]["backlog"][(sz-BACKLOG_SIZE)..sz];
      }

      private void channel_out(object user,string chan,string msg){
          // Send outgoing channel message.
          send_packet(GET_CAP_NAME(user),"ice-msg-b","*","*",
            sprintf("channel=%s text=%s emote=0 echo=1",
              chan,escape(GET_CAP_NAME(user)+"@"+MUDNAME+" "+msg)));
      }

      private varargs void tell_out(object from, string targname, string targmud, string msg, int reply, int emote){
          // Send outgoing tell.
          if(!reply) reply=0;
          send_packet(GET_CAP_NAME(from),"tell",targname,targmud,
            sprintf("level=%d text=%s reply=%d emote=%d", level(from),escape(msg),reply,emote));
      }

      private void tell_in(string sender, string origin, string target, mapping data){
          // Incoming tell.
          object who;
          int sz;
          string blmsg;
          who=FIND_PLAYER(lower_case(target));
          target=GET_CAP_NAME(who);
          data["text"]=imc2_to_pinkfish(data["text"]);
          if(who
#ifdef INVIS
            && VISIBLE(who)
#endif
            && can_use(who)
          ){
              switch(data["isreply"]){
              case 2: // emote
                  IMC2_MSG(sprintf("%s tell: %%^CYAN%%^%s@%s%%^RESET%%^ %s\n",
                      NETWORK_ID,sender,origin,data["text"]), who);
                  blmsg=sprintf("%s tell: %s@%s %s\n",
                    NETWORK_ID,sender,origin,data["text"]);
                  break;
              case 1: // reply
                  IMC2_MSG(sprintf("%s %%^CYAN%%^%s@%s%%^RESET%%^ replies to you: %s\n",
                      NETWORK_ID,sender,origin,data["text"]), who);		  blmsg=sprintf("%s %s@%s replied to you: %s\n",
                    NETWORK_ID,sender,origin,data["text"]);
                  break;
              default:
                  IMC2_MSG(sprintf("%s %%^CYAN%%^%s@%s%%^RESET%%^ tells you: %s\n",
                      NETWORK_ID,sender,origin,data["text"]), who);
                  blmsg=sprintf("%s %s@%s told you: %s\n",
                    NETWORK_ID,sender,origin,data["text"]);
                  break;
              }
              if(!tells[target])
                  tells[target]=([ ]);
              tells[target]["reply"] = sprintf("%s@%s",sender,origin);
              if(!tells[target]["backlog"] || !arrayp(tells[target]["backlog"]))
                  tells[target]["backlog"]=({ });
              tells[target]["backlog"] += ({ blmsg });
              sz = sizeof(tells[target]["backlog"]);
              if(sz>BACKLOG_SIZE)
                  tells[target]["backlog"]=tells[target]["backlog"][(sz-BACKLOG_SIZE)..sz];
          }
          else{
#ifdef TELL_BOTS
              // Can have a mapping of bots which can get tells.
              if(TELL_BOTS[target]){
                  call_other(TELL_BOTS[target],"got_tell",sender, origin, target, data["text"]);
                  return;
              }
#endif
#ifdef TELL_BOT
              // They should have got_tell(fromname, frommud, target, text)
              // I'll assume bots don't care about emote/reply... tell me if you do?
              // TELL_BOT will get told of all tells which don't have a valid target, and
              // then the sender will be notified that it didn't get to anyone.
              // This would be useful if for example you wanted all tells sent to
              // offline people to be mudmailed to them or something.
              call_other(TELL_BOTS[target],"got_tell",sender, origin, target, data["text"]);
#endif
              send_packet("*","tell",sender,origin,
                sprintf("level=-1 text=\"%s is not online on this mud.\" isreply=1",target));
          }
      }

      string pinkfish_to_imc2(string str){
          // Foreground
          str=replace_string(str,"%^BLACK%^","~x"); // Black
          str=replace_string(str,"%^RED%^","~R"); // Red
          str=replace_string(str,"%^GREEN%^","~G"); // Green
          str=replace_string(str,"%^BLUE%^","~B"); // Blue
          str=replace_string(str,"%^WHITE%^","~W"); // White
          str=replace_string(str,"%^ORANGE%^","~y"); // Orange
          str=replace_string(str,"%^CYAN%^","~c"); // Cyan
          str=replace_string(str,"%^YELLOW%^","~Y"); // Yellow
          str=replace_string(str,"%^MAGENTA%^","~p"); // Magenta -> purple sounds closest
          str=replace_string(str,"%^GRAY%^","~w"); // Gray doesn't display on my MUD, bah :(
          // Background
          str=replace_string(str,"%^B_BLACK%^","^x");
          str=replace_string(str,"%^B_RED%^","^R"); // Red
          str=replace_string(str,"%^B_GREEN%^","^G"); // Green
          str=replace_string(str,"%^B_BLUE%^","^b"); // Blue
          str=replace_string(str,"%^B_WHITE%^","^W"); // White
          str=replace_string(str,"%^B_ORANGE%^","^O"); // Orange
          str=replace_string(str,"%^B_CYAN%^","^c"); // Cyan
          str=replace_string(str,"%^B_YELLOW%^","^Y"); // Yellow
          str=replace_string(str,"%^B_MAGENTA%^","^p"); // Magenta -> purple sounds closest
          // Misc.
          str=replace_string(str,"%^FLASH%^","~$"); // Flash -> Blink
          str=replace_string(str,"%^BOLD%^","~L"); // Bold
          str=replace_string(str,"%^RESET%^","~!");
          return str;
      }

      string imc2_to_pinkfish(string str){
          string output="";
          int i,sz;
          /*
          For colors explanation, refer to IMC Packet Documentation by Xorith.
          Thanks very much for putting that out, by the way. :)
          Found at http://hub00.muddomain.com/imc2_protocol_doc.txt
          */
          sz=sizeof(str)-1;
          while(sizeof(str)>1){
              switch(str[0]){
              case '~': // Foreground
                  switch(str[1]){
                  case 'Z': break; // Random
                  case 'x': output += "%^BLACK%^"; break; // Black
                  case 'r': output += "%^RED%^"; break; // Dark Red
                  case 'g': output += "%^GREEN%^"; break; // Dark Green
                  case 'y': output += "%^ORANGE%^"; break; // Orange
                  case 'b': output += "%^BLUE%^"; break; // Dark Blue
                  case 'p': output += "%^MAGENTA%^"; break; // Purple
                  case 'c': output += "%^CYAN%^"; break; // Cyan
                  case 'w': output += "%^WHITE%^"; break; // Grey
                  case 'D': output += "%^BLACK%^"; break; // Dark Grey
                  case 'z': output += "%^BLACK%^"; break; // Same as ~D
                  case 'R': output += "%^RED%^"; break; // Red
                  case 'G': output += "%^GREEN%^"; break; // Green
                  case 'Y': output += "%^YELLOW%^"; break; // Yellow
                  case 'B': output += "%^BLUE%^"; break; // Blue
                  case 'P': output += "%^MAGENTA%^"; break; // Pink
                  case 'C': output += "%^BLUE%^"; break; // Light Blue
                  case 'W': output += "%^WHITE%^"; break; // White

                  case 'm': output += "%^MAGENTA%^"; break; // same as p
                  case 'd': output += "%^WHITE%^"; break; // same as w
                  case 'M': output += "%^MAGENTA%^"; break; // same as P
                      // Misc.
                  case '!': output += "%^RESET%^"; break; // Reset
                  case 'L': output += "%^BOLD%^"; break; // Bold
                  case 'u': break; // Underline
                  case '$': output += "%^FLASH%^"; break; // Blink
                  case 'i': break; // Italic
                  case 'v': break; // Reverse
                  case 's': break; // Strike-thru

                  case '~': output += "~"; break; // ~~ prints as ~
                  default : output += "~"; // Don't skip over this
                      // (cheap hack is to add a character in front so the [2..] thing still works)
                      str = " "+str;
                      break;
                  }
                  str=str[2..];
                  break;
              case '^':  // Background
                  switch(str[1]){
                  case 'Z': break; // Random
                  case 'x': output += "%^B_BLACK%^"; break; // Black
                  case 'r': output += "%^B_RED%^"; break; // Dark Red
                  case 'g': output += "%^B_GREEN%^"; break; // Dark Green
                  case 'O': output += "%^B_ORANGE%^"; break; // Orange
                  case 'B': output += "%^B_BLUE%^"; break; // Dark Blue
                  case 'p': output += "%^B_MAGENTA%^"; break; // Purple
                  case 'c': output += "%^B_CYAN%^"; break; // Cyan
                  case 'w': output += "%^B_WHITE%^"; break; // Grey
                  case 'z': output += "%^B_BLACK%^"; break; // Dark Grey
                  case 'R': output += "%^B_RED%^"; break; // Red
                  case 'G': output += "%^B_GREEN%^"; break; // Green
                  case 'Y': output += "%^B_YELLOW%^"; break; // Yellow
                  case 'b': output += "%^B_BLUE%^"; break; // Blue
                  case 'P': output += "%^B_MAGENTA%^"; break; // Pink
                  case 'C': output += "%^B_BLUE%^"; break; // Light Blue
                  case 'W': output += "%^B_WHITE%^"; break; // White
                  case '^': output += "^"; break; // ^^ prints as ^
                  default : output += "^"; // Don't skip over this
                      // (cheap hack is to add a character in front so the [2..] thing still works)
                      str = " "+str;
                      break;
                  }
                  str=str[2..];
                  break;
              case '`': // Blinking Foreground
                  switch(str[1]){
                  case 'Z': output += "%^FLASH%^"; break; // Random
                  case 'x': output += "%^FLASH%^%^BLACK%^"; break; // Black
                  case 'r': output += "%^FLASH%^%^RED%^"; break; // Dark Red
                  case 'g': output += "%^FLASH%^%^GREEN%^"; break; // Dark Green
                  case 'O': output += "%^FLASH%^%^ORANGE%^"; break; // Orange
                  case 'b': output += "%^FLASH%^%^BLUE%^"; break; // Dark Blue
                  case 'p': output += "%^FLASH%^%^MAGENTA%^"; break; // Purple
                  case 'c': output += "%^FLASH%^%^CYAN%^"; break; // Cyan
                  case 'w': output += "%^FLASH%^%^WHITE%^"; break; // Grey
                  case 'z': output += "%^FLASH%^%^BLACK%^"; break; // Dark Grey
                  case 'R': output += "%^FLASH%^%^RED%^"; break; // Red
                  case 'G': output += "%^FLASH%^%^GREEN%^"; break; // Green
                  case 'Y': output += "%^FLASH%^%^YELLOW%^"; break; // Yellow
                  case 'B': output += "%^FLASH%^%^BLUE%^"; break; // Blue
                  case 'P': output += "%^FLASH%^%^MAGENTA%^"; break; // Pink
                  case 'C': output += "%^FLASH%^%^BLUE%^"; break; // Light Blue
                  case 'W': output += "%^FLASH%^%^WHITE%^"; break; // White
                  case '`': output += "`"; break; // `` prints as `
                  default : output += "`"; // Don't skip over this
                      // (cheap hack is to add a character in front so the [2..] thing still works)
                      str = " "+str;
                      break;
                  }
                  str=str[2..];
                  break;
              default:
                  output += str[0..0];
                  str=str[1..];
                  break;
              }
          }
          output += str;
          return output;
      }

      private void who_reply_in(string origin, string target, mapping data){
          string output;
          object targuser;
          targuser = FIND_PLAYER(lower_case(target));
          if(targuser){
              output = NETWORK_ID+" who reply from: %^CYAN%^"+origin+"%^RESET%^\n";
              output += imc2_to_pinkfish(data["text"])+"\n";
              IMC2_MSG(output,targuser);
          }
      }

      private void whois_in(string fromname, string frommud, string targ, mapping data){
          if(FIND_PLAYER(lower_case(targ)) 
#ifdef INVIS
            && !INVIS(FIND_PLAYER(lower_case(targ)))
#endif
            && can_use(FIND_PLAYER(lower_case(targ)))
          ){
              send_packet(targ,"whois-reply",fromname,frommud,"text=Online");
          }
#ifdef USER_EXISTS
          else if(USER_EXISTS(lower_case(targ))){
              send_packet(targ,"whois-reply",fromname,frommud,
                "text=\"Exists but is offline\"");
          }
#endif
      }

      private void whois_reply_in(string targ,string fromname,string frommud,mapping data){
          object who;
          who=FIND_PLAYER(lower_case(targ));
          if(who){
              IMC2_MSG(sprintf("%s whois reply: %s@%s: %s\n",
                  NETWORK_ID,fromname,frommud,data["text"]),who);
          }
      }

      private void beep_in(string sender, string origin, string target, mapping data){
          object who;
          who=FIND_PLAYER(lower_case(target));
          if(who && can_use(who)
#ifdef INVIS
            && VISIBLE(who)
#endif
          ){
              IMC2_MSG(sprintf("%s- %%^CYAN%%^%s@%s%%^RESET%%^ \abeeps you.\n",
                  NETWORK_ID,sender,origin,data["text"]), who);
          }
          else{
              send_packet("*","tell",sender,origin,
                sprintf("level=-1 text=\"%s is not online.\" isreply=1",target));
          }
      }

      private void beep_out(object from, string targname, string targmud){
          send_packet(GET_CAP_NAME(from),"beep",targname,targmud,
            sprintf("level=%d ", level(from)));
      }

      private void send_ice_refresh(){ send_packet("*","ice-refresh","*","*",""); }

      private void ping_reply_in(string sender,string origin,string target,mapping data){
          object who;
          if((target=="*") && target=ping_requests[sender]){
              target=ping_requests[sender];
              map_delete(ping_requests,sender);
          }
          who=FIND_PLAYER(lower_case(target));
          if(who){
              IMC2_MSG(sprintf("%s route to %%^CYAN%%^%s%%^RESET%%^ is: %s\n",
                  NETWORK_ID,origin,data["path"]), who);
          }
      }

      private void ping_out(string from,string targmud){ send_packet(from,"ping","*",targmud,""); }
      private varargs void who_out(string from,string targmud,string type){ send_packet(from,"who","*",targmud,(type ? sprintf("type=\"%s\"",type) : "type=who")); }

      private void chanwho_out(object from,string chan,string mud){
          send_packet(GET_CAP_NAME(from),"ice-chan-who","*",mud,
            sprintf("level=%d channel=%s lname=%s",level(from),
              localchaninfo[chan]["name"],chan));
      }

      private void chanwho_reply_in(string origin, string target, mapping data){
          string output;
          object targuser;
          targuser = FIND_PLAYER(lower_case(target));
          if(targuser){
              output = NETWORK_ID+" chanwho reply from "+origin+" for "+data["channel"]+"\n";
              output += imc2_to_pinkfish(data["list"]);
              IMC2_MSG(output,targuser);
          }
      }

      private void chan_who_in(string fromname, string frommud, mapping data){
          // Handles an incoming channel who request.
          object user, *usrs=({ });
          string local, lname;
          string output;
          local=localize_channel(data["channel"]);
          if(data["lname"]) lname = data["lname"];
          else lname = local;
          if(!local){ // Channel not used locally.
              output = sprintf("channel=%s list=\"%s (%s) is not configured on this MUD.\n\"",
                data["channel"],lname,data["channel"]);
          }
          else{ // Is used locally
              output = "The following users are listening to "+lname+" ("+local+"):\n  ";
              foreach(user in users()){
                  if(chan_listening(user,local)
#ifdef INVIS
                    && !INVIS(user)
#endif
                  ){
                      usrs += ({ GET_CAP_NAME(user) });
                  }
              } // foreach
              output += implode(usrs,", ");
              output += "\n";
              if(!sizeof(usrs)){
                  output=sprintf("channel=%s list=\"Nobody is listening to %s (%s) on this MUD.\n\"",
                    lname,local);
              }
          }
          send_packet("*","ice-chan-whoreply",fromname,frommud,output);
      }

      string find_mud(string str){
          // Makes case-insensitive mud name into the actual mud name, or else 0.
          string mud;
          if(!str) return 0;
          str=lower_case(str);
          foreach(mud in keys(mudinfo)){
              if(lower_case(mud)==str) return mud;
          }
          return 0;
      }

      string localize_channel(string str){
          // Tells what the local name for a channel is, given the network name for it, or else 0.
          string a;
          foreach(a in keys(localchaninfo)){
              if(lower_case(localchaninfo[a]["name"])==lower_case(str)) return a;
          }
          return 0;
      }

      int command(string str){
          // Takes the arguments given to the command which does IMC2 stuff.
          string cmd, args;
          string mud, *muds;
          string output;
          string a,b,c;
          int x,y,z;
          int emote,reply;
          object usr, *usrs=({ });
          if(!str) str = "help";
          sscanf(str,"%s %s",cmd,args);
          if(!cmd) cmd=str;
          if(!args) args = "";
          switch(cmd){
          case "list":
              muds = sort_array(keys(mudinfo),1);
              if (!sizeof(mudinfo)){
                  return notify_fail("There are no muds on the "+NETWORK_ID+" network!\n");
              }
              else{
                  x=0; y=0;
                  output=sprintf("[%s] %-20s %-20s %-20s\n","U/D?","Name","Network","IMC2 Version");
                  foreach (mud in muds){
                      if(!mudinfo[mud]) output += "Error on mud: "+mud+"\n"; 
                      else{
                          if(mudinfo[mud]["online"])
                              x++; else y++;
                          output += sprintf("[%s] %-20s %-20s %-20s\n",
                            (mudinfo[mud]["online"] ? "%^GREEN%^ UP %^RESET%^" : "%^RED%^DOWN%^RESET%^"),
                            mud,
                            mudinfo[mud]["networkname"],
                            mudinfo[mud]["versionid"]);
                      }
                  }
                  output += sprintf("%d of %d MUDs are online.\n",x,x+y);
                  IMC2_MSG(output,THIS_PLAYER);
                  return 1;
              }
              break;
          case "setup":
              output=sprintf(@EndText
IMC2 NETWORK INFORMATION
------------------------
Status: %s
Hub address: %s
Hub port: %d
The hub calls itself: %s
The network calls itself: %s
Command to use this network: %s
The MUD calls this connection: %s

Packet logging: %s

The network calls the MUD: %s
The MUD's Version ID: %s
The MUD's URL: %s
EndText,
                ((mode==MODE_CONNECTED) ? "Connected" : "Not connected"),
#ifdef HOSTNAME
                HOSTNAME,
#else
                HOSTIP,
#endif
                HOSTPORT,hub_name,network_name,COMMAND_NAME,NETWORK_ID,
#ifdef DATA_LOG
                "on",
#else
                "off",
#endif
                MUDNAME,VERSION,URL);
              IMC2_MSG(output,THIS_PLAYER);
              return 1;
              break;
          case "info":
              if(!args) return notify_fail("See info for which MUD?\n");
              str=find_mud(args);
              if(!str) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              output=("Mud info for: "+str+"\nStatus: ");
              if(mudinfo[str]["online"])
                  output+=("%^GREEN%^Online%^RESET%^\n");
              else
                  output+=("%^RED%^Offline%^RESET%^\n");
              if(mudinfo[str]["versionid"]) output+=("Version ID: "+mudinfo[str]["versionid"]+"\n");
              if(mudinfo[str]["url"]) output+=("URL: "+mudinfo[str]["url"]+"\n");
              if(mudinfo[str]["networkname"]) output+=("Network name: "+mudinfo[str]["networkname"]+"\n");
              IMC2_MSG(output,THIS_PLAYER);
              return 1;
              break;
          case "ping":
              if(!args) return notify_fail("Send ping to which MUD?\n");
              str=find_mud(args);
              if(!str) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!mudinfo[str]["online"]) return notify_fail(NETWORK_ID+"- "+b+" is offline right now.\n");
              ping_out(GET_CAP_NAME(THIS_PLAYER),str);
              IMC2_MSG(NETWORK_ID+"- Sent a ping to "+str+"\n",THIS_PLAYER);
              return 1;
              break;
          case "who":
              if(!args) return notify_fail("Send who request to which MUD?\n");
              str=find_mud(args);
              if(!str) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!mudinfo[str]["online"]) return notify_fail(NETWORK_ID+"- "+str+" is offline right now.\n");
              who_out(GET_CAP_NAME(THIS_PLAYER),str);
              IMC2_MSG(NETWORK_ID"- Sent a who request to "+str+"\n",THIS_PLAYER);
              return 1;
              break;
          case "finger":
              notify_fail("Send finger request to who@where?\n");
              if(!args) return 0;
              if(sscanf(args,"%s@%s",a,b)!=2){
                  return 0;
              }
              b=find_mud(b);
              who_out(GET_CAP_NAME(THIS_PLAYER),b,"finger "+a);
              IMC2_MSG(NETWORK_ID"- Sent a finger request to "+a+"@"+b+"\n",THIS_PLAYER);
              return 1;
              break;
          case "mudinfo":
              if(!args) return notify_fail("Send info request to which MUD?\n");
              str=find_mud(args);
              if(!str) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!mudinfo[str]["online"]) return notify_fail(NETWORK_ID+"- "+str+" is offline right now.\n");
              who_out(GET_CAP_NAME(THIS_PLAYER),str,"info");
              IMC2_MSG(NETWORK_ID"- Sent an info request to "+str+"\n",THIS_PLAYER);
              return 1;
              break;
          case "chanwho":
              if(!args) return notify_fail("What channel?\n");
              if(sscanf(args,"%s %s",a,b)!=2){
                  a=args;
                  b="";
              }
              else {
                  if(b!="*") b=find_mud(b);
              }
              if(!b) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!localchaninfo[a]) return notify_fail("Invalid channel.\n");
              if(b==""){ // check who's on locally
                  c = "The following users are listening to "+a+":\n  ";
                  foreach(usr in users())
                      if(chan_listening(usr,a)){
                          usrs += ({ usr });
                          c += " "+GET_CAP_NAME(usr);
                      }
                  c += "\n";
                  if(!usrs) c="Nobody on this mud is listening to that channel.\n";
                  IMC2_MSG(c,THIS_PLAYER);
                  return 1;
              }
              if((b!="*") && (!mudinfo[b]["online"]))
                  return notify_fail(NETWORK_ID+"- "+b+" is offline right now.\n");
              chanwho_out(THIS_PLAYER,a,b);
              IMC2_MSG(sprintf("%s- Sent a channel who request to %s for the %s channel.\n",
                  NETWORK_ID,b,a),THIS_PLAYER);
              return 1;
              break;
          case "chans":
              output="Local setup for "+NETWORK_ID+" channels:\n";
              output += sprintf("%-15s %-15s %-15s %-15s\n",
                "Local Name","Allowed (you?)","Listening","Network Name");  
              foreach(a in sort_array(keys(localchaninfo),1)){
                  output += sprintf("%-15s %s%-15s%%^RESET%%^ %s%-15s%%^RESET%%^ %-15s\n",
                    a,
                    ((chan_perm_allowed(THIS_PLAYER,a) ? "%^GREEN%^" : "%^RED%^")),
                    chan_perm_desc(localchaninfo[a]["perm"])+" "+
                    ((chan_perm_allowed(THIS_PLAYER,a) ? "(yes)" : "(no)")),
                    ((chan_listening(THIS_PLAYER,a) ? "%^GREEN%^" : "%^RED%^")),
                    ((chan_listening(THIS_PLAYER,a) ? "yes" : "no")),
                    localchaninfo[a]["name"]);
              }
              IMC2_MSG(output,THIS_PLAYER);
              return 1;
              break;
          case "allchans":
              output=NETWORK_ID+" channels:\n";
              output += sprintf("%-23s %-17s %-7s %-6s %-10s %-10s\n",
                "Name","Owner","Policy","Level","Suggested","Local Name");  
              foreach(a in sort_array(keys(chaninfo),1)){
                  output += sprintf("%-23s %-17s %-7s %-6s %-10s %-10s\n",
                    a,chaninfo[a]["owner"],
                    chaninfo[a]["policy"],
                    chaninfo[a]["level"],chaninfo[a]["localname"],
                    (localize_channel(a) ? localize_channel(a) : "<none>"));
              }
              IMC2_MSG(output,THIS_PLAYER);
              return 1;
              break;
          case "beep":
              if(!args || sscanf(args,"%s@%s",a,b)!=2)
                  return notify_fail("Invalid syntax. "+COMMAND_NAME+" beep name@mud\n");
              b=find_mud(b);
              if(!b) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!mudinfo[b]["online"]) return notify_fail("That MUD is offline right now\n");
              IMC2_MSG(sprintf("%s- You beep %s@%s\n",NETWORK_ID,a,b),THIS_PLAYER);
              beep_out(THIS_PLAYER,a,b);
              return 1;
              break;
          case "chanemote": // Drop through
          case "chatemote": // Drop through
          case "chansocial": // Drop through
          case "chatsocial": // Drop through
          case "chat": // Drop through
          case "chan":
              switch(cmd){
              case "chatemote" : // Drop-through
              case "chanemote" : emote=1; break;
              case "chatsocial" : // Drop-through
              case "chansocial" : emote=2; break;
              }
              if(!args || (sscanf(args,"%s %s",a,b)!=2))
                  return notify_fail("Syntax: "+COMMAND_NAME+" chan channel message\n");
              if(!localchaninfo[a]) return notify_fail(a+" is not a channel.\n");
              if(!chan_perm_allowed(THIS_PLAYER,a))
                  return notify_fail("You need permission to use the "+a+" channel.\n");
              if(!chan_listening(THIS_PLAYER,a))
                  return notify_fail("You are not listening to the "+a+" channel.\n");
              if(emote==2){
                  IMC2_MSG(sprintf("Sent to %s- [%s] %s\n",NETWORK_ID,
                      a,b),THIS_PLAYER);
              }
              else if(emote){
                  IMC2_MSG(sprintf("Sent to %s- [%s] %s %s\n",NETWORK_ID,
                      a,GET_CAP_NAME(THIS_PLAYER),b),THIS_PLAYER);
              }
              else{
                  IMC2_MSG(sprintf("Sent to %s- %s [%s] %s\n",NETWORK_ID,
                      GET_CAP_NAME(THIS_PLAYER),a,b),THIS_PLAYER);
              }
              send_packet(GET_CAP_NAME(THIS_PLAYER),"ice-msg-b","*","*",
                sprintf("channel=%s text=%s emote=%d echo=1",
                  localchaninfo[a]["name"],escape(pinkfish_to_imc2(b)),emote));
              return 1;
              break;			
          case "reply": // Drop-through
          case "replyemote": // Drop-through
          case "tellemote": // Drop-through
          case "tell": // Drop-through
              switch(cmd){
              case "reply" : cmd="reply to"; reply=1; break;
              case "replyemote" : cmd="reply an emote to"; reply=1; emote=1; break;
              case "tell" : break;
              case "tellemote" : cmd="tell an emote to"; emote=1; break;
              }
              if(!args)
                  return notify_fail("Need a message.\n");
              if(reply){
                  a=tells[GET_CAP_NAME(THIS_PLAYER)]["reply"];
                  if(!a) return notify_fail("You haven't recieved an incoming tell through "+
                        NETWORK_ID+" yet.  Can't reply.\n");
                  sscanf(a,"%s@%s",a,b);
                  c=args;
              }
              else{
                  if(sscanf(args,"%s@%s %s",a,b,c)!=3)
                      return notify_fail("Invalid syntax.  "+COMMAND_NAME+" tell name@mud message\n");
              }
              if(emote)
                  c=GET_CAP_NAME(THIS_PLAYER)+"@"+MUDNAME+" "+c;
              b=find_mud(b);
              if(!b) return notify_fail("MUD isn't known on "+NETWORK_ID+".\n");
              if(!mudinfo[b]["online"]) return notify_fail(NETWORK_ID+"- "+b+" is offline right now.\n");
              IMC2_MSG(sprintf("%s- You %s %s@%s: %s\n",NETWORK_ID,cmd,a,b,c),THIS_PLAYER);
              tell_out(THIS_PLAYER,a,b,c,reply,emote);
              return 1;
              break;
          case "listen":
              if(!args || !sizeof(args)){ // Listen to all
                  foreach(args in keys(localchaninfo)){
                      if(chan_perm_allowed(THIS_PLAYER,args) &&
                        (member_array(GET_NAME(THIS_PLAYER),localchaninfo[args]["users"])==-1)){
                          localchaninfo[args]["users"] += ({ GET_NAME(THIS_PLAYER) });
                          IMC2_MSG(NETWORK_ID+"- You are now listening to "+
                            args+"\n",THIS_PLAYER);
                      }
                  }
                  IMC2_MSG("Listened to all allowed "+NETWORK_ID+" channels.\n",THIS_PLAYER);
                  return 1;
              }
              if(!localchaninfo[args]) return notify_fail("Not a locally configured channel.\n");
              if(!chan_perm_allowed(THIS_PLAYER,args))
                  return notify_fail("You don't have permission to use that channel.\n");
              if(member_array(THIS_PLAYER->query("name"),localchaninfo[args]["users"])!=-1)
                  return notify_fail("You're already listening to that channel.\n");
              localchaninfo[args]["users"] += ({ GET_NAME(THIS_PLAYER) });
              IMC2_MSG(NETWORK_ID+"- You are now listening to "+args+"\n",THIS_PLAYER);
              return 1;
              break;
          case "unlisten":
              if(!localchaninfo[args]) return notify_fail("Not a locally configured channel.\n");
              localchaninfo[args]["users"] -= ({ GET_NAME(THIS_PLAYER) });
              IMC2_MSG(NETWORK_ID+"- You are no longer listening to "+args+"\n",THIS_PLAYER);
              return 1;
              break;
          case "whois":
              if(!args || !sizeof(args)) return notify_fail("Locate who?\n");
              send_packet(GET_CAP_NAME(THIS_PLAYER),"whois",args,"*",
                sprintf("level=%d ",level(THIS_PLAYER)));
              IMC2_MSG("Sent a request on "+NETWORK_ID+" looking for "+args+"\n",THIS_PLAYER);
              return 1;
              break;
          case "backlog":
              if(!localchaninfo[args]) return notify_fail("Not a locally configured channel.\n");
              if(!chan_perm_allowed(THIS_PLAYER,args))
                  return notify_fail("You need permission to use the "+args+" channel.\n");
              if(!chan_listening(THIS_PLAYER,args))
                  return notify_fail("You are not listening to the "+args+" channel.\n");
              IMC2_MSG(sprintf("Backlog of channel %s on %s:\n%s",args,NETWORK_ID,
                  implode(localchaninfo[args]["backlog"],"\n")+"\n"),THIS_PLAYER);
              return 1;
              break;
          case "tells":
              a=GET_CAP_NAME(THIS_PLAYER);
              if(!tells[a] || !tells[a]["backlog"])
                  return notify_fail("You have received no tells on the "+NETWORK_ID+" network yet.\n");
              IMC2_MSG(sprintf("Tell backlog on %s:\n%s",NETWORK_ID,
                  implode(tells[a]["backlog"],"")+"\n"),THIS_PLAYER);
              return 1;
              break;
          case "configchan":
              if(!ADMIN(THIS_PLAYER)) return notify_fail("You aren't allowed to use configchan.\n");
              if(sscanf(args,"%s %s %d",a,b,x)!=3)
                  return notify_fail("Syntax: "+COMMAND_NAME+" configchan local_name hub_name level_number\n");
              if(member_array(b,keys(chaninfo))==-1)
                  return notify_fail("The "+b+" channel isn't on the network.\n");
              c=localize_channel(b);
              if(c) return notify_fail("That channel is already configured as "+c+" locally.\n");
              localchaninfo[a]=([ "perm":x, "name":b, "users":({ }), "backlog":({ }) ]);
              IMC2_MSG("Added the "+a+" channel for "+chan_perm_desc(x)+
                " to use, which is "+b+" on the network.\n",THIS_PLAYER);
              return 1;
              break;
          case "unconfigchan":
              if(!ADMIN(THIS_PLAYER)) return notify_fail("You aren't allowed to use unconfigchan.\n");
              map_delete(localchaninfo,args);
              IMC2_MSG("Got rid of "+args+" channel.\n",THIS_PLAYER);
              return 1;
              break;
          case "chancmd":
              if(!ADMIN(THIS_PLAYER)) return notify_fail("You aren't allowed to use chancmd.\n");
              if(!args || (sscanf(args,"%s:%s %s",a,b,c)!=3))
                  return notify_fail("Syntax: "+COMMAND_NAME+" chancmd hub:channel command\n");
              if(!chaninfo[a+":"+b]) write(a+" is not listed as a channel, sending command anyway...\n");
              send_packet(GET_CAP_NAME(THIS_PLAYER),"ice-cmd","IMC",b,
                sprintf("channel=%s command=\"%s\"",
                  a+":"+b,escape(c)));
              IMC2_MSG(sprintf("%s- Sent a command for the %s channel to %s\n",NETWORK_ID,
                  a,GET_CAP_NAME(THIS_PLAYER),b),THIS_PLAYER);
              return 1;
              break;
          case "chanperms":
              output = MUDNAME+" will handle the following levels for channels:\n";
              output += sprintf("%-10s  %-10s\n", "Number", "Description");
              for(x=2;x>0;x--){
                  output = sprintf("%-10d  %-10s\n", x, chan_perm_desc(x));
              }
              IMC2_MSG(output,THIS_PLAYER);
              return 1;
          case "help": // drop through to default
          default:
              IMC2_MSG(main_help(),THIS_PLAYER);
              return 1;
              break;
          }
      }

      string main_help(){
          return sprintf(@EndText
IMC2 system by Tim, set up for %s.
To use this, type the command '%s' followed by one of the following:
	chans - lists the channels that this MUD uses
	allchans - lists all the channels on this network
	chan (channel) (message) - talks on a channel
	chanemote (channel) (message) - emotes on a channel
	chansocial (channel) (message) - does a social on a channel
	chanwho (channel) (mud) - checks who is listening to a channel
	backlog (channel) - reads last %d messages on the channel
	listen (channel) - listen to a channel
	unlisten (channel) - stop listening to a channel
	info (name) - lists information about a MUD
	list - lists the MUDs on this network
	beep (name)@(mud) - send a beep through the network
	tell (name)@(mud) (message) - send a tell through the network
	tellemote (name)@(mud) (message) - send an emote through the network
	tells - shows your last %d tells
	finger (name)@(mud) - send a finger request for information about name@mud
	reply (message) - reply to the last incoming tell you received
	replyemote (message) - reply with an emote to the last incoming tell you received
	ping (mud) - pings a mud
	setup - shows information about this IMC2 network
	help - see this help message
Admin commands:
	configchan (local_name) (remote_name) (level_number) - configures a channel locally
	unconfigchan (local_name) - removes a locally configured channel
	chancmd (channel) (command) - for remote channel administration
	chanperms - lists the permission levels that are possible for configchan
EndText, NETWORK_ID,COMMAND_NAME,BACKLOG_SIZE,BACKLOG_SIZE);
      }

      string html(string str){
          string mud, *muds;
          string a,b;
          int x=0,y=0;
          string output="";
          if(!str) str="";
          sscanf(str,"%s_%s",a,b);
          if(!a) a=str;
          switch(a){
          case "list":
              muds = sort_array(keys(mudinfo),1);
              if (!sizeof(mudinfo)){
                  return ("There are no muds on the "+NETWORK_ID+" network!\n");
              }
              else{
                  output="<tr><td><b>Status</b></td><td><b>Name</b></td><td><b>Network</b></td><td><b>IMC2 Version</b></td></tr>\n";
                  foreach (mud in muds){
                      if(mudinfo[mud]["online"]) x++; else y++;
                      output += sprintf("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",
                        (mudinfo[mud]["online"] ? "UP" : "DOWN"),
                        mudinfo[mud]["url"] ? "<a href=\""+
                        ((mudinfo[mud]["url"][0..6]!="http://") ? "http://"+mudinfo[mud]["url"] : mudinfo[mud]["url"])
                        +"\">"+mud+"</a>" : mud,
                        mudinfo[mud]["networkname"],
                        mudinfo[mud]["versionid"]);
                  }
              }
              return sprintf("<B>%d of %d MUDs are on %s.</B.\n<BR><table>%s</table>",
                x,x+y,NETWORK_ID,output);
              break;
          case "backlog":
              if(b && localchaninfo[b] && (localchaninfo[b]["perm"]==BACKLOG_WEB_LEVEL)){
                  // Show backlog for channel.
                  return b+" channel backlog:\n"+implode(localchaninfo[b]["backlog"],"\n");
              }
              // List the channels
              output = NETWORK_ID+" channels on "+MUDNAME+":\n";
              foreach(b in sort_array(keys(localchaninfo),1)){
                  if(localchaninfo[b]["perm"]==BACKLOG_WEB_LEVEL) // is public
                      output += "<a href=\""+HTML_LOCATION+"backlog_"+b+"\">";
                  output += b;
                  output += " - "+chan_perm_desc(localchaninfo[b]["perm"]);
                  if(localchaninfo[b]["perm"]==BACKLOG_WEB_LEVEL) // is public
                      output += " - on web</a>";
                  else
                      output += " - not on web";
                  output += "\n";
              }
              return output;
              break;
          default:
              return MUDNAME+" uses Tim's LPC IMC2 system to connect to the "+
              NETWORK_ID+" network.\n"+
              "(version:"+VERSION+")\n"+
              "From here, you can look at the <a href=\""+HTML_LOCATION+"list\">list of muds</a>,\n"+
              "view the <a href=\""+HTML_LOCATION+"backlog\">public channel backlogs</a>,\n"+
              "or go to the main <a href=\""+URL+"\">"+MUDNAME+"</a> web site.\n";
              break;
          }
      }

      int clean_up(){ return 0; }


      void forget_user(string str){ map_delete(tells,str); }
