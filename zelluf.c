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
	int x1_speed;
	int y1_speed;
	int x2;
	int y2;
	int x2_speed;
	int y2_speed;
	float r1;
	float r1_speed;
	float g1;
	float g1_speed;
	float b1;
	float b1_speed;
} Line;

Point pointer;
Point click_pointer;

#define LINES 25
int head = 0;
int tail = 0;
Line line[LINES];

int x_max = 900;
int y_max = 700;

bool mouse_click = false;

void on_key(S2D_Event e) {
  switch (e.type) {
    case S2D_KEY_DOWN:
      printf("Key down: %s\n", e.key);
      if (strcmp(e.key, "Escape") == 0) S2D_Close(window);
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
        line[head].x1_speed *= 2;
        line[head].y1_speed *= 2;
        line[head].x2_speed *= 2;
        line[head].y2_speed *= 2;
        printf("%d,%d %d,%d\n", line[head].x1_speed, line[head].y1_speed, line[head].x2_speed, line[head].y2_speed);
        if (abs(line[head].x1_speed) > 32) line[head].x1_speed = 1;
        if (abs(line[head].y1_speed) > 32) line[head].y1_speed = 1;
        if (abs(line[head].x2_speed) > 32) line[head].x2_speed = 1;
        if (abs(line[head].y2_speed) > 32) line[head].y2_speed = 1;
        printf("%d,%d %d,%d\n", line[head].x1_speed, line[head].y1_speed, line[head].x2_speed, line[head].y2_speed);
      } else if (e.direction == S2D_MOUSE_SCROLL_INVERTED) {
        puts("Direction inverted");
        line[head].x1_speed /= 2;
        line[head].y1_speed /= 2;
        line[head].x2_speed /= 2;
        line[head].y2_speed /= 2;
        printf("%d,%d %d,%d\n", line[head].x1_speed, line[head].y1_speed, line[head].x2_speed, line[head].y2_speed);
        if (abs(line[head].x1_speed) < 1) line[head].x1_speed = 1;
        if (abs(line[head].y1_speed) < 1) line[head].y1_speed = 1;
        if (abs(line[head].x2_speed) < 1) line[head].x2_speed = 1;
        if (abs(line[head].y2_speed) < 1) line[head].y2_speed = 1;
        printf("%d,%d %d,%d\n", line[head].x1_speed, line[head].y1_speed, line[head].x2_speed, line[head].y2_speed);
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

void init() {
	line[head].x1 = 200;
	line[head].y1 = 200;
	line[head].x2 = 300;
	line[head].y2 = 300;
	line[head].x1_speed = 2;
	line[head].y1_speed = 3;
	line[head].x2_speed = 4;
	line[head].y2_speed = 5;
	line[head].r1 = 0.4;
	line[head].r1_speed = 0.001;
	line[head].g1 = 1;
	line[head].g1_speed = -0.002;
	line[head].b1 = 0.1;
	line[head].b1_speed = 0.003;
}

void colorcycle(float *color, float *color_speed) {
	*color += *color_speed;
	if (*color < 0.0) {
		*color = 0.0;
		*color_speed *= -1;
	}
	if (*color > 1.0) {
		*color = 1.0;
		*color_speed *= -1;
	}
}

// update app state and calculate here, do not draw stuff here
void update() {
	pointer.x = window->mouse.x;
	pointer.y = window->mouse.y;

	int newhead = 0;
	if (head >= LINES -1) {
		newhead = 0;
	} else {
		newhead = head + 1;
	}
	line[newhead].x1 = line[head].x1;
	line[newhead].y1 = line[head].y1;
	line[newhead].x2 = line[head].x2;
	line[newhead].y2 = line[head].y2;
	line[newhead].x1_speed = line[head].x1_speed;
	line[newhead].y1_speed = line[head].y1_speed;
	line[newhead].x2_speed = line[head].x2_speed;
	line[newhead].y2_speed = line[head].y2_speed;
	line[newhead].r1 = line[head].r1;
	line[newhead].r1_speed = line[head].r1_speed;
	line[newhead].g1 = line[head].g1;
	line[newhead].g1_speed = line[head].g1_speed;
	line[newhead].b1 = line[head].b1;
	line[newhead].b1_speed = line[head].b1_speed;
	head = newhead;
	if (tail < LINES - 1) {
		tail++;
	}

	line[head].x1 += line[head].x1_speed;
	line[head].y1 += line[head].y1_speed;
	if (0 > line[head].x1 || line[head].x1 > x_max) {
		line[head].x1_speed *= -1;
		line[head].x1 += line[head].x1_speed;
	}
	if (0 > line[head].y1 || line[head].y1 > y_max) {
		line[head].y1_speed *= -1;
		line[head].y1 += line[head].y1_speed;
	}

	line[head].x2 += line[head].x2_speed;
	line[head].y2 += line[head].y2_speed;
	if (0 > line[head].x2 || line[head].x2 > x_max) {
		line[head].x2_speed *= -1;
		line[head].x2 += line[head].x2_speed;
	}
	if (0 > line[head].y2 || line[head].y2 > y_max) {
		line[head].y2_speed *= -1;
		line[head].y2 += line[head].y2_speed;
	}

	colorcycle(&line[head].r1, &line[head].r1_speed);
	colorcycle(&line[head].g1, &line[head].g1_speed);
	colorcycle(&line[head].b1, &line[head].b1_speed);

}

// draw stuff
void render() {

  // Lines

  for (int line_id = 0; line_id <= tail; line_id++) {
	  S2D_DrawLine(
			  line[line_id].x1, line[line_id].y1, line[line_id].x2, line[line_id].y2,
			  1,
			  line[line_id].r1, line[line_id].g1, line[line_id].b1, 1,
			  line[line_id].r1, line[line_id].g1, line[line_id].b1, 1,
			  line[line_id].r1, line[line_id].g1, line[line_id].b1, 1,
			  line[line_id].r1, line[line_id].g1, line[line_id].b1, 1
	  );
  }

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
    line[head].x1 = click_pointer.x;
    line[head].y1 = click_pointer.y;
  }
}


int main() {
    S2D_Diagnostics(true);

    window = S2D_CreateWindow("zelluf doen", x_max, y_max, update, render, S2D_RESIZABLE);

    window->on_key        = on_key;
    window->on_mouse      = on_mouse;

  // Change viewport scaling modes:
  //   window->viewport.mode = S2D_FIXED;
  //   window->viewport.mode = S2D_EXPAND;
  //   window->viewport.mode = S2D_SCALE;  // Default
  //   window->viewport.mode = S2D_STRETCH;

    puts("Press escape to exit.");

    init();

	S2D_Show(window);

	S2D_FreeWindow(window);

	return 0;
}
