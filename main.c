#include "./include/gameStructure/gameStructure.h"
#include "./include/gameLogic/gameLogic.h"
#include "./include/gameState/gameState.h"

int main()
{
    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    if (!al_install_mouse())
    {
        printf("couldn't initialize mouse\n");
        return 1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_HEIGHT, SCREEN_WIDTH);
    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT *font = al_load_font("./fonts/OpenSans-Regular.ttf", 36, 0);
    if (!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }

    if (!al_init_image_addon())
    {
        printf("couldn't initialize image addon\n");
        exit(1);
    }

    // INICIALIZA GAME MANAGER
    GAME *game = init_game_manager();

    // INICIALIZA TABULEIRO
    game->board = init_board();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:

            switch (game->game_state)
            {
            case GAMING:
                game_logic(game);
                break;

            case MENU:
                done = menu_logic(game);
                break;

            case HELP:
                help_logic(game);
                break;

            case ENDING:
                game_over_logic(game);
                break;

            default:
                break;
            }

            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if (game->game_state == GAMING && event.keyboard.keycode == ALLEGRO_KEY_D)
                game->god_mode = !game->god_mode;

            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (game->game_event == IDLE)
            {
                game->mouse_x = event.mouse.x;
                game->mouse_y = event.mouse.y;
                game->menu_jewel = 1 + rand() % 7;
                game->game_event = INPUT;
            }
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            screen_update(game, font);
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}