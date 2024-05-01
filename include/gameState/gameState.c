#include "gameState.h"
#include <time.h>
#include <stdlib.h>

void show_game_over(GAME *game, ALLEGRO_FONT *font)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 50, 0, "SEM MOVIMENTOS - FIM DE JOGO!");

    al_draw_text(font, al_map_rgb(255, 255, 255), 200, 180, 0, "Pontuação:");

    char int_str[20];

    if (game->points < 0)
        game->points = 0;

    sprintf(int_str, "%.0f", game->points);

    al_draw_text(font, al_map_rgb(255, 255, 255), 270, 230, 0, int_str);

    // MENU BUTTON
    al_draw_tinted_bitmap(game->menu_images[6], al_map_rgba_f(1, 1, 1, 1), 150, 400, 0);

    al_flip_display();
}

void show_help(GAME *game, ALLEGRO_FONT *font)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_tinted_bitmap(game->menu_images[4], al_map_rgba_f(1, 1, 1, 1), 0, 0, 0);

    // BACK BUTTON
    al_draw_tinted_bitmap(game->menu_images[3], al_map_rgba_f(1, 1, 1, 1), 150, 400, 0);

    al_flip_display();
}

void show_menu(GAME *game)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_tinted_bitmap(game->jewels_images[game->menu_jewel], al_map_rgba_f(1, 1, 1, 1), 250, 50, 0);

    // PLAY BUTTON
    al_draw_tinted_bitmap(game->menu_images[0], al_map_rgba_f(1, 1, 1, 1), 150, 160, 0);

    // HELP BUTTON
    al_draw_tinted_bitmap(game->menu_images[1], al_map_rgba_f(1, 1, 1, 1), 150, 270, 0);

    // EXIT BUTTON
    al_draw_tinted_bitmap(game->menu_images[2], al_map_rgba_f(1, 1, 1, 1), 150, 380, 0);

    al_flip_display();
}

void show_board(GAME *game)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (game->god_mode)
        al_draw_tinted_bitmap(game->menu_images[5], al_map_rgba_f(1, 0.1, 0.1, 1), 0, 0, 0);

    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (&game->board->piece[i][j] == game->first_selected ||
                (game->god_mode && &game->board->piece[i][j] == game->last_movement_found[0]) ||
                (game->god_mode && &game->board->piece[i][j] == game->last_movement_found[1]) ||
                (game->god_mode && &game->board->piece[i][j] == game->last_movement_found[2]))
            {
                al_draw_tinted_bitmap(game->jewels_images[game->board->piece[i][j].type], al_map_rgba_f(0.5, 0.5, 0.5, 1), game->board->piece[i][j].x, game->board->piece[i][j].y, 0);
            }
            else
            {
                al_draw_tinted_bitmap(game->jewels_images[game->board->piece[i][j].type], al_map_rgba_f(1, 1, 1, 1), game->board->piece[i][j].x, game->board->piece[i][j].y, 0);
            }
        }

    al_flip_display();
}

void screen_update(GAME *game, ALLEGRO_FONT *font)
{
    switch (game->game_state)
    {
    case MENU:
        show_menu(game);
        break;

    case GAMING:
        show_board(game);
        break;

    case ENDING:
        show_game_over(game, font);
        break;

    case HELP:
        show_help(game, font);
        break;

    default:
        break;
    }
}