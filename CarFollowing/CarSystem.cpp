#include "CarSystem.h"
#include <iostream>
#include <cassert>

// --- Constructor --- my list of cars
CarSystem::CarSystem(int n) : head(nullptr), tail(nullptr), current(nullptr), numCars(n)
{
    for (int i = 0; i < n; i++) {
        CarNode* node = new CarNode;

        InitCar(node->data, 20000, 0.1, -0.01 * i, 0, 0);

        node->next = nullptr;
        node->prev = tail;

        if (tail)
            tail->next = node;
        else
            head = node;

        tail = node;
    }
}

// --- Destructor --- clean up my memory
CarSystem::~CarSystem()
{
    CarNode* node = head;
    while (node) {
        CarNode* next = node->next;
        delete node;
        node = next;
    }
}

// --- Internal helpers ---
void CarSystem::InitCar(Car& car,
    double sensitivity,
    double timeLag,
    double initialX,
    double initialV,
    double initialA)
{
    car.sensitivity = sensitivity;
    car.timeLag = timeLag;
    car.position = initialX;
    car.velocity = initialV;
    car.acceleration = initialA;
}

void CarSystem::UpdatePosVel(double deltaT, Car& car)
{
    car.position += car.velocity * deltaT;
    car.velocity += car.acceleration * deltaT;
}

void CarSystem::UpdateCar(double deltaT, Car& follower, Car& leader)
{
    double d = leader.position
        - follower.position
        - (10.0 / (1760.0 * 3.0));

    UpdatePosVel(deltaT, follower);

    follower.acceleration =
        follower.sensitivity *
        (d - follower.velocity * follower.timeLag);
}

void CarSystem::UpdateLeader(double time, double deltaT, Car& leader)
{
    UpdatePosVel(deltaT, leader);

    if (time < (8.0 / 3600.0)) {
        leader.acceleration = 27000.0;
    }
    else if (time < 20.0 / 3600.0) {
        leader.acceleration = 0;
        leader.velocity = 60.0;
    }
    else if (time < 25.5 / 3600.0) {
        leader.acceleration = -40000.0;
    }
    else {
        leader.acceleration = 0;
        leader.velocity = 0;
    }
}

// --- Iteration --- for main.cpp to get the cars
Car* CarSystem::GetFirstCar()
{
    current = head;
    return current ? &current->data : nullptr;
}

Car* CarSystem::GetNextCar()
{
    if (!current || !current->next)
        return nullptr;

    current = current->next;
    return &current->data;
}

// --- Simulation update ---
void CarSystem::Update(double t, double deltaT)
{
    CarNode* node = tail;

    while (node && node->prev) {
        UpdateCar(deltaT, node->data, node->prev->data);
        node = node->prev;
    }

    UpdateLeader(t, deltaT, head->data);
}

// --- Printing --- output car states to console
void CarSystem::Print(double time)
{
    std::cout << time << ", ";

    CarNode* node = head;
    while (node) {
        std::cout << node->data.position * 1760 * 3
            << ", "
            << node->data.velocity
            << ", "
            << node->data.acceleration
            << ", ";
        node = node->next;
    }

    std::cout << std::endl;
}

// --- Insert a car ---
void CarSystem::InsertCar(int position)
{
    assert(position > 0 && position < numCars - 1);

    CarNode* node = head;
    for (int i = 0; i < position; i++)
        node = node->next;

    CarNode* newNode = new CarNode;

    newNode->data = node->prev->data;

    newNode->data.position =
        (node->prev->data.position + node->data.position) / 2.0;

    newNode->prev = node->prev;
    newNode->next = node;

    node->prev->next = newNode;
    node->prev = newNode;

    numCars++;
}

// --- Remove a car ---
void CarSystem::RemoveCar(int position)
{
    assert(position > 0 && position < numCars - 1);

    CarNode* node = head;
    for (int i = 0; i < position; i++)
        node = node->next;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    delete node;
    numCars--;
}
