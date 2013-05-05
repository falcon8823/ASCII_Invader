/* [ASCII Invader]
 * File: drawing.c
 * Description: キャラクター描画コード
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#include <stdlib.h>
#include <curses.h>
#include "invader.h"
#include "drawing.h"


static char* character[2][9] = {
	{
		"\\(^p^)/",
		"\\(=3=)/",
		"\\(o\"o)/",
		"\\(OwO)/",
		"\\(-A-)/",
		"\\(@*@)/",
		"\\('=')/",
		"\\(DuD)/",
		"\\(pep)/"
	},
	{
		"/(-p-)\\",
		"/(=3=)\\",
		"/(o\"o)\\",
		"/(OwO)\\",
		"/(-A-)\\",
		"/(@*@)\\",
		"/('=')\\",
		"/(DuD)\\",
		"/(pep)\\"
	}
};

void draw_player(PLAYER *player, WINDOW *win) { 
	int x = player->pos.x, y = player->pos.y;

	wmove(win, y, x - 6);
	wprintw(win,"  ._/:::\\_.  ");

	wmove(win, y + 1, x - 6);
	wprintw(win," /|::(O)::|\\ ");

	wmove(win, y + 2, x - 6);
	wprintw(win,"/-l-------l-\\");
}

void draw_wall(WALL *wall, WINDOW *win) {
	wmove(win, wall->pos.y, wall->pos.x - 6);
	wprintw(win,"=========");

	wmove(win, wall->pos.y + 1, wall->pos.x - 6);
	wprintw(win, "=danger!=");

	wmove(win, wall->pos.y + 2, wall->pos.x - 6);
	wprintw(win, "=========");
}

void draw_enemy(ENEMY *enemy, POS *enemy_pos, WINDOW *win) {
	wmove(win, ENEMY_Y_POS(enemy->pos.y, enemy_pos->y), ENEMY_X_POS(enemy->pos.x, enemy_pos->x));
	wprintw(win, character[enemy_pos->x % 2][enemy->type]);
}

void draw_bullet(BULLET *bullet,WINDOW *win) {
	wmove(win, bullet->pos.y, bullet->pos.x);
	wprintw(win, "^");
}

