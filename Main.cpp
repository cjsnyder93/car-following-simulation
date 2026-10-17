#include <iostream>
#include "Car.h"

using namespace std;


// ============================================================================
// main
//
// This program simulates a line of cars following a leader.
//
// The simulation demonstrates several important programming concepts:
//
//   * structures
//   * pointers
//   * dynamically allocated memory
//   * arrays
//   * functions
//   * loops
//   * conditional statements
//   * numerical simulation
//   * changing the size of a data structure during a simulation
//
// The simulation runs for 30 seconds. At 15 seconds, a car is removed from
// the line. The InsertCar() call can be used instead to experiment with
// adding a car.
// ============================================================================
void main()
{
	// ------------------------------------------------------------------------
	// Unit conversions
	// ------------------------------------------------------------------------

	// Number of feet in one mile.
	//
	// 1 mile = 1760 yards
	// 1 yard = 3 feet
	const double fpm = 1760.0 * 3.0;

	// Number of seconds in one hour.
	const double sph = 3600.0;


	// ------------------------------------------------------------------------
	// Simulation parameters
	//
	// Values are supplied in both seconds/feet and hours/miles where useful.
	// The simulation calculations use consistent units, while some values
	// are specified in units that are easier for people to understand.
	// ------------------------------------------------------------------------

	// Number of cars initially in the line.
	const int numCars = 4;

	// Simulation time step in seconds.
	const double deltaTs = 0.1;

	// Simulation time step converted from seconds to hours.
	const double deltaTh = deltaTs / sph;

	// Driver sensitivity parameter.
	const double sensitivityFS = 0.4;

	// Convert the sensitivity parameter to the units used by the model.
	const double sensitivityMH =
		sensitivityFS * 1760.0 * 3.0 * 3600.0 * 10.0;

	// Driver reaction time in seconds.
	const double timeLags = 1.5;

	// Driver reaction time converted to hours.
	const double timeLagh = timeLags / sph;

	// Initial position of the leader.
	const double initialX = 0.0;

	// Initial velocity of all cars.
	const double initialV = 0.0;

	// Initial acceleration of the following cars.
	const double initialA = 0.0;

	// Initial acceleration of the leader.
	const double leaderA = 27000;


	// ------------------------------------------------------------------------
	// Create the collection of cars.
	//
	// CreateCars() dynamically allocates the Cars structure and the array
	// containing the individual Car objects.
	// ------------------------------------------------------------------------
	Cars* cars = CreateCars(numCars);


	// ------------------------------------------------------------------------
	// Initialize the leader.
	//
	// GetFirstCar() returns a pointer to the first car in the collection.
	// This car becomes the leader.
	//
	// The leader starts at position 0.
	// ------------------------------------------------------------------------
	Car* carPtr = GetFirstCar(cars);

	InitCar(carPtr,
		sensitivityMH,
		timeLagh,
		initialX / fpm,
		initialV,
		leaderA);


	// ------------------------------------------------------------------------
	// Initialize the following cars.
	//
	// Each following car begins 10 feet behind the car immediately ahead.
	//
	// The loop starts at 1 because car 0 is the leader.
	// GetNextCar() returns a pointer to the next car in the collection.
	// ------------------------------------------------------------------------
	for (int i = 1; i < numCars; i++) {

		// Get the next car in the collection.
		carPtr = GetNextCar(cars);

		// Place this car 10*i feet behind the leader's initial position.
		InitCar(carPtr,
			sensitivityMH,
			timeLagh,
			(initialX - 10.0 * i) / fpm,
			initialV,
			initialA);
	}


	// ------------------------------------------------------------------------
	// Print the initial state of the simulation.
	// ------------------------------------------------------------------------
	PrintCars(cars, 0.0);


	// Keep track of simulation time in hours because the model uses hours
	// internally.
	double timeh = deltaTh;


	// ------------------------------------------------------------------------
	// Run the first part of the simulation.
	//
	// The simulation runs from 0.1 seconds through 15 seconds.
	//
	// Each iteration:
	//   1. updates all cars,
	//   2. prints their states,
	//   3. advances the simulation time.
	// ------------------------------------------------------------------------
	for (double times = deltaTs;
		times <= 15.0;
		times += deltaTs) {

		// Advance the model by one time step.
		UpdateCars(timeh, deltaTh, cars);

		// Print the state of all cars at this time.
		PrintCars(cars, times);

		// Advance the internal simulation time.
		timeh += deltaTh;
	}


	// ------------------------------------------------------------------------
	// Change the number of cars.
	//
	// Uncomment InsertCar() to test inserting a car into the line.
	//
	// The current program removes car 2 instead.
	// Remember that array indexing begins at zero:
	//
	//     position 0 -> first car
	//     position 1 -> second car
	//     position 2 -> third car
	//     ...
	// ------------------------------------------------------------------------

	InsertCar(cars, 2);
	RemoveCar(cars, 2);


	// ------------------------------------------------------------------------
	// Continue the simulation from 15 seconds through 30 seconds.
	//
	// Notice that the number of cars is now different because a car was
	// removed above.
	// ------------------------------------------------------------------------
	for (double times = 15.0 + deltaTs;
		times <= 30.0;
		times += deltaTs) {

		// Advance the model by one time step.
		UpdateCars(timeh, deltaTh, cars);

		// Print the current state of all remaining cars.
		PrintCars(cars, times);

		// Advance the internal simulation time.
		timeh += deltaTh;
	}
}