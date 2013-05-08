/* [ASCII Invader]
 * File: network.h
 * Description: ネットワーク通信関数ライブラリ
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

extern int init_server(in_port_t port);
extern int init_client(char *hostname, in_port_t port);
extern char *nl_to_null(char *str, int len);
