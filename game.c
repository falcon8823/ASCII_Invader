#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>
#include <unistd.h>
#include "invader.h"
#include "drawing.h"

// ウィンドウ
#define WIN_WIDTH  100
#define WIN_HEIGHT 30

#define BREAK 1
#define NON_BREAK 0
#define KEY_SPACE ' '

#define K_QUIT 'q'

#define BUL_MAX 10
#define ENEMY_X_MAX 5
#define ENEMY_Y_MAX 4

static void update();
static void draw();
static int keyUpdate(int c);
static void die();
static void shot_bullet(int x, int y);


static int width;
static fd_set mask; // ファイルディスクリプタのマスク

static WINDOW *win; // cursesのWINDOW
static struct timeval t_out;

static PLAYER player;
static WALL wall[4];
static ENEMY enemy[20];
static BULLET pl_bul[BUL_MAX];
static int pl_bul_pos;

// ゲーム初期化関数
void game_init() {
	int i, j;
	// ファイルディスクリプタの設定
	width = 1;
	FD_ZERO(&mask);
	FD_SET(0, &mask); // 標準入力の監視	
	t_out.tv_sec = 0;
	t_out.tv_usec = 100000; // 100ms
		
	// init bullet
	for(i = 0; i < BUL_MAX; i++) {
		pl_bul[i].active = FALSE;
		pl_bul[i].velocity = -1;
	}
	pl_bul_pos = 0;

	// init player
	player.x = 50;

	// init enemy
	for(j = 0; j < ENEMY_Y_MAX; j++) {
		for(i = 0; i < ENEMY_X_MAX; i++) {
			enemy[j * ENEMY_X_MAX + i].x = i;
			enemy[j * ENEMY_X_MAX + i].y = j;
			enemy[j * ENEMY_X_MAX + i].type = j;
		}
	}
	
	// 画面作成
	setlocale(LC_ALL,"");
	initscr();
	signal(SIGINT, die); // INTシグナル割り込みでdie終了関数を呼び出し
	win = newwin(WIN_HEIGHT + 2, WIN_WIDTH + 2, 0, 0);
	box(win, '|', '-');
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}

// ゲームループ関数
void game_loop() {
	int key;
	int break_flag = NON_BREAK;
	fd_set readOk;

	while(1) {
		readOk = mask;
		select(width, (fd_set*)&readOk, NULL, NULL, &t_out);		

		// 標準入力の処理
		if(FD_ISSET(0, &readOk)) {
			key = getch();
			break_flag = keyUpdate(key);
		}

		update();

		draw();

		if(break_flag == BREAK) break;
	}
	
	// 終了処理
	die();
}

// ロジックの更新処理関数
// （画面に現れない物）
static void update() {
	int i;

	for(i = 0; i < BUL_MAX; i++) {
		if(pl_bul[i].active == TRUE) {
			pl_bul[i].y += pl_bul[i].velocity;
			if(pl_bul[i].y <= 0) pl_bul[i].active = FALSE;
		}
	}
}

// 描画の更新処理関数
// （画面に現れる物）
static void draw() {
	int i;

	wclear(win);
	box(win, '|', '-');

	draw_player(&player, win);
	
	for(i = 0; i < ENEMY_X_MAX * ENEMY_Y_MAX; i++) {
		draw_enemy(&enemy[i], win);
	}
	
	for(i = 0; i < BUL_MAX; i++) {
		if(pl_bul[i].active == TRUE) {
			draw_bullet(&pl_bul[i], win);
		}
	}
	usleep(10000);
	wrefresh(win);
}

// キー入力取得関数
static int keyUpdate(int c) {
	switch(c) {
		case KEY_RIGHT: // 右矢印キー
			// 自機の位置を右に
			if(player.x <= WIN_WIDTH - 7) player.x += 1;
			break;
		case KEY_LEFT: // 左矢印キー
			// 自機の位置を左に
			if(player.x > 7) player.x -= 1;
			break;
		case KEY_SPACE:
			// 弾を撃つ
			shot_bullet(player.x, PLAYER_Y);
			break;
		case K_QUIT:
			return BREAK;
		default:
			break;
	}
	
	return NON_BREAK;
}

// 終了処理関数
// cursesの終了処理，ネットワークの切断処理
static void die() {
	endwin();
	exit(0);
}

static void shot_bullet(int x, int y) {
	pl_bul_pos = (pl_bul_pos + 1) % BUL_MAX;
	pl_bul[pl_bul_pos].active = TRUE;
	pl_bul[pl_bul_pos].x = x;
	pl_bul[pl_bul_pos].y = y;
}
