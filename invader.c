/* [ASCII Invader]
 * File: invader.c
 * Description: メインプログラム（エントリポイント）
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#include "invader.h"
#include "game.h"

int main(int argc, char **argv) {
	game_init();
	game_loop();
	return 0;
}

