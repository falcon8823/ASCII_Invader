/* [ASCII Invader]
 * File: invader.h
 * Description: オブジェクト構造体定義ヘッダー
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

// ウィンドウ
#define WIN_WIDTH  100
#define WIN_HEIGHT 30

// breakフラグの定数
#define BREAK 1
#define NON_BREAK 0

// キー定数
#define KEY_SPACE ' '
#define K_QUIT 'q'

// ゲーム要素定数
#define ENEMY_X_MAX 7
#define ENEMY_Y_MAX 7
#define ENEMY_NUM (ENEMY_X_MAX * ENEMY_Y_MAX)

#define ENEMY_MOVE_RATE 10

// 左右方向の単位ベクトル
#define D_LEFT -1
#define D_RIGHT 1

// 更新レート
#define FPS (clock_t)(CLOCKS_PER_SEC / 60)


// 2次のベクトル構造体 VECT
typedef struct {
	int x;
	int y;
} VECT;

// 座標型POS
typedef VECT POS;

//弾
typedef struct {
	POS pos;
	int active;
	int velocity;//速度
	int sorce;//現在の親
} BULLET;


//自機
typedef struct {
	POS pos;
	BULLET bullet; // 撃てる弾（1発）
} PLAYER;


//相手
typedef struct {
	POS pos;
	int active;
	int type; //種類
} ENEMY;


//UFO
typedef struct
{
	int x;
	int endure;
	int life;
} UFO;


//ガード（トーチカ）
typedef struct
{
	POS pos;
} WALL;

