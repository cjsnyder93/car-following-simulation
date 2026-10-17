#include <iostream>
#include <assert.h>
#include "Car.h"

using namespace std;


// ============================================================================
// Initialize a Car object.
//
// The function receives a pointer to a Car and stores the supplied initial
// values in the corresponding data members.
//
// Using a pointer allows the function to modify the original Car object
// rather than a copy of it.
// ============================================================================
void InitCar(Car* car,
	double sensitivity,
	double timeLag,
	double initialX,
	double initialV,
	double initialA)
{
	// The arrow operator (->) is used to access a data member through a pointer.
	car->sensitivity = sensitivity;
	car->timeLag = timeLag;
	car->position = initialX;
	car->velocity = initialV;
	car->acceleration = initialA;
}


// ============================================================================
// Update position and velocity for one time step.
//
// This function uses a simple numerical integration method:
//
//     new position = old position + velocity * deltaT
//     new velocity = old velocity + acceleration * deltaT
//
// deltaT is the amount of simulation time represented by one update.
// ============================================================================
void UpdatePosVel(double deltaT, Car& car)
{
	// Update position using the current velocity.
	car.position = car.position + car.velocity * deltaT;

	// Update velocity using the current acceleration.
	car.velocity = car.velocity + car.acceleration * deltaT;
}


// ============================================================================
// Update a following car.
//
// A following car determines its acceleration from the distance to the
// vehicle immediately ahead of it and its current velocity.
//
// The model first updates position and velocity. It then calculates the
// acceleration that will be used during the next time step.
//
// deltaT   - simulation time step
// follower - car being updated
// leader   - car immediately ahead of the follower
// ============================================================================
void UpdateCar(double deltaT, Car& follower, Car& leader)
{
	// Calculate the distance between the two cars.
	//
	// The 10-foot term represents the desired spacing between cars.
	// Positions are stored in miles, so 10 feet must be converted to miles.
	double d = leader.position
		- follower.position
		- (10.0 / (1760.0 * 3.0));

	// Update the follower's position and velocity for this time step.
	UpdatePosVel(deltaT, follower);

	// Calculate the follower's new acceleration.
	//
	// The acceleration depends on:
	//   1. the driver's sensitivity,
	//   2. the distance from the leader, and
	//   3. the driver's reaction time (timeLag).
	follower.acceleration =
		follower.sensitivity *
		(d - follower.velocity * follower.timeLag);
}


// ============================================================================
// Update the leader.
//
// Unlike the following cars, the leader does not respond to another car.
// Instead, its behavior is specified by a script based on simulation time.
//
// The leader:
//   1. accelerates initially,
//   2. travels at 60 mph,
//   3. decelerates,
//   4. stops.
//
// time   - current simulation time, in hours
// deltaT - simulation time step, in hours
// leader - car being updated
// ============================================================================
void UpdateLeader(double time, double deltaT, Car& leader)
{
	// First update the leader's position and velocity.
	UpdatePosVel(deltaT, leader);


	// ------------------------------------------------------------------------
	// Phase 1: Accelerate.
	//
	// 8 seconds is converted to hours because simulation time is measured
	// in hours.
	// ------------------------------------------------------------------------
	if (time < (8.0 / 3600.0)) {
		leader.acceleration = 27000.0;
	}


	// ------------------------------------------------------------------------
	// Phase 2: Maintain a velocity of 60 mph.
	//
	// Acceleration is zero because the leader is no longer speeding up.
	// ------------------------------------------------------------------------
	else if (time < 20.0 / 3600.0) {
		leader.acceleration = 0;
		leader.velocity = 60.0;
	}


	// ------------------------------------------------------------------------
	// Phase 3: Decelerate.
	// ------------------------------------------------------------------------
	else if (time < 25.5 / 3600.0) {
		leader.acceleration = -40000;
	}


	// ------------------------------------------------------------------------
	// Phase 4: Stop.
	//
	// Both acceleration and velocity are set to zero so that the leader
	// remains stopped.
	// ------------------------------------------------------------------------
	else {
		leader.acceleration = 0;
		leader.velocity = 0;
	}
}


// ============================================================================
// Print one car's state.
//
// Position is converted from miles back to feet for easier interpretation
// when looking at the simulation output.
//
// The output contains:
//
//     position, velocity, acceleration
// ============================================================================
void PrintCar(Car car)
{
	cout << car.position * 1760 * 3
		<< ", "
		<< car.velocity
		<< ", "
		<< car.acceleration;
}


// ============================================================================
// Cars
//
// This structure represents a collection of Car objects.
//
// carArray         - dynamically allocated array containing the cars
// numCars          - current number of cars in the array
// currentCarIndex  - index used by GetFirstCar() and GetNextCar()
// ============================================================================
struct Cars
{
	Car* carArray;
	int numCars;

	// Stores the index of the most recently returned car.
	int currentCarIndex;
};


// ============================================================================
// Create a collection of cars.
//
// Dynamic memory is used because the number of cars can change during the
// simulation. In particular, InsertCar() and RemoveCar() can change the
// size of the collection.
//
// n - number of cars to create
//
// Returns a pointer to the newly allocated Cars structure.
// ============================================================================
Cars* CreateCars(int n)
{
	// Allocate memory for the Cars structure.
	Cars* cars = new Cars;

	// Store the number of cars.
	cars->numCars = n;

	// Allocate an array containing n Car objects.
	cars->carArray = new Car[n];

	return cars;
}


