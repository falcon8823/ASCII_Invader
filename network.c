/* [ASCII Invader]
 * File: network.h
 * Description: ネットワーク通信関数ライブラリ
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
 */

#include "network.h"

// サーバとしてソケット通信を開始し，接続したソケットを返す
int init_server(in_port_t port) {
	struct sockaddr_in me;
	int waiting_soc;
	int session_soc;

	memset((char*)&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	me.sin_port = htons(port);
	
	// 待機ソケットの作成
	if((waiting_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	// バインド
	if(bind(waiting_soc, (struct sockaddr*) &me, sizeof(me)) == -1) {
		perror("bind");
		return -1;
	}

	// リッスン，待機
	listen(waiting_soc, 1);
	fprintf(stderr, "Waiting client...\n");
	
	// 接続
	session_soc = accept(waiting_soc, NULL, NULL);
	// 待機ソケットを閉じる
	close(waiting_soc);

	return session_soc;
}

// サーバとしてソケット通信を開始し，接続したソケットを返す
int init_client(char *hostname, in_port_t port) {
	struct hostent *srv_ent;
	struct sockaddr_in server;
	int session_soc;
	
	// ホスト名からアドレスを取得
	if((srv_ent = gethostbyname(hostname)) == NULL) {
		perror("gethostbyname");
		return -1;
	}

	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy((char*) &server.sin_addr, srv_ent->h_addr, srv_ent->h_length);
	
	// ソケットの作成
	if((session_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	
	// 接続要求
	if(connect(session_soc, (struct sockaddr *)&server, sizeof(server)) == -1) {
			perror("connect");
			return -1;
	}

	fprintf(stderr, "Successfully connected.\n");

	return session_soc;
}

char *nl_to_null(char *str, int len) {
	int n = strlen(str);

	if(n < len && str[n - 1] == '\n') {
		str[n - 1] = '\0';
	}

	return str;
}
