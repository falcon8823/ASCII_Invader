/* [ASCII Invader]
 * File: invader.c
 * Description: メインプログラム（エントリポイント）
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
 */

#include <stdio.h>
#include "invader.h"
#include "game.h"


int main(int argc, char **argv) {
	int soc;
	int is_server;
	char hostname[HOSTNAME_LENGTH];
	char yn;

	printf("Are you host?(y or n): ");
	scanf("%[yn]%*c", &yn);

	if(yn == 'y') {
		// server
		if((soc = setup_server(PORT)) == -1) {
			exit(1);
		}
		is_server = TRUE;
	} else {
		// client
		printf("input server's hostname: ");
		fgets(hostname, HOSTNAME_LENGTH, stdin);
		chop_newline(hostname, HOSTNAME_LENGTH);

		if((soc = setup_client(hostname, PORT)) == -1) {
			exit(1);
		}
		is_server = FALSE;
	}

	game_init(soc, is_server);
	game_loop();

	return 0;
}

