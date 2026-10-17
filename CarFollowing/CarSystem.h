#pragma once
#include <iostream>

struct Car
{
    int id;
    double position;
    double velocity;
    double acceleration;
    double sensitivity;
    double timeLag;
};

class CarSystem {
public:
    CarSystem(int n);
    ~CarSystem();

    Car* GetFirstCar();
    Car* GetNextCar();

    // Public because main.cpp uses it
    void InitCar(Car& car,
        double sensitivity,
        double timeLag,
        double initialX,
        double initialV,
        double initialA);

    void Update(double t, double deltaT);
    void Print(double time);

    void InsertCar(int position);
    void RemoveCar(int position);

private:
    struct CarNode {
        Car data;
        CarNode* next;
        CarNode* prev;
    };

    CarNode* head;
    CarNode* tail;
    CarNode* current;
    int numCars;

    void UpdatePosVel(double deltaT, Car& car);
    void UpdateCar(double deltaT, Car& follower, Car& leader);
    void UpdateLeader(double time, double deltaT, Car& leader);
};

