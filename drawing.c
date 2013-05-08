/* [ASCII Invader]
 * File: drawing.c
 * Description: キャラクター描画コード
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

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


void draw_score(PLAYER *player1,PLAYER *player2, WINDOW *win) {
    int pos_y,pos_x;
    pos_y = 2;
    pos_x = 2;
    
    wmove(win,pos_y , pos_x);
    wprintw(win, "------------------------------------------------");
    
    wmove(win, pos_y + 1, pos_x);
    wprintw(win, "| YOU score:%5d",player1->score);
    
    wmove(win, pos_y + 1, pos_x + 26);
    wprintw(win, "CHARANGER score:%5d",player2->score);

    wmove(win, pos_y + 1, pos_x + 48);
    wprintw(win, "|");

    wmove(win, pos_y + 2, pos_x);
    wprintw(win,"------------------------------------------------");
}

void draw_ending(PLAYER *player1,PLAYER *player2,WINDOW *win)
{
    int pos_y,pos_x;
    pos_y = 16;
    pos_x = 51;
    
    wmove(win,pos_y - 4 , pos_x - 9);
    wprintw(win, "You're score:%5d",player1->score);

    wmove(win,pos_y - 2 , pos_x - 12);
    wprintw(win, "CHARANGER's  score:%5d",player2->score);

    if(player1->score > player2->score){
        wmove(win,pos_y, pos_x - 5);
        wprintw(win, "You WIN !!");
    }

    else if(player1->score == player2->score){
        wmove(win,pos_y, pos_x - 5);
        wprintw(win, "DRAW !");
    }

    else{
        wmove(win,pos_y, pos_x - 6);
        wprintw(win, "You LOSS ...");
    }
}


