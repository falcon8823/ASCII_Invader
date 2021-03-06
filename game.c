/* [ASCII Invader]
 * File: game.c
 * Description: 制御ロジックコード
 * Author: Hayato OKUMOTO
 * Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013
*/

#include "invader.h"
#include "drawing.h"

#define BUF_LEN 30

static void update();
static void draw();
static int keyUpdate(int c);
static void die();
static void shot_bullet(PLAYER *pl);
static int enemy_collision(PLAYER *pl);
static int interpret();
static void dispose_bullet(BULLET *bul);
static void dispose_enemy(ENEMY *ene);
static void ending_win();

static int session_soc;
static char buf[BUF_LEN];
static int is_server;

static int width;
static fd_set mask; // ファイルディスクリプタのマスク

static WINDOW *win; // cursesのWINDOW
static struct timeval t_out;

static clock_t cur_t, pre_t;

static PLAYER player;
static PLAYER player2;
//static WALL wall[4];
static ENEMY enemy[ENEMY_X_MAX * ENEMY_Y_MAX];
static POS enemy_pos;
static int enemy_vel;
static int enemy_move_count;
static int dead_enemy;
static int alive_enemy;

// ゲーム初期化関数
void game_init(int soc, int is_srv) {
	int i, j;
	ENEMY *ene_tmp;

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
	player.bullet.active = FALSE;
	player.bullet.velocity = -1;

	// init player
	player.pos.x = PLAYER_POS_X;
	player.pos.y = PLAYER_POS_Y;

	// init enemy
	for(j = 0; j < ENEMY_Y_MAX; j++) {
		for(i = 0; i < ENEMY_X_MAX; i++) {
			ene_tmp = &enemy[j * ENEMY_X_MAX + i];
			ene_tmp->pos.x = i;
			ene_tmp->pos.y = j;
			ene_tmp->type = j;
			ene_tmp->active = TRUE;
			ene_tmp->point = (ENEMY_Y_MAX - j) * 10;
		}
	}

	enemy_pos = (POS){ .x = ENEMY_FIELD_X, .y = ENEMY_FIELD_Y };

	enemy_vel = D_LEFT;
	enemy_move_count = 0;
	dead_enemy = -1;
	alive_enemy = ENEMY_NUM;
	
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
			break_flag = interpret();
		}


		if(cur_t - pre_t >= FPS) {
			update();
			draw();
			pre_t = clock();
			
			// 相手に情報を送信
			sprintf(buf, "%d %d %d %d %d %d %d %d",
					player.pos.x,
					player.pos.y,
					player.score,
					player.bullet.pos.x,
					player.bullet.pos.y,
					enemy_pos.x,
					enemy_pos.y,
					dead_enemy
					);
			write(session_soc, buf, BUF_LEN);
		}

		if(alive_enemy <= 0) {
			break_flag = BREAK;
		}

		if(break_flag == BREAK) {
			break;
		}
	}
	
	// 終了画面
	ending_win();

	// 終了処理
	die();
}

// ロジックの更新処理関数
// （画面に現れない物）
static void update() {
	BULLET *p_bul = &player.bullet;
	
	// 自分の弾の移動更新
	if(p_bul->active == TRUE) {
		p_bul->pos.y += p_bul->velocity;
		// 枠外に出たら消す
		if(p_bul->pos.y <= 0) dispose_bullet(p_bul);
	}

	// あたり判定と死んだ敵の配列インデックスを取得
	dead_enemy = enemy_collision(&player);
	
	// サーバ側に同期する
	if(is_server == TRUE) {
		// 敵の動作クロックを分周してつくる
		if(enemy_move_count >= ENEMY_MOVE_RATE) {
			// 敵の水平移動
			enemy_pos.x += enemy_vel;
			if(enemy_pos.x <= 5 || enemy_pos.x + (ENEMY_WIDTH + SPACE_X) * ENEMY_X_MAX >= WIN_WIDTH) enemy_vel *= -1;
			enemy_move_count = 0;
		}
		enemy_move_count++;
	}
}

