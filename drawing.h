/* [ASCII Invader]
 * File: drawing.h
 * Description: キャラクター描画定義ヘッダー
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#define PLAYER_POS_X 50
#define PLAYER_POS_Y 27
#define WALL_Y 23
#define PLAYER_WIDTH 13
#define PLAYER_HEIGHT 3
#define WALL_WIDTH 9
#define WALL_HEIGHT 3

#define SPACE_X 4
#define SPACE_Y 1
#define ENEMY_WIDTH 7
#define ENEMY_HEIGHT 1
#define ENEMY_FIELD_X 15
#define ENEMY_FIELD_Y 5
#define ENEMY_X_POS(i, FIELD_X) (FIELD_X + i * (ENEMY_WIDTH + SPACE_X))
#define ENEMY_Y_POS(j, FIELD_Y) (FIELD_Y + j * (ENEMY_HEIGHT + SPACE_Y))

extern void draw_player(PLAYER *player, WINDOW *win);
extern void draw_wall(WALL *wall, WINDOW *win);
extern void draw_enemy(ENEMY *enemy, POS *enemy_pos, WINDOW *win);
extern void draw_bullet(BULLET *bullet, WINDOW *win);
extern void draw_score(PLAYER *player1, PLAYER *player2, WINDOW *win);
extern void draw_ending(PLAYER *player1, PLAYER *player2, WINDOW *win);


