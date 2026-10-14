#include "raylib.h"
#include <stdlib.h>

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

	if(mouse_x >= 0 && mouse_x < GRID_WIDTH && mouse_y >= 0 && mouse_y < GRID_HEIGHT) {
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			grid[mouse_x][mouse_y] = CELL_WATER;
		}
	}
}

void copy_grid(cell_type src[GRID_WIDTH][GRID_HEIGHT], cell_type dest[GRID_WIDTH][GRID_HEIGHT]){
	for(int x = 0; x < GRID_WIDTH; x++) {
		for(int y = 0; y < GRID_HEIGHT; y++) {
			dest[x][y] = src[x][y];
		}
	}
} 

void update_simulation() {
	copy_grid(grid, grid_next);
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[x][y] == CELL_WATER) {
				// falling down
				bool is_in_range = y + 1 < GRID_HEIGHT;
				bool is_cell_empty = grid[x][y + 1] == CELL_EMPTY && grid_next[x][y + 1] == CELL_EMPTY;
				if (is_in_range && is_cell_empty) {
					grid_next[x][y] = CELL_EMPTY;
					grid_next[x][y + 1] = CELL_WATER;
				}else {
					// diagonal falling
					int dir = (rand() % 2 == 0) ? 1 : -1;
					int new_x = x + dir;
					int new_y = y + 1;
					bool bounds_ok = new_x >= 0 && new_x < GRID_WIDTH && new_y < GRID_HEIGHT;
					bool cell_available = grid[new_x][new_y] == CELL_EMPTY && grid_next[new_x][new_y] == CELL_EMPTY;

					int new_x_opp = x - dir;
					bool bounds_ok_opp = new_x_opp >= 0 && new_x_opp < GRID_WIDTH && new_y < GRID_HEIGHT;
					bool cell_available_opp = grid[new_x_opp][new_y] == CELL_EMPTY && grid_next[new_x_opp][new_y] == CELL_EMPTY;

					if(bounds_ok && cell_available) {
						grid_next[x][y] = CELL_EMPTY;
						grid_next[new_x][new_y] = CELL_WATER;
					}else if(bounds_ok_opp && cell_available_opp){
						grid_next[x][y] = CELL_EMPTY;
						grid_next[new_x_opp][y + 1] = CELL_WATER;
					}else {
						int side_dir = (rand() % 2 == 0) ? 1 : -1;
						int side_x = x + side_dir;
						bool bounds_ok_side = side_x >= 0 && side_x < GRID_WIDTH;
						bool cell_available_side = grid[side_x][y] == CELL_EMPTY && grid_next[side_x][y] == CELL_EMPTY;
						if(bounds_ok_side && cell_available_side) {
							grid_next[x][y] = CELL_EMPTY;
							grid_next[side_x][y] = CELL_WATER;
						}
					}
				}
			}
		}
	}
	copy_grid(grid_next, grid);
}
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
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