// ============================================================================
// GetFirstCar
//
// Return a pointer to the first car in the collection.
//
// If there are no cars, nullptr is returned.
//
// The function also sets currentCarIndex to 0 so that a subsequent call to
// GetNextCar() will return the second car.
// ============================================================================
Car* GetFirstCar(Cars* cars)
{
	// Check whether the collection is empty.
	if (cars->numCars == 0) {
		return nullptr;
	}
	else {
		// The first car has array index 0.
		cars->currentCarIndex = 0;

		// Return the address of the first Car object.
		return &cars->carArray[0];
	}
}


// ============================================================================
// GetNextCar
//
// Return a pointer to the next car in the collection.
//
// currentCarIndex keeps track of which car was returned most recently.
//
// If another car exists, increment the index and return its address.
// Otherwise, return nullptr.
// ============================================================================
Car* GetNextCar(Cars* cars)
{
	// Determine whether another car exists.
	if (cars->currentCarIndex < cars->numCars - 1) {

		// Move to the next array element.
		cars->currentCarIndex++;

		// Return the address of the next car.
		return &cars->carArray[cars->currentCarIndex];
	}
	else {
		// There are no more cars.
		return nullptr;
	}
}


// ============================================================================
// UpdateCars
//
// Update all cars for one simulation time step.
//
// The following cars are updated in reverse order. Each following car uses
// the car immediately ahead of it as its leader.
//
// For example, with four cars:
//
//     Car 0    Car 1    Car 2    Car 3
//     leader   follower follower follower
//
// The loop updates:
//
//     Car 3 using Car 2
//     Car 2 using Car 1
//     Car 1 using Car 0
//
// The leader (Car 0) is then updated separately.
//
// Updating the followers first means that each follower uses the state of
// the car ahead from the beginning of the current time step.
// ============================================================================
void UpdateCars(double t, double deltaT, Cars* cars)
{
	// Start with the last car and work toward the leader.
	for (int i = cars->numCars - 1; i > 0; --i) {

		// Each car uses the previous array element as its leader.
		UpdateCar(deltaT,
			cars->carArray[i],
			cars->carArray[i - 1]);
	}

	// The first car is the leader and follows its own scripted behavior.
	UpdateLeader(t, deltaT, cars->carArray[0]);
}


// ============================================================================
// PrintCars
//
// Print the simulation time followed by the state of every car.
//
// Each car's output consists of:
//
//     position, velocity, acceleration
//
// The output is written as a comma-separated sequence, making it convenient
// to import into a spreadsheet or other data-analysis program.
// ============================================================================
void PrintCars(Cars* cars, double time)
{
	// Print the simulation time first.
	cout << time << ", ";

	// Visit every car in the array.
	for (int i = 0; i <= cars->numCars - 1; i++) {

		// Print this car's state.
		PrintCar(cars->carArray[i]);

		// Separate this car from the next value.
		cout << ", ";
	}

	// Move to the next line of output.
	cout << endl;
}


// ============================================================================
// InsertCar
//
// Insert a new car into the array at the specified position.
//
// Because a C++ built-in array has a fixed size, a larger array must be
// dynamically allocated. The existing cars are then copied into the new
// array, leaving an empty position for the new car.
//
// position - array index where the new car will be inserted
//
// This implementation assumes that the new car is not inserted at either
// end of the line of cars.
// ============================================================================
void InsertCar(Cars* cars, int position)
{
	// Verify that the requested position is valid.
	//
	// assert() terminates the program if the condition is false.
	assert(position > 0 && position < cars->numCars - 1);


	// Allocate an array containing space for one additional car.
	Car* newCars = new Car[cars->numCars + 1];


	// Copy all cars before the insertion position.
	for (int i = 0; i < position; ++i) {
		newCars[i] = cars->carArray[i];
	}


	// Copy the remaining cars one position to the right.
	//
	// This leaves newCars[position] available for the new car.
	for (int i = position; i < cars->numCars; ++i) {
		newCars[i + 1] = cars->carArray[i];
	}


	// Initially make the new car a copy of the car in front of it.
	newCars[position] = newCars[position - 1];


	// Place the new car halfway between its two neighboring cars.
	newCars[position].position =
		(newCars[position - 1].position +
			newCars[position + 1].position) / 2.0;


	// The collection now contains one additional car.
	cars->numCars++;


	// Save the address of the old array before replacing it.
	Car* oldCars = cars->carArray;

	// Make the Cars structure point to the new array.
	cars->carArray = newCars;

	// Release the memory occupied by the old array.
	delete[] oldCars;
}


// ============================================================================
// RemoveCar
//
// Remove a car from the collection.
//
// Because a C++ built-in array cannot be resized, a smaller array is created.
// All cars except the one being removed are copied into the new array.
//
// position - array index of the car to remove
//
// This implementation assumes that the first and last cars cannot be
// removed.
// ============================================================================
void RemoveCar(Cars* cars, int position)
{
	// Verify that the requested position is valid.
	assert(position > 0 && position < cars->numCars - 1);


	// Allocate an array with space for one fewer car.
	Car* newCars = new Car[cars->numCars - 1];


	// Copy all cars before the one being removed.
	for (int i = 0; i < position; ++i) {
		newCars[i] = cars->carArray[i];
	}


	// Copy all cars after the removed car.
	//
	// Each car after the removed car moves one position to the left.
	for (int i = position; i < cars->numCars - 1; ++i) {
		newCars[i] = cars->carArray[i + 1];
	}


	// The collection now contains one fewer car.
	cars->numCars--;


	// Save the address of the old array.
	Car* oldCars = cars->carArray;

	// Replace the old array with the smaller array.
	cars->carArray = newCars;

	// Release the memory occupied by the old array.
	delete[] oldCars;
}