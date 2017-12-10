#include "stdafx.h"
#include <chrono>

using namespace std;

int const X_DIM = 100;
int const Y_DIM = 100;
int const Z_DIM = 100;
float nav_cube[X_DIM][Y_DIM][Z_DIM];
void nav_next_to_direct_above(int x, int y, int z, float dot, float sideUp);
void nav_next_to_direct_below(int x, int y, int z, float dot, float sideDown);
void nav_side(int x, int y, int z, float dot, float side);

/*
INIT CUBE - with starting values
*/
void nav_fillCube(float topMidValue) {
	for (int x = 0; x < X_DIM - 1; x++) {
		for (int y = 0; y < Y_DIM - 1; y++) {
			for (int z = 0; z < Z_DIM - 1; z++) {
				nav_cube[x][y][z] = 0.0;
			}
		}
	}
	nav_cube[(X_DIM / 2) - 1][(Y_DIM / 2) - 1][0] = topMidValue;
}

chrono::seconds navier_Stokes(float temp, float thickness) {
	/*
	How fast the color spreads will be determined by the experiment(s).

	The transfer speed will increase/decrease based on a weighted amount of the given tempeture/thickiness.
	*/
	double weight_temp = .0001;
	double weight_thick = .01;
	double directUp = .037 + (temp*weight_temp) - (thickness*weight_thick);
	double sideUp = .001 + (temp*weight_temp) - (thickness*weight_thick);
	double directDown = .06 + (temp*weight_temp) - (thickness*weight_thick);
	double sideDown = .02 + (temp*weight_temp) - (thickness*weight_thick);
	double sideUnit = .001 + (temp*weight_temp) - (thickness*weight_thick);

	auto start_timer = chrono::high_resolution_clock::now();
	bool notFullyMixed = true;
	while (notFullyMixed) {
		for (int z = 1; z < Z_DIM - 1; z++) {
			for (int x = 1; x < X_DIM - 1; x++) {
				for (int y = 1; y < Y_DIM - 1; y++) {
					float dot = nav_cube[x][y][z];
					if (dot > 0) {
						/*
						Unit directly above current unit
						*/
						nav_cube[x][y][z - 1] += dot*directUp;
						/*
						Units next to direct-above unit
						*/
						nav_next_to_direct_above(x, y, z, dot, sideUp);
						/*
						Unit directly below current unit
						*/
						nav_cube[x][y][z + 1] += dot*directDown;
						/*
						Unit next to direct-below unit
						*/
						nav_next_to_direct_below(x, y, z, dot, sideDown);
						/*
						Unit to the side of current unit.
						*/
						nav_side(x, y, z, dot, sideUnit);
						/*
						Remove transfer from original
						*/
						nav_cube[x][y][z] -= dot*directDown + dot*sideDown * 8 + dot*sideUnit * 8 + dot*sideUp*8 + dot*directUp;
					}
				}
			}
		}
		if (nav_cube[(X_DIM / 2) - 1][(Y_DIM / 2) - 1][Z_DIM - 1] > 0 || nav_cube[0][Y_DIM - 1][Z_DIM - 1] > 0
			|| nav_cube[X_DIM - 1][Y_DIM - 1][Z_DIM - 1] > 0 || nav_cube[X_DIM - 1][0][Z_DIM - 1] > 0 || nav_cube[0][0][Z_DIM - 1] > 0) {
			notFullyMixed = false;
		}
	}

	auto end_timer = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::seconds>(end_timer - start_timer);
}

/*
For units that are next to the unit directly above
*/
void nav_next_to_direct_above(int x, int y, int z, float dot, float sideUp) {
	nav_cube[x + 1][y + 1][z - 1] += dot*sideUp;
	nav_cube[x][y + 1][z - 1] += dot*sideUp;
	nav_cube[x - 1][y + 1][z - 1] += dot*sideUp;
	nav_cube[x - 1][y][z - 1] += dot*sideUp;
	nav_cube[x + 1][y][z - 1] += dot*sideUp;
	nav_cube[x - 1][y - 1][z - 1] += dot*sideUp;
	nav_cube[x][y - 1][z - 1] += dot*sideUp;
	nav_cube[x + 1][y - 1][z - 1] += dot*sideUp;
}

/*
For units that are next to the unit directly below
*/
void nav_next_to_direct_below(int x, int y, int z, float dot, float sideDown) {
	nav_cube[x + 1][y + 1][z + 1] += dot*sideDown;
	nav_cube[x][y + 1][z + 1] += dot*sideDown;
	nav_cube[x - 1][y + 1][z + 1] += dot*sideDown;
	nav_cube[x - 1][y][z + 1] += dot*sideDown;
	nav_cube[x + 1][y][z + 1] += dot*sideDown;
	nav_cube[x - 1][y - 1][z + 1] += dot*sideDown;
	nav_cube[x][y - 1][z + 1] += dot*sideDown;
	nav_cube[x + 1][y - 1][z + 1] += dot*sideDown;
}

/*
For units that are off to the side of the current unit
*/
void nav_side(int x, int y, int z, float dot, float side) {
	nav_cube[x + 1][y + 1][z] += dot*side;
	nav_cube[x][y + 1][z] += dot*side;
	nav_cube[x - 1][y + 1][z] += dot*side;
	nav_cube[x - 1][y][z] += dot*side;
	nav_cube[x + 1][y][z] += dot*side;
	nav_cube[x - 1][y - 1][z] += dot*side;
	nav_cube[x][y - 1][z] += dot*side;
	nav_cube[x + 1][y - 1][z] += dot*side;
}