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

void draw_player(PLAYER *player,WINDOW *win)
{ 
    wmove(win,PLAYER_Y,player->x-6);
    wprintw(win,"  ._/:::\\_.  ");

    wmove(win,PLAYER_Y+1,player->x-6);
    wprintw(win," /|::(O)::|\\ ");

    wmove(win,PLAYER_Y+2,player->x-6);
    wprintw(win,"/-l-------l-\\");
}

void draw_wall(WALL *wall,WINDOW *win)
{
    wmove(win,WALL_Y,wall->x-6);
    wprintw(win,"=========");
    
    wmove(win,WALL_Y+1,wall->x-6);
    wprintw(win,"=danger!=");

    wmove(win,WALL_Y+2,wall->x-6);
    wprintw(win,"=========");
}

void draw_enemy(ENEMY *enemy,WINDOW *win)
{
    int i = enemy->x;
    int j = enemy->y;
    
    if(enemy->type==0){       
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(^p^)/");
    }
   else if(enemy->type==1){
       wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
       wprintw(win,"\\(=3=)/");
    }
    else if(enemy->type==2){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(o\"o)/");
    }     
    else if(enemy->type==3){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(OwO)/");
    }
    else if(enemy->type==4){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(-A-)/");
    }
    else if(enemy->type==5){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(@*@)/");
    }     
    else if(enemy->type==6){     
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\('=')/");
    }
    else if(enemy->type==7){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(DuD)/");
    }
    else if(enemy->type==8){
        wmove(win,ENEMY_FIELD_Y+j*(ENEMY_HEIGHT+SPACE_Y),ENEMY_FIELD_X+i*(ENEMY_WIDTH+SPACE_X));
        wprintw(win,"\\(pep)/");
    }
}

void draw_bullet(BULLET *bullet,WINDOW *win)
{
    wmove(win,bullet->y,bullet->x);
    wprintw(win,"#");
}

