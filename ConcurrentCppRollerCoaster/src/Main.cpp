/*
 * Main.cpp
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <pthread.h>
#include <sstream>
#include "RollerCoaster.h"
#include "RollerCoasterOneCar.h"

using namespace std;

pthread_attr_t threadAttributes;
RollerCoasterOneCar* rollerCoasterOneCar;
RollerCoaster* rollerCoasterSeveralCars;

void testRollerCoasterOneCar(int carCapacity, int numPassengers);
void testRollerCoasterSeveralCars(int carCapacity, int numPassengers, int numCars);

void* passengerThreadFunctionOneCar(int passengerId);
void* carThreadFunctionOneCar();

void* passengerThreadFunctionSeveralCars(int passengerId);
void* carThreadFunctionSeveralCars();

int main(int argc, char* argv[]) {
	int numCars;
	int carCapacity;
	int numPassengers;
	do {
		cout << "Number of cars: ";
		cin >> numCars;
	} while (numCars <= 0);
	do {
		cout << "Number of passengers per car: ";
		cin >> carCapacity;
	} while (carCapacity <= 0);
	do {
		cout << "Number of passengers: ";
		cin >> numPassengers;
	} while (numPassengers <= 0);
	cout << endl;

	//Initialize the thread attributes:
	pthread_attr_init(&threadAttributes);
	pthread_attr_setscope(&threadAttributes, PTHREAD_SCOPE_SYSTEM);

	if (numCars == 1) {
		testRollerCoasterOneCar(carCapacity, numPassengers);
	} else {
		testRollerCoasterSeveralCars(carCapacity, numPassengers, numCars);
	}
	pthread_exit(NULL);
	return 0;
}

void testRollerCoasterOneCar(int carCapacity, int numPassengers) {
	rollerCoasterOneCar = new RollerCoasterOneCar(carCapacity);

	pthread_t carThread;
	pthread_create(&carThread, &threadAttributes, (void*(*)(void*)) carThreadFunctionOneCar, (void*) NULL);

	pthread_t passengersThreads[numPassengers];
	for (int i = 0; i < numPassengers; i++) {
		pthread_create(&passengersThreads[i], &threadAttributes, (void*(*)(void*)) passengerThreadFunctionOneCar, (void*) i);
	}
}

void testRollerCoasterSeveralCars(int carCapacity, int numPassengers, int numCars) {
	rollerCoasterSeveralCars = new RollerCoaster(carCapacity, numCars);

	pthread_t carsThreads[numCars];
	for (int i = 0; i < numCars; i++) {
		pthread_create(&carsThreads[i], &threadAttributes, (void*(*)(void*)) carThreadFunctionSeveralCars, (void*) NULL);
	}

	pthread_t passengersThreads[numPassengers];
	for (int i = 0; i < numPassengers; i++) {
		pthread_create(&passengersThreads[i], &threadAttributes, (void*(*)(void*)) passengerThreadFunctionSeveralCars, (void*) i);
	}
}

void* carThreadFunctionOneCar() {
	while (true) {
		rollerCoasterOneCar->load();
		usleep(2000000);
		rollerCoasterOneCar->unload();
	}
	return NULL;
}
void* passengerThreadFunctionOneCar(int passengerId) {
	while (true) {
		rollerCoasterOneCar->takeRide(passengerId);
	}
	return NULL;
}
void* carThreadFunctionSeveralCars() {
	while (true) {
		rollerCoasterSeveralCars->load();
		usleep(2000000);
		rollerCoasterSeveralCars->unload();
	}
	return NULL;
}
void* passengerThreadFunctionSeveralCars(int passengerId) {
	while (true) {
		rollerCoasterSeveralCars->takeRide(passengerId);
	}
	return NULL;
}
