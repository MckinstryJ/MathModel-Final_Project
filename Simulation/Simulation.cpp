// Simulation.cpp : Simulating the temporal length of two mixing solutions.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

/*
	Questions data gathering will answer:
	1. What is the transfer rate given a certain tempture?
	2. How soon does (top_side, mid_side, bottom_side, bottom_middle) get effected by the mixing solution?
*/
int const X_DIM = 100;
int const Y_DIM = 100;
int const Z_DIM = 100;
float cube[X_DIM][Y_DIM][Z_DIM];
void fillCube(float topMidValue);
float simulation(double temp);
void direct_below(int i, int j, int k, double dot, double sideDown);
void side(int i, int j, int k, double dot, double sideUnit);

/*
	MAIN APPLICATION
*/
int main()
{
	//Init cube
	float topMidValue = 100.0;
	fillCube(topMidValue);

	cout << "Time to evenly spread = " << simulation(70) << endl;
	cin >> topMidValue;
}

/*
	***********************
	*****SUB-FUNCTIONS*****
	***********************
*/

/*
	INIT CUBE - with starting values
*/
void fillCube(float topMidValue) {
	for (int x = 0; x < X_DIM-1; x++) {
		for (int y = 0; y < Y_DIM-1; y++) {
			for (int z = 0; z < Z_DIM-1; z++) {
				cube[x][y][z] = 0.0;
			}
		}
	}
	cube[(X_DIM/2)-1][(Y_DIM/2)-1][0] = topMidValue;
}

/*
	SIMULATION - measuring the time it takes for the mixing solution to full spread.
	
	@param temp the tempeture of the fuild to determine the transfer delay
		(No use for it yet... waiting for actual data)
*/
float simulation(double temp) {
	bool notFullyMixed = true;
	/*
		Even spread in all directions -> directDown = sideDown = sideUnit = .1158
	*/
	double directDown = .1158;
	double sideDown = .1158;
	double sideUnit = .1158;

	//Add...
	//...transfer delay based on tempeture
	//...behavior for bounds (sides and bottom)
	//Start timer
	auto start_timer = chrono::high_resolution_clock::now();
	while (notFullyMixed) {
		for (int z = 0; z < Z_DIM-1; z++) {
			for (int x = 1; x < X_DIM-1; x++) {
				for (int y = 1; y < Y_DIM-1; y++) {
					float dot = cube[x][y][z];
					if (dot > 0) {
						/*
							Unit directly below current unit
						*/
						cube[x][y][z + 1] += dot*directDown;
						/*
							Unit next to direct-below unit
						*/
						direct_below(x, y, z, dot, sideDown);
						/*
							Unit to the side of current unit.
						*/
						side(x, y, z, dot, sideUnit);
					}
				}
			}
		}
		cout << "Round number: " << c++ << endl;
		if (cube[(X_DIM/2)-1][(Y_DIM/2)-1][Z_DIM-1] && cube[0][Y_DIM-1][Z_DIM-1] 
			&& cube[X_DIM-1][Y_DIM-1][Z_DIM-1] && cube[X_DIM-1][0][Z_DIM-1] && cube[0][0][Z_DIM-1]) {
			notFullyMixed = false;
		}
	}
	//End timer and return the duration
	auto end_timer = chrono::high_resolution_clock::now();
	auto microseconds = chrono::duration_cast<chrono::microseconds>(end_timer - start_timer);
	return microseconds.count();
}

/*
	For units that are next to the unit directly below
*/
void direct_below(int x, int y, int z, double dot, double sideDown) {
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
void side(int x, int y, int z, double dot, double side) {
	cube[x + 1][y + 1][z] += dot*side;
	cube[x][y + 1][z] += dot*side;
	cube[x - 1][y + 1][z] += dot*side;
	cube[x - 1][y][z] += dot*side;
	cube[x + 1][y][z] += dot*side;
	cube[x - 1][y - 1][z] += dot*side;
	cube[x][y - 1][z] += dot*side;
	cube[x + 1][y - 1][z] += dot*side;
}
