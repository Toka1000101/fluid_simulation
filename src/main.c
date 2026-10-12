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

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
