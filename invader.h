/* [ASCII Invader]
 * File: invader.h
 * Description: オブジェクト構造体定義ヘッダー
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

// 2次のベクトル構造体 VECT
typedef struct {
	int x;
	int y;
} VECT;

// 座標型POS
typedef VECT POS;

//自機
typedef struct
{
    int x;
} PLAYER;


//相手
typedef struct
{
    int x;
    int y;
		int active;
    int type;//種類
} ENEMY;


//弾
typedef struct
{
    int active;
    int x;
    int y;
    int velocity;//速度
    int sorce;//現在の親
} BULLET;


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
    int x;
} WALL;

