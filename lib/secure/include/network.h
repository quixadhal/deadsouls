#ifndef __NETWORK_H 
#define __NETWORK_H 

#include <socket.h> 

#define MUD                       0 
#define STREAM                    1 
#define DATAGRAM                  2 
#define STREAM_BINARY             3
#define DATAGRAM_BINARY           4

#define OFFSET_RCP                -10
#define OFFSET_HFTP               -6
#define OFFSET_HTTP               1
#define OFFSET_FTP                -1
#define OFFSET_ECHO               2
#define OFFSET_FLASH              3
#define OFFSET_OOB               6
#define OFFSET_ICP                6

#define PORT_ADMIN                17010
#define PORT_ECHO                 (query_host_port() + OFFSET_ECHO)
#define PORT_FLASH                (query_host_port() + OFFSET_FLASH)
#define PORT_FTP                  (query_host_port() + OFFSET_FTP)
#define PORT_HFTP                 (query_host_port() + OFFSET_HFTP)
#define PORT_HTTP                 (query_host_port() + OFFSET_HTTP)
#define PORT_ICP                  (query_host_port() + OFFSET_ICP)
#define PORT_OOB                  (query_host_port() + OFFSET_OOB)
#define PORT_RCP                  (query_host_port() + OFFSET_RCP)
#define PORT_UDP                  (query_host_port() + 8) 

#define MIN_PASV_PORT   (query_host_port() - 65)
#define MAX_PASV_PORT   (query_host_port() - 11)

#endif /* __NETWORK_H */ 
