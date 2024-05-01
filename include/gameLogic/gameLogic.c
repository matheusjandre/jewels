#include "gameLogic.h"
#include <stdio.h>
#include <stdlib.h>

PIECE *check_click(GAME *game)
{
    if (game->mouse_x > BOARD_WIDTH * SCALE || game->mouse_y > BOARD_HEIGHT * SCALE)
        return NULL;

    int i = (int)game->mouse_y / SCALE;
    int j = (int)game->mouse_x / SCALE;

    return &(game->board->piece[i][j]);
}

int check_distance(PIECE *first, PIECE *second)
{
    int x_dist = (first->x - second->x) / SCALE;
    int y_dist = (first->y - second->y) / SCALE;

    x_dist *= (x_dist < 0) ? -1 : 1;
    y_dist *= (y_dist < 0) ? -1 : 1;

    if (x_dist > 1 || y_dist > 1 || (x_dist == y_dist))
        return 0;

    return 1;
}

int swap_pieces(PIECE *first, PIECE *second)
{
    // PRIMEIRO RECEBE A POSICAO DO SEGUNDO
    first->dx = second->x;
    first->dy = second->y; 

    // SEGUNDO RECEBE A POSICAO DO PRIMEIRO
    second->dx = first->x;
    second->dy = ((int)(first->y / SCALE)) * SCALE;

    // ATUALIZAR BOARD
    PIECE temp = *first;
    *first = *second;
    *second = temp;

    return 1;
}

int gravity(GAME *game)
{
    for (int j = 0; j < BOARD_WIDTH; j++)
    {
        PIECE col[BOARD_HEIGHT];

        int c = 0;
        for (int i = 0; i < BOARD_HEIGHT; i++)
            if (game->board->piece[i][j].type == 0)
            {
                col[c] = game->board->piece[i][j];
                c++;
            }

        for (int i = 0; i < BOARD_HEIGHT; i++)
            if (game->board->piece[i][j].type != 0)
            {
                col[c] = game->board->piece[i][j];
                c++;
            }

        for (int k = 0; k < BOARD_HEIGHT; k++)
            game->board->piece[k][j] = col[k];

        int n = -1;

        for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
        {
            game->board->piece[i][j].dy = i * SCALE;

            if (game->board->piece[i][j].type == 0)
            {
                game->board->piece[i][j].y = n * SCALE;
                game->board->piece[i][j].type = 1 + rand() % 7;
                n--;
            }
        }
    }

    return 1;
}

int update_board(GAME *game)
{
    int u = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (game->board->piece[i][j].x != game->board->piece[i][j].dx)
            {
                if (game->board->piece[i][j].x > game->board->piece[i][j].dx)
                    game->board->piece[i][j].x -= VELOCITY;
                else
                    game->board->piece[i][j].x += VELOCITY;

                u = 1;
            }
            else if (game->board->piece[i][j].y != game->board->piece[i][j].dy)
            {
                if (game->board->piece[i][j].y > game->board->piece[i][j].dy)
                    game->board->piece[i][j].y -= VELOCITY;
                else
                    game->board->piece[i][j].y += VELOCITY;

                u = 1;
            }
        }
    }

    return u;
}

int check_sequence(PIECE *check[5])
{
    if ((check[0] != NULL && check[1] != NULL && check[2] != NULL) && (check[0]->type == 0 || check[1]->type == 0 || check[2]->type == 0))
        return 0;

    if (check[1] != NULL && check[2] != NULL && check[0]->type == check[1]->type && check[1]->type == check[2]->type)
    {
        if (check[3] != NULL && check[2]->type == check[3]->type)
        {
            if (check[4] != NULL && check[3]->type == check[4]->type)
            {
                check[0]->type = NONE;
                check[1]->type = NONE;
                check[2]->type = NONE;
                check[3]->type = NONE;
                check[4]->type = NONE;

                return 5;
            }
            else
            {
                check[0]->type = NONE;
                check[1]->type = NONE;
                check[2]->type = NONE;
                check[3]->type = NONE;

                return 4;
            }
        }
        else
        {
            check[0]->type = NONE;
            check[1]->type = NONE;
            check[2]->type = NONE;

            return 3;
        }
    }

    return 0;
}

