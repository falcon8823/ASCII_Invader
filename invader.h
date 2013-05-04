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
    int endure;//耐久
    int life;//HP
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


    
    
    
