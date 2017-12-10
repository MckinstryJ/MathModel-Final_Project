#include "stdafx.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int const X_DIM = 100;
int const Y_DIM = 100;
int const Z_DIM = 100;
float cube[X_DIM][Y_DIM][Z_DIM];
void next_to_direct_below(int x, int y, int z, float dot, float sideDown);
void side(int x, int y, int z, float dot, float side);

/*
	INIT CUBE - with starting values
*/
void fillCube(float topMidValue) {
	for (int x = 0; x < X_DIM - 1; x++) {
		for (int y = 0; y < Y_DIM - 1; y++) {
			for (int z = 0; z < Z_DIM - 1; z++) {
				cube[x][y][z] = 0.0;
			}
		}
	}
	cube[(X_DIM / 2) - 1][(Y_DIM / 2) - 1][0] = topMidValue;
}

/*
	BASE SIMULATION - measuring the time it takes for the mixing solution to full spread.

	@param temp    the tempeture of the fuild to determine the transfer delay
	@param thickness    the thickiness of the fuild (with an increase in corn starch).
*/
chrono::seconds base_Simulation(float temp, float thickness) {
	/*
		How fast the color spreads will be determined by the experiment(s).

		The transfer speed will increase/decrease based on a weighted amount of the given tempeture/thickiness.
	*/
	double weight_temp = .0001;
	double weight_thick = .01;
	double directDown = .05 + (temp*weight_temp) - (thickness*weight_thick);
	double sideDown = .025 + (temp*weight_temp) - (thickness*weight_thick);
	double sideUnit = .0125 + (temp*weight_temp) - (thickness*weight_thick);

	auto start_timer = chrono::high_resolution_clock::now();
	bool notFullyMixed = true;
	while (notFullyMixed) {
		for (int z = 0; z < Z_DIM - 1; z++) {
			for (int x = 1; x < X_DIM - 1; x++) {
				for (int y = 1; y < Y_DIM - 1; y++) {
					float dot = cube[x][y][z];
					if (dot > 0) {
						/*
							Unit directly below current unit
						*/
						cube[x][y][z + 1] += dot*directDown;
						/*
							Unit next to direct-below unit
						*/
						next_to_direct_below(x, y, z, dot, sideDown);
						/*
							Unit to the side of current unit.
						*/
						side(x, y, z, dot, sideUnit);
						/*
							Remove transfer from original
						*/
						cube[x][y][z] -= dot*directDown + dot*sideDown * 8 + dot*sideUnit * 8;
					}
				}
			}
		}
		if (cube[(X_DIM / 2) - 1][(Y_DIM / 2) - 1][Z_DIM - 1] > 0 || cube[0][Y_DIM - 1][Z_DIM - 1] > 0
			|| cube[X_DIM - 1][Y_DIM - 1][Z_DIM - 1] > 0 || cube[X_DIM - 1][0][Z_DIM - 1] > 0 || cube[0][0][Z_DIM - 1] > 0) {
			notFullyMixed = false;
		}
	}

	auto end_timer = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::seconds>(end_timer - start_timer);
}

/*
	For units that are next to the unit directly below
*/
void next_to_direct_below(int x, int y, int z, float dot, float sideDown) {
	cube[x + 1][y + 1][z + 1] += dot*sideDown;
	cube[x][y + 1][z + 1] += dot*sideDown;
	cube[x - 1][y + 1][z + 1] += dot*sideDown;
	cube[x - 1][y][z + 1] += dot*sideDown;
	cube[x + 1][y][z + 1] += dot*sideDown;
	cube[x - 1][y - 1][z + 1] += dot*sideDown;
	cube[x][y - 1][z + 1] += dot*sideDown;
	cube[x + 1][y - 1][z + 1] += dot*sideDown;
}

/*
For units that are off to the side of the current unit
*/
void side(int x, int y, int z, float dot, float side) {
	cube[x + 1][y + 1][z] += dot*side;
	cube[x][y + 1][z] += dot*side;
	cube[x - 1][y + 1][z] += dot*side;
	cube[x - 1][y][z] += dot*side;
	cube[x + 1][y][z] += dot*side;
	cube[x - 1][y - 1][z] += dot*side;
	cube[x][y - 1][z] += dot*side;
	cube[x + 1][y - 1][z] += dot*side;
}