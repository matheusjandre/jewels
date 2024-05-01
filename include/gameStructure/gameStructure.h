#ifndef _GAME_STRUCTURE_H_
#define _GAME_STRUCTURE_H_

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 600

#define BOARD_HEIGHT 6
#define BOARD_WIDTH 6

#define SCALE (SCREEN_HEIGHT / BOARD_HEIGHT)

#define VELOCITY 10

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

typedef enum
{
    NONE,
    BLUE,
    GREEN,
    LIGHT_BLUE,
    PINK,
    RED,
    YELLOW,
    ORANGE
} JEWEL;

typedef struct
{
    int x;
    int y;
    int dx;
    int dy;
    JEWEL type;
} PIECE;

typedef struct
{
    PIECE piece[BOARD_WIDTH][BOARD_HEIGHT];
} BOARD;

typedef enum
{
    IDLE,
    INPUT,
    UPDATING,
    DESTROYED,
    CHECKING,
    GAME_OVER
} GAME_EVENT;

typedef enum
{
    MENU,
    GAMING,
    ENDING,
    HELP
} GAME_STATE;

typedef struct
{
    ALLEGRO_BITMAP **jewels_images;
    ALLEGRO_BITMAP **menu_images;
    int menu_jewel;

    int god_mode;
    float points;

    BOARD *board;
    GAME_EVENT game_event;
    GAME_STATE game_state;

    int mouse_x;
    int mouse_y;

    PIECE *first_selected;
    PIECE *second_selected;

    PIECE *last_movement_found[3];
} GAME;

BOARD *init_board();
GAME *init_game_manager();
void shuffle_board(GAME *game);

#endif