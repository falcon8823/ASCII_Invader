/* [ASCII Invader]
 * File: invader.h
 * Description: オブジェクト構造体定義ヘッダー
 * Author: Yuta KOBAYASHI
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include "mylib.h"

#define PORT (in_port_t)50000
#define HOSTNAME_LENGTH 64

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


    
    
    
