/*
 * RollerCoaster.cpp
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <pthread.h>
#include <iostream>
#include "RollerCoaster.h"

using namespace std;

RollerCoaster::RollerCoaster(int carCapacity, int numCars) : CAR_CAPACITY(carCapacity), NUM_CARS(numCars) {
	passengersInCar = new int[numCars];
	nextCarRequestLoad = 0;
	nextCarToUnload = 0;
	nextCarToLoad = 0;
	nextCarToLoadIsWaitingPassengers = false;
	pthread_mutex_init(&rollerCoasterLock, NULL);
	pthread_cond_init(&carLoad, NULL);
	pthread_cond_init(&carUnload, NULL);
	pthread_cond_init(&carIsFull, NULL);
	pthread_cond_init(&nextCarCanLoad, NULL);
}

RollerCoaster::~RollerCoaster() {
}

void RollerCoaster::takeRide(const int passengerId) {
	pthread_mutex_lock(&rollerCoasterLock);

	//Waits for an available car:
	while (!nextCarToLoadIsWaitingPassengers || passengersInCar[nextCarToLoad] == CAR_CAPACITY) {
		cout << "Passenger " << passengerId << ": Waiting for a car" << endl;
		pthread_cond_wait(&carLoad, &rollerCoasterLock);
	}

	const int ridingCar = nextCarToLoad;
	passengersInCar[ridingCar]++;
	cout << "Passenger " << passengerId << ": Will take ride in the car " << ridingCar << endl;
	if (passengersInCar[ridingCar] == CAR_CAPACITY) {
		//Notifies the car that it is full:
		pthread_cond_signal(&carIsFull);
	}
	//Waits until the car gets full:
	pthread_cond_wait(&carIsFull, &rollerCoasterLock);
	//Waits the end of the ride:
	pthread_cond_wait(&carUnload, &rollerCoasterLock);
	cout << "Passenger " << passengerId << ": Left the car " << ridingCar << endl;

	pthread_mutex_unlock(&rollerCoasterLock);
}

void RollerCoaster::load() {
	pthread_mutex_lock(&rollerCoasterLock);

	const int thisCar = nextCarRequestLoad;
	//Updates the index of the next car that will carry passengers:
	nextCarRequestLoad = (nextCarRequestLoad + 1) % NUM_CARS;
	//If this car is not the next car to go around then waits its successor car to leave:
	if (thisCar != nextCarToLoad) {
		pthread_cond_wait(&nextCarCanLoad, &rollerCoasterLock);
	}
	nextCarToLoadIsWaitingPassengers = true;
	//Notifies the passengers the car is ready to load them:
	pthread_cond_broadcast(&carLoad);
	cout << "Car " << thisCar << ": Waiting for passengers" << endl;
	//Waits until the car gets full:
	pthread_cond_wait(&carIsFull, &rollerCoasterLock);
	nextCarToLoadIsWaitingPassengers = false;
	//Notifies the passengers that the car is full:
	pthread_cond_broadcast(&carIsFull);
	cout << "Car " << thisCar << ": The car is full. Going around..." << endl << endl;
	nextCarToLoad = (nextCarToLoad + 1) % NUM_CARS;
	//Awakes up the next car, i.e. its predecessor:
	pthread_cond_signal(&nextCarCanLoad);

	pthread_mutex_unlock(&rollerCoasterLock);
}

void RollerCoaster::unload() {
	pthread_mutex_lock(&rollerCoasterLock);

	//Unload the passengers:
	for (int i = 0; i < CAR_CAPACITY; i++) {
		pthread_cond_signal(&carUnload);
	}
	passengersInCar[nextCarToUnload] = 0;
	cout << "Car " << nextCarToUnload << ": Unloaded its passengers" << endl;
	nextCarToUnload = (nextCarToUnload + 1) % NUM_CARS;

	pthread_mutex_unlock(&rollerCoasterLock);
}
