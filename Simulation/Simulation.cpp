#include "stdafx.h"
#include <iostream>
#include <string>
#include <chrono>
#include "bSim.h"
#include "navier_stokes.h"

using namespace std;

/*
	MAIN APPLICATION
*/
int main()
{
	//Inital amount in the top middle unit of the cube
	float topMidValue = 100.0;
	fillCube(topMidValue);
	nav_fillCube(topMidValue);

	/*
		Temp == degrees in F
		Thickness == one millipascal-second
			-Water has cP of .894
			-Within our experiement every teaspoon of cornstarch added roughly .4 cP to overall viscosity
		Actual Time == how long the ink took to hit bottom center/corner in seconds
	*/
	float temp = 70;
	float water_viscosity = .894;
	float amount_of_cornstarch = 0;
	float total_viscosity = water_viscosity + amount_of_cornstarch*.4;
	chrono::seconds actualTime(7);

	//Result in seconds with BASE_SIMULATION
	chrono::seconds timeBS = base_Simulation(temp, total_viscosity);
	cout << "Time to evenly spread (base simulation) = " 
		 << chrono::duration_cast<chrono::seconds>(timeBS).count() 
		 << " seconds" << endl;
	cout << "Difference with the actual time = " 
		 << chrono::duration_cast<chrono::seconds>(actualTime - timeBS).count()
		 << " seconds" << endl;


	//Result in seconds with NAVIER_STOKES
	chrono::seconds timeNS = navier_Stokes(temp, total_viscosity);
	cout << "\nTime to spread (navier stokes) = " 
		 << chrono::duration_cast<chrono::seconds>(timeNS).count() 
		 << " seconds" << endl;
	cout << "Difference with the actual time = " 
		 << chrono::duration_cast<chrono::seconds>(actualTime - timeNS).count()
		 << " seconds" << endl;

	//Pause the screen
	cin >> topMidValue;
}
