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
	float topMidValue = 100.0;
	fillCube(topMidValue);

	float temp = 70;
	float thickness = 100;
	chrono::seconds actualTime(0);

	//Result in seconds with BASE_SIMULATION
	chrono::seconds timeBS = base_Simulation(temp, thickness);
	cout << "Time to evenly spread (base simulation) = " << chrono::duration_cast<chrono::seconds>(timeBS).count() << " seconds" << endl;
	cout << "Difference with the actual time = " << chrono::duration_cast<chrono::seconds>(actualTime - timeBS).count() << endl;


	//Result in seconds with NAVIER_STOKES
	chrono::seconds timeNS = navier_Stokes(temp, thickness);
	cout << "\nTime to spread (navier stokes) = " << chrono::duration_cast<chrono::seconds>(timeNS).count() << " seconds" << endl;
	cout << "Difference with the actual time = " << chrono::duration_cast<chrono::seconds>(actualTime - timeNS).count() << endl;

	//Pause the screen
	cin >> topMidValue;
}
