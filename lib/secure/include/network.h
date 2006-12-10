#ifndef __NETWORK_H 
#define __NETWORK_H 

#include <socket.h> 
//#include <config.h>

#define MUD                       0 
#define STREAM                    1 
#define DATAGRAM                  2 
#define STREAM_BINARY             3
#define DATAGRAM_BINARY           4

#define OFFSET_FTP                -1
#define OFFSET_HTTP               -5
#define OFFSET_RCP                -10

#define PORT_ADMIN                17010
#define PORT_FTP                  (query_host_port() + OFFSET_FTP)
#define PORT_HTTP                 (query_host_port() + OFFSET_HTTP)
#define PORT_OOB                  (query_host_port() + 5)
#define PORT_RCP                  (query_host_port() + OFFSET_RCP)
#define PORT_UDP                  (query_host_port() + 8) 

//#define HOST_IP                   "71.234.154.99"
#define HOST_IP                   "192.168.0.5"

#endif /* __NETWORK_H */ 
