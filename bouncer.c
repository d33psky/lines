// bouncer.c
#include <simple2d.h>

S2D_Window *window;
S2D_Text   *score;
S2D_Text   *score_val;

const char *font = "media/bitstream_vera/vera.ttf";
int font_size = 20;

typedef struct {
    int x;
    int y;
} Point;
Point pointer;
Point click_pointer;

typedef struct
{
    int x;
    int y;
    int x_size;
    int y_size;
    int bottom_offset;
} Bat;
Bat bat;

typedef struct {
    int x;
    int y;
    int x_speed;
    int y_speed;
} Stone;
Stone stone;

int window_x = 600;
int window_y = 500;
int bat_bottom_offset = 40;

bool mouse_click = false;
bool rotate = false;

int hits = 0;
int misses = 0;


void on_key(S2D_Event e) {
    switch (e.type) {
    case S2D_KEY_DOWN:
        printf("Key down: %s\n", e.key);
        if (strcmp(e.key, "Escape") == 0) S2D_Close(window);
        if (strcmp(e.key, "R") == 0) rotate = rotate ? false : true;
        break;

    case S2D_KEY_HELD:
        printf("Key held: %s\n", e.key);
        break;

    case S2D_KEY_UP:
        printf("Key up: %s\n", e.key);
        break;
    }
}


void print_mouse_button(int e) {
    switch (e) {
    case S2D_MOUSE_LEFT:
        puts("Button left");
        break;
    }
}


void on_mouse(S2D_Event e) {
    puts("=== Mouse Event ===");

    switch (e.type) {
    case S2D_MOUSE_DOWN:
        puts("Mouse down");
        print_mouse_button(e.button);
        if (e.dblclick) puts("Double click");
        mouse_click = true;
        click_pointer.x = e.x;
        click_pointer.y = e.y;
        break;

    case S2D_MOUSE_UP:
        puts("Mouse up");
        print_mouse_button(e.button);
        if (e.dblclick) puts("Double click");
        break;
    }
}


void update() {
    pointer.x = window->mouse.x;
    pointer.y = window->mouse.y;
    bat.x = window->mouse.x;
    bat.y = window_y - bat.bottom_offset;

    stone.x += stone.x_speed;
    stone.y += stone.y_speed;
    if (0 >= stone.x || stone.x >= window_x) {
        stone.x_speed = -stone.x_speed;
        stone.x += stone.x_speed;
    }
    if (0 >= stone.y) {
        stone.y_speed = -stone.y_speed;
        stone.y += stone.y_speed;
    }
    if (stone.y > bat.y) {
        if (bat.x - bat.x_size < stone.x && stone.x < bat.x + bat.x_size) {
            hits += 1;
            stone.y_speed = -stone.y_speed;
            stone.y += stone.y_speed;
        }
    }
    if (stone.y > window_y) {
        misses += 1;

        // prep a new stone
        stone.x = window_x / 2;
        stone.y = 0;
        stone.x_speed *= 2;
        stone.y_speed *= 2;
        if (stone.x_speed > 10) {
            stone.x_speed = 10;
        }
        if (stone.x_speed < -10) {
            stone.x_speed = -10;
        }
        if (stone.y_speed > 10) {
            stone.y_speed = 10;
        }
    }
}


void render() {

    // stone
    S2D_DrawQuad(stone.x +  0, stone.y +  0, 1, 0, 0, 1,
                 stone.x + 10, stone.y +  0, 1, 0, 0, 1,
                 stone.x + 10, stone.y + 10, 1, 0, 0, 1,
                 stone.x +  0, stone.y + 10, 1, 0, 0, 1);

    // Bat
    S2D_DrawQuad(bat.x - bat.x_size, bat.y - bat.y_size,  1, 1, 1, 1,
                 bat.x + bat.x_size, bat.y - bat.y_size,  1, 1, 1, 1,
                 bat.x + bat.x_size, bat.y + bat.y_size,  1, 1, 1, 1,
                 bat.x - bat.x_size, bat.y + bat.y_size,  1, 1, 1, 1);

    // Score
    S2D_DrawText(score);

    if (window->frames % 20 == 0) S2D_SetText(score_val, "%d - %d", hits, misses);
    S2D_DrawText(score_val);

    // Mouse positions
    S2D_DrawQuad(pointer.x - 5, pointer.y - 7,  1, 1, 1, 1,
                 pointer.x + 5, pointer.y - 7,  1, 1, 1, 1,
                 pointer.x + 5, pointer.y + 4,  1, 1, 1, 1,
                 pointer.x - 5, pointer.y + 4,  1, 1, 1, 1);

    if (mouse_click) {
        S2D_DrawQuad(click_pointer.x - 9, click_pointer.y - 11, 0, 1, 0, 1,
                     click_pointer.x + 9, click_pointer.y - 11, 0, 1, 0, 1,
                     click_pointer.x + 9, click_pointer.y + 8,  0, 1, 0, 1,
                     click_pointer.x - 9, click_pointer.y + 8,  0, 1, 0, 1);
        mouse_click = false;
    }

}


int main() {
    stone.x = window_x / 2;
    stone.y = 0;
    stone.x_speed = 1;
    stone.y_speed = 1;

    bat.x_size = 50;
    bat.y_size = 4;
    bat.bottom_offset = 40;

    S2D_Diagnostics(true);

    window = S2D_CreateWindow("Simple 2D — Bouncer", window_x, window_y, update, render, S2D_RESIZABLE);
    window->on_key        = on_key;
    window->on_mouse      = on_mouse;

    // Change viewport scaling modes:
    //   window->viewport.mode = S2D_FIXED;
    //   window->viewport.mode = S2D_EXPAND;
    //   window->viewport.mode = S2D_SCALE;  // Default
    //   window->viewport.mode = S2D_STRETCH;

    score = S2D_CreateText(font, "Score hits and misses:", font_size);
    score->x = 230;
    score->y = 470;

    score_val = S2D_CreateText(font, "", font_size);
    score_val->x = 480;
    score_val->y = 470;

    S2D_Show(window);
    S2D_FreeWindow(window);

    return 0;
}
