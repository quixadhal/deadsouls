#ifndef __NETWORK_H 
#define __NETWORK_H 
 
#include <socket.h> 
#include <config.h>
 
#define MUD                       0 
#define STREAM                    1 
#define DATAGRAM                  2 
#define STREAM_BINARY             3
#define DATAGRAM_BINARY           4
 
#define PORT_ADMIN                17010
#define PORT_FTP                  (query_host_port() - 1)
#define PORT_HTTP                 (query_host_port() - 5)
#define PORT_OOB                  (query_host_port() + 5)
#define PORT_RCP                  (query_host_port() - 10)
#define PORT_UDP                  (query_host_port() + 8) 
 
#endif /* __NETWORK_H */ 
