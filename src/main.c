#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 6
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

typedef enum {
	CELL_EMPTY = 0,
	CELL_WALL,
	CELL_WATER
}cell_type;

cell_type grid[GRID_WIDTH][GRID_HEIGHT];
cell_type grid_next[GRID_WIDTH][GRID_HEIGHT];

void init_grid() {
	for(int x = 0; x < GRID_WIDTH; x++) {
		for(int y = 0; y < GRID_WIDTH; y++) {
			grid[x][y] = CELL_EMPTY;
			grid_next[x][y] = CELL_EMPTY;
		}
	}
}

void draw() {
	for (int x = 0; x < GRID_WIDTH; x++) {
		for(int y = 0; y < GRID_WIDTH; y++) {
			if(grid[x][y] == CELL_WALL) {
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
			}else if(grid[x][y] == CELL_WATER) {
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
			}
		}
	}
}

void mouse_input() {
	Vector2 mouse_pos = GetMousePosition();
	int mouse_x = (int) mouse_pos.x / CELL_SIZE;
	int mouse_y = (int) mouse_pos.y / CELL_SIZE;

	if(mouse_x >= 0 && mouse_x < GRID_WIDTH && mouse_y >= 0 && mouse_y < GRID_HEIGHT) {
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			grid[mouse_x][mouse_y] = CELL_WATER;
		}
	}
}
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
			mouse_input();
      BeginDrawing();
      	ClearBackground(BLACK);
				draw();
      EndDrawing();
    }

    CloseWindow();

    return 0;
}