// 描画の更新処理関数
// （画面に現れる物）
static void draw() {
	int i;

	werase(win);
	box(win, '#', '#');
	
	// プレイヤーの描画
	draw_player(&player2, win);
	draw_player(&player, win);
	
	// 敵の描画
	for(i = 0; i < ENEMY_NUM; i++) {
		if(enemy[i].active == TRUE) {
			draw_enemy(&enemy[i], &enemy_pos, win);
		}
	}
	
	// player2の弾の描画
	if(player2.bullet.pos.x != -1) {
		draw_bullet(&player2.bullet, win);
	}
	
	// player1の弾の描画
	if(player.bullet.active == TRUE) {
		draw_bullet(&player.bullet, win);
	}

	draw_score(&player, &player2, win);

	wrefresh(win);
}

// キー入力取得関数
static int keyUpdate(int c) {
	switch(c) {
		case KEY_RIGHT: // 右矢印キー
			// 自機の位置を右に
			if(player.pos.x <= WIN_WIDTH - 7) player.pos.x += 1;
			break;
		case KEY_LEFT: // 左矢印キー
			// 自機の位置を左に
			if(player.pos.x > 7) player.pos.x -= 1;
			break;
		case KEY_SPACE:
			// 弾を撃つ
			shot_bullet(&player);
			break;
		case K_QUIT:
			// 終了シグナルを相手に送信
			buf[0] = 'q';
			write(session_soc, buf, BUF_LEN);
			// breakフラグをreturn
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
	close(session_soc);
	exit(0);
}

static void shot_bullet(PLAYER *pl) {
	// 弾数MAX内で撃つ
	if(pl->bullet.active == FALSE) {
		pl->bullet.active = TRUE;
		pl->bullet.pos = pl->pos;
	}
}

// 敵の当たり判定
static int enemy_collision(PLAYER *pl) {
	int j;
	int x, y;
	BULLET *bul = &pl->bullet;

	if(bul->active == TRUE) {
		for(j = 0; j < ENEMY_NUM; j++) {
			if(enemy[j].active == TRUE) {
				x = ENEMY_X_POS(enemy[j].pos.x, enemy_pos.x);
				y = ENEMY_Y_POS(enemy[j].pos.y, enemy_pos.y);
				if(bul->pos.y == y && bul->pos.x >= x && bul->pos.x < x + ENEMY_WIDTH) {
					// 当たってる
					dispose_bullet(bul);
					dispose_enemy(&enemy[j]);
					pl->score += enemy[j].point;

					return j; // 当たった場合は敵の番号を戻す
				}
			}
		}
	}

	return -1; //当たってない
}

// 通信パケット解釈関数
static int interpret() {
	int dead_enemy;
	POS e_pos;
	
	// 終了シグナルのときはbreakフラグをreturn
	if(buf[0] == 'q') return BREAK;
	
	// パース
	sscanf(buf, "%d %d %d %d %d %d %d %d",
			&player2.pos.x,
			&player2.pos.y,
			&player2.score,
			&player2.bullet.pos.x,
			&player2.bullet.pos.y,
			&e_pos.x,
			&e_pos.y,
			&dead_enemy);
	
	// クライアント側では、敵の位置をサーバと同期
	if(is_server == FALSE) {
		enemy_pos = e_pos;
	}
	
	// 通信相手側で倒された敵を消す
	if(dead_enemy != -1) {
		dispose_enemy(&enemy[dead_enemy]);
	}

	return NON_BREAK;
}

// 弾の消去処理関数
static void dispose_bullet(BULLET *bul) {
	bul->active = FALSE;
	bul->pos.x = -1;
	bul->pos.y = -1;
}

// 敵の消滅処理
static void dispose_enemy(ENEMY *ene) {
	ene->active = FALSE;
	alive_enemy--;
}

// 終了画面
static void ending_win() {
	int key;
	fd_set readOk;

	werase(win);
	box(win, '#', '#');
	draw_ending(&player, &player2, win);
	wrefresh(win);

	while(1) {
		readOk = mask;
		select(width, (fd_set*)&readOk, NULL, NULL, &t_out);		

		// 標準入力の処理
		if(FD_ISSET(0, &readOk)) {
			key = getch();
			if(key == 'q') break;
		}
	}
}
