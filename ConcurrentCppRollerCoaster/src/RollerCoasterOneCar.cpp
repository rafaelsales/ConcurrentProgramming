/*
 * RollerCoasterOneCar.cpp
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <pthread.h>
#include <iostream>
#include "RollerCoasterOneCar.h"

using namespace std;

RollerCoasterOneCar::RollerCoasterOneCar(int carCapacity) : CAR_CAPACITY(carCapacity) {
	passengersInCar = 0;
	carIsWaitingPassengers = false;
	pthread_mutex_init(&rollerCoasterLock, NULL);
	pthread_cond_init(&carLoad, NULL);
	pthread_cond_init(&carUnload, NULL);
	pthread_cond_init(&carIsFull, NULL);
}

RollerCoasterOneCar::~RollerCoasterOneCar() {
}

void RollerCoasterOneCar::takeRide(const int passengerId) {
	pthread_mutex_lock(&rollerCoasterLock);

	//Waits the car become available:
	while (!carIsWaitingPassengers || passengersInCar == CAR_CAPACITY) {
		cout << "Passenger " << passengerId << ": Waiting for the car" << endl;
		pthread_cond_wait(&carLoad, &rollerCoasterLock);
	}

	passengersInCar++;
	cout << "Passenger " << passengerId << ": Will take ride in the car" << endl;
	if (passengersInCar == CAR_CAPACITY) {
		//Notifies the car that it is full:
		pthread_cond_signal(&carIsFull);
	}
	//Waits until the car gets full:
	pthread_cond_wait(&carIsFull, &rollerCoasterLock);
	//Waits the end of the ride:
	pthread_cond_wait(&carUnload, &rollerCoasterLock);
	cout << "Passenger " << passengerId << ": Left the car" << endl;

	pthread_mutex_unlock(&rollerCoasterLock);
}

void RollerCoasterOneCar::load() {
	pthread_mutex_lock(&rollerCoasterLock);

	//Waits for the car if it is riding:
	while (passengersInCar > 0) {
		pthread_cond_wait(&carUnload, &rollerCoasterLock);
	}
	carIsWaitingPassengers = true;
	//Notifies the passengers the car is ready to load them:
	pthread_cond_broadcast(&carLoad);
	cout << "Car: Waiting for passengers" << endl;
	//Waits until the car gets full:
	pthread_cond_wait(&carIsFull, &rollerCoasterLock);
	carIsWaitingPassengers = false;
	//Notifies the passengers that the car is full:
	pthread_cond_broadcast(&carIsFull);
	cout << "Car: The car is full. Going around...\n" << endl;

	pthread_mutex_unlock(&rollerCoasterLock);
}

void RollerCoasterOneCar::unload() {
	pthread_mutex_lock(&rollerCoasterLock);

	//Unload the passengers:
	pthread_cond_broadcast(&carUnload);
	passengersInCar = 0;
	cout << "Car: Unloaded its passengers" << endl;

	pthread_mutex_unlock(&rollerCoasterLock);
}
