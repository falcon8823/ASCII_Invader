/* [ASCII Invader]
 * File: drawing.h
 * Description: キャラクター描画定義ヘッダー
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#define PLAYER_Y 27
#define WALL_Y 23
#define PLAYER_WIDTH 13
#define PLAYER_HEIGHT 3
#define WALL_WIDTH 9
#define WALL_HEIGHT 3
#define ENEMY_WIDTH 7
#define ENEMY_HEIGHT 1
#define ENEMY_FIELD_X 15
#define ENEMY_FIELD_Y 5
#define SPACE_X 4
#define SPACE_Y 1

void draw_player(PLAYER *player,WINDOW *win);
void draw_wall(WALL *wall,WINDOW *win);
void draw_enemy(ENEMY *enemy,WINDOW *win);
void draw_bullet(BULLET *bullet,WINDOW *win);

