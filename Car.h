#pragma once

// ============================================================================
// Car.h
//
// This header file defines the data stored for a single car and declares
// the functions that can be used to manipulate cars.
//
// The implementation of these functions is contained in Car.cpp.
// ============================================================================


// ----------------------------------------------------------------------------
// Car
//
// A Car object stores the current state and behavioral parameters for one
// vehicle in the simulation.
//
// The position and velocity are expressed in miles and miles/hour,
// respectively. Acceleration and the other parameters use the units required
// by the simulation model.
// ----------------------------------------------------------------------------
struct Car
{
	int id;                 // Identifier for the car
	double position;        // Current position of the car
	double velocity;        // Current velocity of the car
	double acceleration;    // Current acceleration of the car
	double sensitivity;     // How strongly the driver responds to conditions
	double timeLag;         // Driver reaction time
};


// ----------------------------------------------------------------------------
// Initialize a car.
//
// car          - pointer to the Car object to initialize
// sensitivity  - driver's sensitivity to the car ahead
// timeLag      - driver's reaction time
// initialX     - initial position
// initialV     - initial velocity
// initialA     - initial acceleration
// ----------------------------------------------------------------------------
void InitCar(Car* car,
	double sensitivity,
	double timeLag,
	double initialX,
	double initialV,
	double initialA);


// ----------------------------------------------------------------------------
// Update a following car.
//
// The follower changes its position and velocity and then calculates a new
// acceleration based on its distance from the leader.
//
// deltaT   - length of the simulation time step
// follower - car being updated
// leader   - car immediately ahead of the follower
// ----------------------------------------------------------------------------
void UpdateCar(double deltaT, Car* follower, Car leader);


// ----------------------------------------------------------------------------
// Update the leader.
//
// The leader follows a predefined sequence of acceleration, constant-speed,
// and deceleration behavior.
//
// time    - current simulation time
// deltaT  - length of the simulation time step
// leader  - car being updated
// ----------------------------------------------------------------------------
void UpdateLeader(double time, double deltaT, Car* leader);


// ----------------------------------------------------------------------------
// Print the state of one car.
//
// The current position, velocity, and acceleration are written to the
// standard output stream.
// ----------------------------------------------------------------------------
void PrintCar(Car car);


// ============================================================================
// Cars
//
// Cars is a data structure used to represent a collection of Car objects.
//
// The actual definition of the structure is intentionally hidden from code
// that includes this header file. This is called a forward declaration.
//
// The complete definition appears in Car.cpp.
// ============================================================================
struct Cars;


// ----------------------------------------------------------------------------
// Create a collection containing n cars.
//
// n - number of cars to create
//
// Returns:
//     A pointer to the newly created Cars object.
// ----------------------------------------------------------------------------
Cars* CreateCars(int n);


// ----------------------------------------------------------------------------
// Update every car in the collection.
//
// t      - current simulation time
// deltaT - length of the simulation time step
// cars   - collection of cars to update
// ----------------------------------------------------------------------------
void UpdateCars(double t, double deltaT, Cars* cars);


// ----------------------------------------------------------------------------
// Get the first car in the collection.
//
// Returns:
//     A pointer to the first Car object.
//     nullptr if the collection contains no cars.
//
// This function also sets the current-car index used by GetNextCar().
// ----------------------------------------------------------------------------
Car* GetFirstCar(Cars* cars);


// ----------------------------------------------------------------------------
// Get the next car in the collection.
//
// Returns:
//     A pointer to the next Car object.
//     nullptr if there are no more cars.
//
// GetNextCar() uses the current-car index maintained by the Cars structure.
// ----------------------------------------------------------------------------
Car* GetNextCar(Cars* cars);


// ----------------------------------------------------------------------------
// Print the state of every car in the collection.
//
// cars - collection of cars to print
// time - simulation time associated with the output
// ----------------------------------------------------------------------------
void PrintCars(Cars* cars, double time);


// ----------------------------------------------------------------------------
// Insert a new car into the collection.
//
// position - array position where the new car should be inserted
//
// The current implementation assumes that the new car is inserted somewhere
// between the first and last cars.
// ----------------------------------------------------------------------------
void InsertCar(Cars* cars, int position);


// ----------------------------------------------------------------------------
// Remove a car from the collection.
//
// position - array position of the car to remove
//
// The current implementation assumes that the first and last cars cannot
// be removed.
// ----------------------------------------------------------------------------
void RemoveCar(Cars* cars, int position);