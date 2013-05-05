/* [ASCII Invader]
 * File: game.c
 * Description: 制御ロジックコード
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#include "invader.h"
#include "drawing.h"

#define BUF_LEN 20

// ウィンドウ
#define WIN_WIDTH  100
#define WIN_HEIGHT 30

#define BREAK 1
#define NON_BREAK 0
#define KEY_SPACE ' '

#define K_QUIT 'q'

#define BUL_MAX 1
#define ENEMY_X_MAX 7
#define ENEMY_Y_MAX 7
#define ENEMY_NUM (ENEMY_X_MAX * ENEMY_Y_MAX)
#define ENEMY_MOVE_RATE 10
#define D_LEFT -1
#define D_RIGHT 1

#define FPS (clock_t)(CLOCKS_PER_SEC / 60)

static void update();
static void draw();
static int keyUpdate(int c);
static void die();
static void shot_bullet(int x, int y);
static void enemy_collision();

static int session_soc;
static char buf[BUF_LEN];
static int is_server;

static int width;
static fd_set mask; // ファイルディスクリプタのマスク

static WINDOW *win; // cursesのWINDOW
static struct timeval t_out;

static clock_t cur_t, pre_t;

static PLAYER player;
static WALL wall[4];
static ENEMY enemy[ENEMY_X_MAX * ENEMY_Y_MAX];
static int enemy_field_x, enemy_field_y;
static BULLET player_bul[BUL_MAX];
static int enemy_vel;
static int enemy_move_count;

// ゲーム初期化関数
void game_init(int soc, int is_srv) {
	int i, j;

	is_server = is_srv;
	session_soc = soc;
	// ファイルディスクリプタの設定
	width = soc + 1;
	FD_ZERO(&mask);
	FD_SET(0, &mask); // 標準入力の監視	
	FD_SET(session_soc, &mask); // ソケットの監視	
	t_out.tv_sec = 0;
	t_out.tv_usec = 1;
		
	// init bullet
	for(i = 0; i < BUL_MAX; i++) {
		player_bul[i].active = FALSE;
		player_bul[i].velocity = -1;
	}

	// init player
	player.x = 50;

	// init enemy
	for(j = 0; j < ENEMY_Y_MAX; j++) {
		for(i = 0; i < ENEMY_X_MAX; i++) {
			enemy[j * ENEMY_X_MAX + i].x = i;
			enemy[j * ENEMY_X_MAX + i].y = j;
			enemy[j * ENEMY_X_MAX + i].type = j;
			enemy[j * ENEMY_X_MAX + i].active = TRUE;
		}
	}
	enemy_field_x = ENEMY_FIELD_X;
	enemy_field_y = ENEMY_FIELD_Y;
	enemy_vel = D_LEFT;
	enemy_move_count = 0;
	
	// 画面作成
	setlocale(LC_ALL,"");
	initscr();
	signal(SIGINT, die); // INTシグナル割り込みでdie終了関数を呼び出し
	win = newwin(WIN_HEIGHT + 2, WIN_WIDTH + 2, 0, 0);
	keypad(stdscr, TRUE);
	curs_set(0); // カーソル非表示
	cbreak();
	noecho();
}

// ゲームループ関数
void game_loop() {
	int key;
	int break_flag = NON_BREAK;
	fd_set readOk;

	while(1) {
		readOk = mask;
		select(width, (fd_set*)&readOk, NULL, NULL, &t_out);		
		cur_t = clock();

		// 標準入力の処理
		if(FD_ISSET(0, &readOk)) {
			key = getch();
			break_flag = keyUpdate(key);
		}

		// ソケット入力の処理
		if(FD_ISSET(session_soc, &readOk)) {
			read(session_soc, buf, BUF_LEN);
			//break_flag = interpret();
		}


		if(cur_t - pre_t >= FPS) {
			update();
			draw();
			pre_t = clock();
		}

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
		if(player_bul[i].active == TRUE) {
			player_bul[i].y += player_bul[i].velocity;
			if(player_bul[i].y <= 0) player_bul[i].active = FALSE;
		}
	}

	enemy_collision();
	
	if(enemy_move_count >= ENEMY_MOVE_RATE) {
		enemy_field_x += enemy_vel;
		if(enemy_field_x <= 5 || enemy_field_x + (ENEMY_WIDTH + SPACE_X) * ENEMY_X_MAX >= WIN_WIDTH) enemy_vel *= -1;
		enemy_move_count = 0;
	}
	enemy_move_count++;
}

// 描画の更新処理関数
// （画面に現れる物）
static void draw() {
	int i;

	werase(win);
	box(win, '#', '#');

	draw_player(&player, win);
	
	for(i = 0; i < ENEMY_NUM; i++) {
		if(enemy[i].active == TRUE) {
			draw_enemy(&enemy[i], enemy_field_x, enemy_field_y, win);
		}
	}
	
	for(i = 0; i < BUL_MAX; i++) {
		if(player_bul[i].active == TRUE) {
			draw_bullet(&player_bul[i], win);
		}
	}

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
	int i;
	
	// 弾数MAX内で撃つ
	for(i = 0; i < BUL_MAX; i++) {
		if(player_bul[i].active == FALSE) {
			player_bul[i].active = TRUE;
			player_bul[i].x = x;
			player_bul[i].y = y;
			break;
		}
	}
}

// 敵の当たり判定
static void enemy_collision() {
	int i, j;
	int x, y;
	BULLET *bul;

	for(i = 0; i < BUL_MAX; i++) {
		if(player_bul[i].active == TRUE) {
			bul = &player_bul[i];
			for(j = 0; j < ENEMY_NUM; j++) {
				if(enemy[j].active == TRUE) {
					x = ENEMY_X_POS(enemy[j].x, enemy_field_x);
					y = ENEMY_Y_POS(enemy[j].y, enemy_field_y);
					if(bul->y == y && bul->x >= x && bul->x < x + ENEMY_WIDTH) {
						// 当たってる
						bul->active = FALSE;
						enemy[j].active = FALSE;
					}
				}
			}
		}
	}
}