int match(GAME *game)
{
    PIECE *check[5];
    int points = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            // Vertical
            for (int c = 0; c < 5; c++)
                check[c] = (j + c < BOARD_WIDTH) ? &game->board->piece[i][j + c] : NULL;

            points += (check_sequence(check) > 0) * 10;
            // Horizontal
            for (int c = 0; c < 5; c++)
                check[c] = (i + c < BOARD_HEIGHT) ? &game->board->piece[i + c][j] : NULL;

            points += (check_sequence(check) > 0) * 10;
        }
    }

    if (points > 0)
    {
        game->points += points;
        game->first_selected = NULL;
        game->second_selected = NULL;
        return 1;
    }

    return 0;
}

int check_possible_row(GAME *game, PIECE *check[3])
{
    // Define se e vertical ou horizontal
    BOARD *board = game->board;

    if (check[0] == NULL || check[1] == NULL || check[2] == NULL)
        return 0;

    if (check[0]->type == NONE || check[1]->type == NONE || check[2]->type == NONE)
        return 0;

    int horizontal = 0;

    if ((check[0]->x / SCALE) == (check[1]->x / SCALE))
    {
        horizontal = 0;
    }
    else
        horizontal = 1;

    if (horizontal)
    {
        if (check[0]->type == check[1]->type)
            if (((check[2]->y / SCALE) + 1 < BOARD_HEIGHT) && (board->piece[(check[2]->y / SCALE) + 1][(check[2]->x / SCALE)].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE) + 1][(check[2]->x / SCALE)];
                return 1;
            }
            else if (((check[2]->y / SCALE) - 1 >= 0) && (board->piece[(check[2]->y / SCALE) - 1][(check[2]->x / SCALE)].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE) - 1][(check[2]->x / SCALE)];
                return 1;
            }
            else if (((check[2]->x / SCALE) + 1 < BOARD_WIDTH) && (board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) + 1].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) + 1];
                return 1;
            }
            else
                return 0;
        else if (check[1]->type == check[2]->type)
            if (((check[0]->x / SCALE) - 1 >= 0) && (board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) - 1].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) - 1];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[0]->y / SCALE) - 1 >= 0) && (board->piece[(check[0]->y / SCALE) - 1][(check[0]->x / SCALE)].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE) - 1][(check[0]->x / SCALE)];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[0]->y / SCALE) + 1 < BOARD_HEIGHT) && (board->piece[(check[0]->y / SCALE) + 1][(check[0]->x / SCALE)].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE) + 1][(check[0]->x / SCALE)];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else
                return 0;
        else if (check[0]->type == check[2]->type)
            if (((check[1]->y / SCALE) - 1 >= 0) && (board->piece[(check[1]->y / SCALE) - 1][(check[1]->x / SCALE)].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = &board->piece[(check[1]->y / SCALE) - 1][(check[1]->x / SCALE)];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[1]->y / SCALE) + 1 < BOARD_HEIGHT) && (board->piece[(check[1]->y / SCALE) + 1][(check[1]->x / SCALE)].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = &board->piece[(check[1]->y / SCALE) + 1][(check[1]->x / SCALE)];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else
                return 0;
        else
            return 0;
    }
    else
    {
        if (check[0]->type == check[1]->type)
            if (((check[2]->y / SCALE) + 1 < BOARD_HEIGHT) && (board->piece[(check[2]->y / SCALE) + 1][(check[2]->x / SCALE)].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE) + 1][(check[2]->x / SCALE)];
                return 1;
            }
            else if (((check[2]->x / SCALE) - 1 >= 0) && (board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) - 1].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) - 1];
                return 1;
            }
            else if (((check[2]->x / SCALE) + 1 < BOARD_WIDTH) && (board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) + 1].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = &board->piece[(check[2]->y / SCALE)][(check[2]->x / SCALE) + 1];
                return 1;
            }
            else
                return 0;
        else if (check[1]->type == check[2]->type)
            if (((check[0]->y / SCALE) - 1 >= 0) && (board->piece[(check[0]->y / SCALE) - 1][(check[0]->x / SCALE)].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE) - 1][(check[0]->x / SCALE)];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[0]->x / SCALE) - 1 >= 0) && (board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) - 1].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) - 1];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[0]->x / SCALE) + 1 < BOARD_WIDTH) && (board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) + 1].type == check[1]->type))
            {
                game->last_movement_found[0] = &board->piece[(check[0]->y / SCALE)][(check[0]->x / SCALE) + 1];
                game->last_movement_found[1] = check[1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else
                return 0;
        else if (check[0]->type == check[2]->type)
            if (((check[1]->x / SCALE) - 1 >= 0) && (board->piece[(check[1]->y / SCALE)][(check[1]->x / SCALE) - 1].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = &board->piece[(check[1]->y / SCALE)][(check[1]->x / SCALE) - 1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else if (((check[1]->x / SCALE) + 1 < BOARD_WIDTH) && (board->piece[(check[1]->y / SCALE)][(check[1]->x / SCALE) + 1].type == check[0]->type))
            {
                game->last_movement_found[0] = check[0];
                game->last_movement_found[1] = &board->piece[(check[1]->y / SCALE)][(check[1]->x / SCALE) + 1];
                game->last_movement_found[2] = check[2];
                return 1;
            }
            else
                return 0;
        else
            return 0;
    }
}

int check_movements(GAME *game)
{
    PIECE *check[3];
    int found = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            // Vertical
            for (int c = 0; c < 3; c++)
                check[c] = (j + c < BOARD_WIDTH) ? &game->board->piece[i][j + c] : NULL;

            if (check_possible_row(game, check))
                found = 1;

            // Horizontal
            for (int c = 0; c < 3; c++)
                check[c] = (i + c < BOARD_HEIGHT) ? &game->board->piece[i + c][j] : NULL;

            if (check_possible_row(game, check))
                found = 1;
        }
    }

    return found;
}

int game_logic(GAME *game)
{
    switch (game->game_event)
    {
    case IDLE:
        if (game->game_state == GAMING)
        {
            game->points -= 0.1;
        }
        break;

    case INPUT:

        PIECE *last = check_click(game);

        if (last->type == NONE)
        {
            last = NULL;
            game->first_selected = NULL;
            game->second_selected = NULL;
            game->game_event = IDLE;
            return 0;
        }

        if (game->first_selected == NULL)
        {
            game->first_selected = last;
        }
        else
        {
            game->second_selected = last;

            if (check_distance(game->first_selected, game->second_selected))
            {
                swap_pieces(game->first_selected, game->second_selected);
                game->game_event = UPDATING;
                return 1;
            }
            else
            {
                game->first_selected = NULL;
                game->second_selected = NULL;
            }
        }

        game->game_event = IDLE;
        break;

    case UPDATING:
        if (update_board(game))
            game->game_event = UPDATING;
        else if (match(game))
            game->game_event = DESTROYED;
        else
        {
            game->game_event = CHECKING;
        }

        break;

    case DESTROYED:
        if (gravity(game))
            game->game_event = UPDATING;

        break;

    case CHECKING:

        if (game->first_selected != NULL && game->second_selected != NULL)
        {

            swap_pieces(game->first_selected, game->second_selected);

            game->first_selected = NULL;
            game->second_selected = NULL;
            game->game_event = UPDATING;
        }
        else
        {

            if (check_movements(game))
            {
                game->game_event = IDLE;
            }
            else
            {
                game->game_event = GAME_OVER;
            }
        }

        break;

    case GAME_OVER:
        game->game_state = ENDING;
        game->game_event = IDLE;
        break;
    }

    return 0;
}

int help_logic(GAME *game)
{
    if (game->game_event == INPUT)
    {
        if (game->mouse_y >= 400 && game->mouse_y <= 500 && game->mouse_x >= 150 && game->mouse_x <= 450)
        {

            game->game_event = IDLE;
            game->game_state = MENU;
            return 0;
        }

        game->mouse_y = -1;
        game->mouse_x = -1;

        game->game_event = IDLE;
    }

    return 0;
}

int game_over_logic(GAME *game)
{
    if (game->game_event == INPUT)
    {
        if (game->mouse_y >= 400 && game->mouse_y <= 500 && game->mouse_x >= 150 && game->mouse_x <= 450)
        {

            game->points = 0;

            shuffle_board(game);

            game->game_event = IDLE;
            game->game_state = MENU;
            return 0;
        }

        game->mouse_y = -1;
        game->mouse_x = -1;

        game->game_event = IDLE;
    }

    return 0;
}

int menu_logic(GAME *game)
{
    // START BUTTON
    if (game->game_event == INPUT)
    {
        if (game->mouse_y >= 160 && game->mouse_y <= 260 && game->mouse_x >= 150 && game->mouse_x <= 450)
        {
            game->game_event = UPDATING;
            game->game_state = GAMING;
            return 0;
        }

        if (game->mouse_y >= 270 && game->mouse_y <= 370 && game->mouse_x >= 150 && game->mouse_x <= 450)
        {

            game->game_state = HELP;
            return 0;
        }

        // EXIT BUTTON
        if (game->mouse_y >= 380 && game->mouse_y <= 480 && game->mouse_x >= 150 && game->mouse_x <= 450)
        {

            return 1;
        }

        game->mouse_y = -1;
        game->mouse_x = -1;

        game->game_event = IDLE;
    }

    return 0;
}