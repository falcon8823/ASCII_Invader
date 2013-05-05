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

void draw_wall(WALL *wall, WINDOW *win)
{
	wmove(win, WALL_Y, wall->x - 6);
	wprintw(win,"=========");

	wmove(win, WALL_Y + 1, wall->x - 6);
	wprintw(win, "=danger!=");

	wmove(win, WALL_Y + 2, wall->x - 6);
	wprintw(win, "=========");
}

void draw_enemy(ENEMY *enemy, int fx, int fy, WINDOW *win)
{

	wmove(win, ENEMY_Y_POS(enemy->y, fy), ENEMY_X_POS(enemy->x, fx));
	wprintw(win, character[fx % 2][enemy->type]);
}

void draw_bullet(BULLET *bullet,WINDOW *win)
{
	wmove(win, bullet->y, bullet->x);
	wprintw(win, "^");
}

