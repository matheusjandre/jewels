#include "gameStructure.h"
#include <time.h>
#include <stdlib.h>

BOARD *init_board()
{
    BOARD *board;

    board = malloc(sizeof(BOARD));

    srand(time(NULL));

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            board->piece[i][j].x = (SCREEN_WIDTH / BOARD_HEIGHT) * j;
            board->piece[i][j].y = (SCREEN_HEIGHT / BOARD_WIDTH) * i;

            board->piece[i][j].dx = board->piece[i][j].x;
            board->piece[i][j].dy = board->piece[i][j].y;

            board->piece[i][j].type = 1 + (rand() % 7);
        }
    }

    return board;
}

void shuffle_board(GAME *game)
{
    BOARD *board = game->board;

    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++)
            board->piece[i][j].type = 1 + (rand() % 7);

}

GAME *init_game_manager()
{
    GAME *game;
    game = malloc(sizeof(GAME));

    game->jewels_images = malloc(8 * 256);
    game->menu_images = malloc(7 * 256);

    game->jewels_images[0] = al_load_bitmap("./img/jewels/0.png");
    game->jewels_images[1] = al_load_bitmap("./img/jewels/1.png");
    game->jewels_images[2] = al_load_bitmap("./img/jewels/2.png");
    game->jewels_images[3] = al_load_bitmap("./img/jewels/3.png");
    game->jewels_images[4] = al_load_bitmap("./img/jewels/4.png");
    game->jewels_images[5] = al_load_bitmap("./img/jewels/5.png");
    game->jewels_images[6] = al_load_bitmap("./img/jewels/6.png");
    game->jewels_images[7] = al_load_bitmap("./img/jewels/7.png");

    game->menu_images[0] = al_load_bitmap("./img/menu/play.png");
    game->menu_images[1] = al_load_bitmap("./img/menu/help.png");
    game->menu_images[2] = al_load_bitmap("./img/menu/exit.png");
    game->menu_images[3] = al_load_bitmap("./img/menu/back.png");
    game->menu_images[4] = al_load_bitmap("./img/menu/help_text.png");
    game->menu_images[5] = al_load_bitmap("./img/menu/borderline.png");
    game->menu_images[6] = al_load_bitmap("./img/menu/menu.png");
    srand(time(NULL));
    game->menu_jewel = 1 + rand() % 7;

    game->game_state = MENU;
    game->first_selected = NULL;
    game->second_selected = NULL;
    game->game_event = IDLE;
    game->god_mode = 0;
    game->points = 0;

    return game;
}