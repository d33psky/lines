#include <simple2d.h>

S2D_Window *window;
S2D_Text   *fps;
S2D_Text   *fps_val;

const char *font = "media/bitstream_vera/vera.ttf";
int font_size = 20;

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
	int x1_speed;
	int y1_speed;
	int x2_speed;
	int y2_speed;
	int color;
} Line;

Point pointer;
Point click_pointer;

Line line;
int x_max = 600;
int y_max = 500;

bool mouse_click = false;
bool rotate = false;

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
    case S2D_MOUSE_MIDDLE:
      puts("Button middle");
      break;
    case S2D_MOUSE_RIGHT:
      puts("Button right");
      break;
    case S2D_MOUSE_X1:
      puts("Button X1");
      break;
    case S2D_MOUSE_X2:
      puts("Button X2");
      break;
  }
}


void on_mouse(S2D_Event e) {
//  puts("=== Mouse Event ===");

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

    case S2D_MOUSE_SCROLL:
      puts("Mouse scroll");
      if (e.direction == S2D_MOUSE_SCROLL_NORMAL) {
        puts("Direction normal");
        line.x1_speed *= 2;
        line.y1_speed *= 2;
        line.x2_speed *= 2;
        line.y2_speed *= 2;
        printf("%d,%d %d,%d\n", line.x1_speed, line.y1_speed, line.x2_speed, line.y2_speed);
        if (abs(line.x1_speed) > 32) line.x1_speed = 1;
        if (abs(line.y1_speed) > 32) line.y1_speed = 1;
        if (abs(line.x2_speed) > 32) line.x2_speed = 1;
        if (abs(line.y2_speed) > 32) line.y2_speed = 1;
        printf("%d,%d %d,%d\n", line.x1_speed, line.y1_speed, line.x2_speed, line.y2_speed);
      } else if (e.direction == S2D_MOUSE_SCROLL_INVERTED) {
        puts("Direction inverted");
        line.x1_speed /= 2;
        line.y1_speed /= 2;
        line.x2_speed /= 2;
        line.y2_speed /= 2;
        printf("%d,%d %d,%d\n", line.x1_speed, line.y1_speed, line.x2_speed, line.y2_speed);
        if (abs(line.x1_speed) < 1) line.x1_speed = 1;
        if (abs(line.y1_speed) < 1) line.y1_speed = 1;
        if (abs(line.x2_speed) < 1) line.x2_speed = 1;
        if (abs(line.y2_speed) < 1) line.y2_speed = 1;
        printf("%d,%d %d,%d\n", line.x1_speed, line.y1_speed, line.x2_speed, line.y2_speed);
      }
      printf("delta x: %i\ndelta y: %i\n", e.delta_x, e.delta_y);
      break;

//    case S2D_MOUSE_MOVE:
//      puts("Mouse movement");
//      printf("delta x: %i\ndelta y: %i\n", e.delta_x, e.delta_y);
//      break;
  }

//  if (e.type != S2D_MOUSE_SCROLL) printf("x: %i, y: %i\n", e.x, e.y);
}


void on_controller(S2D_Event e) {
  puts("=== Controller Event ===");
  printf("Controller #%i\n", e.which);

  switch (e.type) {
    case S2D_AXIS:
      printf("Axis: %i\n", e.axis);
      printf("Value: %i\n", e.value);
      break;

    case S2D_BUTTON_DOWN:
      printf("Button #%i down\n", e.button);
      break;

    case S2D_BUTTON_UP:
      printf("Button #%i up\n", e.button);
      break;
  }
}

// update app state, calculate here, do not draw stuff
void update() {
  pointer.x = window->mouse.x;
  pointer.y = window->mouse.y;

  line.x1 += line.x1_speed;
  line.y1 += line.y1_speed;
  if (0 > line.x1 || line.x1 > x_max) {
	  line.x1_speed *= -1;
	  line.x1 += line.x1_speed;
  }
  if (0 > line.y1 || line.y1 > y_max) {
	  line.y1_speed *= -1;
	  line.y1 += line.y1_speed;
  }

  line.x2 += line.x2_speed;
  line.y2 += line.y2_speed;
  if (0 > line.x2 || line.x2 > x_max) {
	  line.x2_speed *= -1;
	  line.x2 += line.x2_speed;
  }
  if (0 > line.y2 || line.y2 > y_max) {
	  line.y2_speed *= -1;
	  line.y2 += line.y2_speed;
  }

}

// draw stuff
void render() {

  // Lines


  S2D_DrawLine(
    line.x1, line.y1, line.x2, line.y2,
    1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
  );
/*
  S2D_DrawLine(
    1, 1, 599, 499,
    1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
  );

  S2D_DrawLine(
    200, 20, 355, 480,
    25,
    1, 0, 0, 0.5,
    0, 1, 0, 0.5,
    0, 0, 1, 0.5,
    1, 0, 1, 0.5
  );
*/

  // Window stats

  S2D_DrawText(fps);
  if (window->frames % 20 == 0) S2D_SetText(fps_val, "%f", window->fps);
  S2D_DrawText(fps_val);

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
    line.x1 = click_pointer.x;
    line.y1 = click_pointer.y;
  }
}


int main() {

  S2D_Diagnostics(true);

  window = S2D_CreateWindow("zelluf doen", 600, 500, update, render, S2D_RESIZABLE);

  window->on_key        = on_key;
  window->on_mouse      = on_mouse;
  window->on_controller = on_controller;

  // Change viewport scaling modes:
  //   window->viewport.mode = S2D_FIXED;
  //   window->viewport.mode = S2D_EXPAND;
  //   window->viewport.mode = S2D_SCALE;  // Default
  //   window->viewport.mode = S2D_STRETCH;

  puts("Press `R` key to rotate textures.");

	line.x1 = 200;
	line.y1 = 200;
	line.x2 = 300;
	line.y2 = 300;
	line.x1_speed = 2;
	line.y1_speed = 3;
	line.x2_speed = 3;
	line.y2_speed = 4;
	line.color = 1;

  S2D_Show(window);

  S2D_FreeWindow(window);

  return 0;
}
