#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "net/ancillarycat/viva/viva.h"

#define WIDTH 30
#define HEIGHT 30

int randint(const int min, const int max) {
	contract_assert(min < max);
	return rand() % (max - min) + min;
}

void initializeMaze(int maze[HEIGHT][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			maze[y][x] = 1; // Fill the maze with walls
		}
	}
}

void carvePath(int maze[HEIGHT][WIDTH], int x, int y) {
	// Directions: right, down, left, up
	int dx[] = {2, 0, -2, 0};
	int dy[] = {0, 2, 0, -2};

	// Randomize directions
	for (int i = 0; i < 4; i++) {
		int r			= rand() % (4 - i);
		int tempX = dx[i];
		int tempY = dy[i];
		dx[i]			= dx[r + i];
		dy[i]			= dy[r + i];
		dx[r + i] = tempX;
		dy[r + i] = tempY;
	}

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		// Check bounds and if the cell is a wall
		if (nx >= 0 && ny >= 0 && nx < WIDTH && ny < HEIGHT && maze[ny][nx] == 1) {
			maze[y + dy[i] / 2][x + dx[i] / 2] = 0; // Carve a path
			maze[ny][nx]											 = 0; // Set the new cell as a path
			carvePath(maze, nx, ny); // Recur
		}
	}
}

void generateMaze(int maze[HEIGHT][WIDTH]) {
	// Initialize maze
	initializeMaze(maze);

	// Randomly set entrance on the left side
	int entranceY			 = rand() % HEIGHT;
	maze[entranceY][0] = 0; // Entrance at a random position on the left

	// Carve paths from the entrance
	carvePath(maze, 1, entranceY);

	// // Ensure only one exit on the right side
	// int exitY = rand() % HEIGHT;
	// while (maze[exitY][WIDTH - 1] != 0) { // Ensure the exit is on a path
	// 	exitY = rand() % HEIGHT;
	// }
	// maze[exitY][WIDTH - 1] = 0; // Exit at a valid position on the right

	// Create a column before the exit with only one exit path
	// int exitColumn = WIDTH - 1; // Second to last column
	while (true) {
		val e = randint(0, HEIGHT);
		if (maze[e][WIDTH - 2] != 0)
			continue;
		maze[e][WIDTH - 1] = 0;
		for (int y = 0; y < HEIGHT; y++) {
			if (y != e) {
				maze[y][WIDTH - 1] = 1; // Fill with walls
			}
		}
		break;
	}
}


void printMaze(int maze[HEIGHT][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			// printf("%d ", maze[y][x]);
			// if val = 0 , print as red number
			if (maze[y][x] == 0) {
				printf("\033[0;31m%d\033[0m ", maze[y][x]);
			} else {
				printf("%d ", maze[y][x]);
			}
		}
		printf("\n");
	}
}

int main() {
	srand(time(NULL)); // Seed for random number generation

	int maze[HEIGHT][WIDTH];
	generateMaze(maze);
	printMaze(maze);

	return 0;
}
