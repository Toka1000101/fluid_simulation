#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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

static bool in_bounds(int x, int y) {
	return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

void init_grid() {
	for(int x = 0; x < GRID_WIDTH; x++) {
		for(int y = 0; y < GRID_HEIGHT; y++) {
			grid[x][y] = CELL_EMPTY;
			grid_next[x][y] = CELL_EMPTY;
		}
	}
}

void draw() {
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
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

	if(!in_bounds(mouse_x, mouse_y)) return;

	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		grid[mouse_x][mouse_y] = CELL_WATER;
	} else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		grid[mouse_x][mouse_y] = CELL_EMPTY;
	}
}

void copy_grid(cell_type src[GRID_WIDTH][GRID_HEIGHT], cell_type dest[GRID_WIDTH][GRID_HEIGHT]){
	memcpy(dest, src, GRID_WIDTH * GRID_HEIGHT * sizeof(cell_type));
}

void update_simulation() {
	copy_grid(grid, grid_next);
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[x][y] != CELL_WATER) continue;

			// falling down
			if (in_bounds(x, y + 1) &&
					grid[x][y + 1] == CELL_EMPTY &&
					grid_next[x][y + 1] == CELL_EMPTY) {
				grid_next[x][y] = CELL_EMPTY;
				grid_next[x][y + 1] = CELL_WATER;
				continue;
			}

			// diagonal falling
			int dir = (rand() % 2 == 0) ? 1 : -1;
			int new_x = x + dir;
			int new_y = y + 1;
			int new_x_opp = x - dir;

			if (in_bounds(new_x, new_y) && grid[new_x][new_y] == CELL_EMPTY && grid_next[new_x][new_y] == CELL_EMPTY) {
				grid_next[x][y] = CELL_EMPTY;
				grid_next[new_x][new_y] = CELL_WATER;
			} else if (in_bounds(new_x_opp, new_y) && grid[new_x_opp][new_y] == CELL_EMPTY && grid_next[new_x_opp][new_y] == CELL_EMPTY) {
				grid_next[x][y] = CELL_EMPTY;
				grid_next[new_x_opp][new_y] = CELL_WATER;
			} else {
				// side spread
				int side_dir = (rand() % 2 == 0) ? 1 : -1;
				int side_x = x + side_dir;
				if (in_bounds(side_x, y) && grid[side_x][y] == CELL_EMPTY && grid_next[side_x][y] == CELL_EMPTY) {
					grid_next[x][y] = CELL_EMPTY;
					grid_next[side_x][y] = CELL_WATER;
				}
			}
		}
	}
	copy_grid(grid_next, grid);
}

int main(void) {
	srand((unsigned)time(NULL));
	init_grid();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Simulation");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		mouse_input();
		update_simulation();
		BeginDrawing();
		ClearBackground(BLACK);
		draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
