#include <iostream>
#include "CarSystem.h"   // your new class header

using namespace std;

int main()
{
    // ------------------------------------------------------------------------
    // Unit conversions
    // ------------------------------------------------------------------------
    const double fpm = 1760.0 * 3.0;
    const double sph = 3600.0;

    // ------------------------------------------------------------------------
    // Simulation parameters
    // ------------------------------------------------------------------------
    const int numCars = 5;
    const double deltaTs = 0.1;
    const double deltaTh = deltaTs / sph;

    const double sensitivityFS = 0.4;
    const double sensitivityMH =
        sensitivityFS * 1760.0 * 3.0 * 3600.0 * 10.0;

    const double timeLags = 1.5;
    const double timeLagh = timeLags / sph;

    const double initialX = 0.0;
    const double initialV = 0.0;
    const double initialA = 0.0;
    const double leaderA = 27000;

    // ------------------------------------------------------------------------
    // Create the car system (class version)
    // ------------------------------------------------------------------------
    CarSystem cars(numCars);

    // Initialize leader
    Car* carPtr = cars.GetFirstCar();
    cars.InitCar(*carPtr,
        sensitivityMH,
        timeLagh,
        initialX / fpm,
        initialV,
        leaderA);

    // Initialize followers
    for (int i = 1; i < numCars; i++) {
        carPtr = cars.GetNextCar();
        cars.InitCar(*carPtr,
            sensitivityMH,
            timeLagh,
            (initialX - 10.0 * i) / fpm,
            initialV,
            initialA);
    }

    // Print initial state
    cars.Print(0.0);

    double timeh = deltaTh;

    // ------------------------------------------------------------------------
    // First half of simulation (0.1s → 15s)
    // ------------------------------------------------------------------------
    for (double times = deltaTs; times <= 15.0; times += deltaTs) {

        cars.Update(timeh, deltaTh);
        cars.Print(times);

        timeh += deltaTh;
    }

    // ------------------------------------------------------------------------
    // Remove car at position 2
    // ------------------------------------------------------------------------
    cars.RemoveCar(2);

    // ------------------------------------------------------------------------
    // Second half of simulation (15s → 30s)
    // ------------------------------------------------------------------------
    for (double times = 15.0 + deltaTs; times <= 30.0; times += deltaTs) {

        cars.Update(timeh, deltaTh);
        cars.Print(times);

        timeh += deltaTh;
    }

    return 0;
}
