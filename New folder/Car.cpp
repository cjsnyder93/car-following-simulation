#include <iostream>
#include <assert.h>
#include "Car.h"

using namespace std;

// ============================================================================
// Linked list node for each car
// ============================================================================
struct CarNode {
    Car data;
    CarNode* next;
    CarNode* prev;
};

// ============================================================================
// Cars list (replaces array version)
// ============================================================================
struct Cars {
    CarNode* head;
    CarNode* tail;
    CarNode* current;
    int numCars;
};

// ============================================================================
// Initialize a Car object (unchanged)
// ============================================================================
void InitCar(Car* car,
    double sensitivity,
    double timeLag,
    double initialX,
    double initialV,
    double initialA)
{
    car->sensitivity = sensitivity;
    car->timeLag = timeLag;
    car->position = initialX;
    car->velocity = initialV;
    car->acceleration = initialA;
}

// ============================================================================
// Update position and velocity (unchanged)
// ============================================================================
void UpdatePosVel(double deltaT, Car& car)
{
    car.position += car.velocity * deltaT;
    car.velocity += car.acceleration * deltaT;
}

// ============================================================================
// Update follower (unchanged)
// ============================================================================
void UpdateCar(double deltaT, Car& follower, Car& leader)
{
    double d = leader.position
        - follower.position
        - (10.0 / (1760.0 * 3.0));

    UpdatePosVel(deltaT, follower);

    follower.acceleration =
        follower.sensitivity *
        (d - follower.velocity * follower.timeLag);
}

// ============================================================================
// Update leader (unchanged)
// ============================================================================
void UpdateLeader(double time, double deltaT, Car& leader)
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

// ============================================================================
// Print one car (unchanged)
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
// CreateCars — now builds a linked list instead of an array
// ============================================================================
Cars* CreateCars(int n)
{
    Cars* cars = new Cars;
    cars->head = nullptr;
    cars->tail = nullptr;
    cars->current = nullptr;
    cars->numCars = n;

    for (int i = 0; i < n; i++) {
        CarNode* node = new CarNode;

        // Same initialization logic as before
        InitCar(&node->data, 20000, 0.1, -0.01 * i, 0, 0);

        node->next = nullptr;
        node->prev = cars->tail;

        if (cars->tail)
            cars->tail->next = node;
        else
            cars->head = node;

        cars->tail = node;
    }

    return cars;
}

// ============================================================================
// GetFirstCar — now returns head->data
// ============================================================================
Car* GetFirstCar(Cars* cars)
{
    if (cars->head == nullptr)
        return nullptr;

    cars->current = cars->head;
    return &cars->current->data;
}

// ============================================================================
// GetNextCar — now uses current->next
// ============================================================================
Car* GetNextCar(Cars* cars)
{
    if (cars->current == nullptr || cars->current->next == nullptr)
        return nullptr;

    cars->current = cars->current->next;
    return &cars->current->data;
}

// ============================================================================
// UpdateCars — reverse traversal using tail->prev
// ============================================================================
void UpdateCars(double t, double deltaT, Cars* cars)
{
    CarNode* node = cars->tail;

    // Followers first
    while (node && node->prev) {
        UpdateCar(deltaT, node->data, node->prev->data);
        node = node->prev;
    }

    // Leader last
    UpdateLeader(t, deltaT, cars->head->data);
}

// ============================================================================
// PrintCars — forward traversal using head->next
// ============================================================================
void PrintCars(Cars* cars, double time)
{
    cout << time << ", ";

    CarNode* node = cars->head;
    while (node) {
        PrintCar(node->data);
        cout << ", ";
        node = node->next;
    }

    cout << endl;
}

// ============================================================================
// InsertCar — linked list version
// ============================================================================
void InsertCar(Cars* cars, int position)
{
    assert(position > 0 && position < cars->numCars - 1);

    CarNode* node = cars->head;
    for (int i = 0; i < position; i++)
        node = node->next;

    CarNode* newNode = new CarNode;

    // Copy car in front
    newNode->data = node->prev->data;

    // Place halfway between neighbors
    newNode->data.position =
        (node->prev->data.position + node->data.position) / 2.0;

    // Insert into list
    newNode->prev = node->prev;
    newNode->next = node;

    node->prev->next = newNode;
    node->prev = newNode;

    cars->numCars++;
}

// ============================================================================
// RemoveCar — linked list version
// ============================================================================
void RemoveCar(Cars* cars, int position)
{
    assert(position > 0 && position < cars->numCars - 1);

    CarNode* node = cars->head;
    for (int i = 0; i < position; i++)
        node = node->next;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    delete node;
    cars->numCars--;
}
