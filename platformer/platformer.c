// Draw a colored poly primitive
//
// With help from Nicolas Noble, Jaby smoll Seamonstah, Lameguy64
// 
// From ../psyq/addons/graphics/MESH/RMESH/TUTO0.C :
// Schnappy 2021
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libapi.h>
#define VMODE 0                 // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240          // Screen height
#define CENTERX SCREENXRES/2    // Center of screen on x 
#define CENTERY SCREENYRES/2    // Center of screen on y
#define MARGINX 0                // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7             // Text Field Height
#define OTLEN 8                    // Ordering Table Length 

#define MAX_BALLS 10
#define PLAYER_LIVES 10

DISPENV disp[2];                   // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
u_long ot[2][OTLEN];               // double ordering table of length 8 * 32 = 256 bits / 32 bytes
char primbuff[2][32768];     // double primitive buffer of length 32768 * 8 =  262.144 bits / 32,768 Kbytes
char *nextpri = primbuff[0];       // pointer to the next primitive in primbuff. Initially, points to the first bit of primbuff[0]
short db = 0;                      // index of which buffer is used, values 0, 1


typedef struct
{
    TILE *tile;
    int x, y;
    int dx, dy;
    int speed;
} Ball;

typedef struct
{
    TILE *tile;
    int x, y;
    int width;
    int speed;
    int lives;
    int pad_id;
} Platform;


Platform platform_top, platform_bottom;

Ball balls[MAX_BALLS];
int active_balls = 0;
int ball_timer = 0;
const int ball_interval = 3 * 60;
int ball_spawn_delay = ball_interval;

void init_platform(Platform *platform, int x, int y, int width, int speed, int lives, int pad_id, u_char r, u_char g, u_char b) {
    platform->x = x;
    platform->y = y;
    platform->width = width;
    platform->speed = speed;
    platform->lives = lives;
    platform->pad_id = pad_id;

    platform->tile = (TILE*)nextpri;
    setTile(platform->tile);
    setRGB0(platform->tile, r, g, b);
    setXY0(platform->tile, platform->x, platform->y);
    setWH(platform->tile, platform->width, 16);
    addPrim(ot[db], platform->tile);

    nextpri += sizeof(TILE);
}

void spawn_ball() {
    if (active_balls >= MAX_BALLS) {
        return;
    }

    Ball *ball = &balls[active_balls++];
    ball->x = CENTERX;
    ball->y = CENTERY;

     // Ensure the ball moves towards either the top or bottom platform
    ball->dx = (rand() % 3) - 1;  // Randomly choose between -1, 0, or 1 for horizontal direction
    ball->dy = (rand() % 2) ? 1 : -1;  // Randomly choose between moving up (-1) or down (1)

    // Ensure the ball has a non-zero horizontal component if desired
    if (ball->dx == 0) {
        ball->dx = (rand() % 2) ? 1 : -1;  // Force a left or right direction
    }

    ball->speed = 2;
    ball->tile = (TILE*)nextpri;
    setTile(ball->tile);
    setRGB0(ball->tile, 255, 255, 0);
    setXY0(ball->tile, ball->x, ball->y);
    setWH(ball->tile, 16, 16);
    addPrim(ot[db], ball->tile);

    nextpri += sizeof(TILE);
}

void remove_ball(int index) {
    if (index < 0 || index >= active_balls) {
        return;
    }

    for (int i = index; i < active_balls - 1; i++) {
        balls[i] = balls[i+1];
    }

    --active_balls;
}

void update_ball(int index) {
    Ball *ball = &balls[index];

    ball->x += ball->dx * ball->speed;
    ball->y += ball->dy * ball->speed;

    if (ball->x < 0 || ball->x > SCREENXRES - 16) {
        ball->dx = -ball->dx;
        // ball->speed++;
    }

    if (ball->y < 0) {
        platform_top.lives--;
        remove_ball(index);

        return;
    }

    if (ball->y > SCREENYRES - 16) {
        platform_bottom.lives--;
        remove_ball(index);

        return;
    }

    int collisionTop = ball->y <= platform_top.y + 16 && ball->x >= platform_top.x && ball->x <= platform_top.x + platform_top.width;
    int collisionBottom = ball->y >= platform_bottom.y - 16 && ball->x >= platform_bottom.x && ball->x <= platform_bottom.x + platform_bottom.width;

    if (collisionTop || collisionBottom) {
        ball->dy = -ball->dy;
        ball->speed++;
    }

    setXY0(ball->tile, ball->x, ball->y);
}

void move_platform(Platform *platform) {
    int pad = PadRead(platform->pad_id);

    if (pad & PADLleft) {
        platform->x -= platform->speed;
        if (platform->x < 0) {
            platform->x = 0;
        }
    } else if (pad & PADLright) {
        platform->x += platform->speed;
        if (platform->x > SCREENXRES - platform->width) {
            platform->x = SCREENXRES - platform->width;
        }
    }

    setXY0(platform->tile, platform->x, platform->y);
}

void init(void)
{
    srand(1526);  // Seed randomness based on vertical sync
    ResetGraph(0);
    // Initialize and setup the GTE
    InitGeom();
    SetGeomOffset(CENTERX,CENTERY);
    SetGeomScreen(CENTERX);
    SetDefDispEnv(&disp[0], 0, 0, SCREENXRES, SCREENYRES);
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);
    if (VMODE) {
        SetVideoMode(MODE_PAL);
        disp[0].disp.y = disp[1].disp.y = 8;
    }
    SetDispMask(1);                 
    setRGB0(&draw[0], 50, 50, 50);
    setRGB0(&draw[1], 50, 50, 50);
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 );
}

void display(void)
{
    DrawSync(0);
    VSync(0);
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    DrawOTag(&ot[db][OTLEN - 1]);

    db = !db;
    nextpri = primbuff[db];
}

int main(void)
{
    int pad = 0;
    init();

    init_platform(&platform_top, CENTERX - 64 / 2, 20, 64, 6, PLAYER_LIVES, 0, 255, 0, 0);
    init_platform(&platform_bottom, CENTERX - 64 / 2, SCREENYRES - 36, 64, 6, PLAYER_LIVES, 1, 0, 0, 255);

    PadInit(0);


    while (platform_top.lives > 0 && platform_bottom.lives > 0)
    {
        ClearOTagR(ot[db], OTLEN);

        --ball_spawn_delay;
        if (ball_spawn_delay <= 0) {
            spawn_ball();
            ball_spawn_delay = ball_interval;
        }

        for (int i = 0; i < active_balls; i++) {
            update_ball(i);
            addPrim(ot[db], balls[i].tile);
        }


        move_platform(&platform_top);
        // move_platform(&platform_bottom, pad);

        addPrim(ot[db], platform_top.tile);
        addPrim(ot[db], platform_bottom.tile);
        
        FntPrint("Top lives: %d, Bottom lives: %d", platform_top.lives, platform_bottom.lives);                   
        FntFlush(-1);
        display();
        }
    return 0;
    }
