/*
 * RollerCoaster.h
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef ROLLERCOASTER_H_
#define ROLLERCOASTER_H_

#include <pthread.h>

/**
 * This class is an implementation of The Roller Coaster concurrent problem for many cars.
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
class RollerCoaster {
public:
	/**
	 * Creates a Roller Coaster implementation for several cars using monitor.
	 * @param carCapacity number of passengers per car
	 * @param numCars number of cars
	 */
	RollerCoaster(int carCapacity, int numCars);
	virtual ~RollerCoaster();
	/**
	 * Requests to take ride in a car.
	 * @param passengerId identifier of the passenger
	 */
	void takeRide(const int passengerId);
	/**
	 * Requests a car to carry passengers.
	 */
	void load();
	/**
	 * Unload the passengers from the arriving car.
	 */
	void unload();
protected:
	const int CAR_CAPACITY;
	const int NUM_CARS;
private:
	int* passengersInCar; //Array whose values are the number of passengers in the car[i]
	int nextCarRequestLoad; //Index of the next car that will request load
	int nextCarToUnload; //Index of the next car that will be unloaded
	int nextCarToLoad; //Index of the next car that will load
	bool nextCarToLoadIsWaitingPassengers; //True if the next car that will load is waiting for passengers
	pthread_mutex_t rollerCoasterLock;
	pthread_cond_t carLoad; //Signaled when the car can load passengers
	pthread_cond_t carUnload; //Signaled when the car is unloading its passengers
	pthread_cond_t carIsFull; //Signaled when the car is full
	pthread_cond_t nextCarCanLoad; //Signaled when the next car to load can load passengers
};

#endif /* ROLLERCOASTER_H_ */
