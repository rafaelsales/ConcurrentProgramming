/*
 * RollerCoasterOneCar.h
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef ROLLERCOASTERONECAR_H_
#define ROLLERCOASTERONECAR_H_

#include <pthread.h>

/**
 * This class is an implementation of The Roller Coaster concurrent problem for one car.
 * Invariant: (pc <= cc and cr => (pc == cc))
 *  - pc: number of passengers in the car
 *  - cc: number of passengers the car can carry
 *  - cr: true if the car is riding, false otherwise
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
class RollerCoasterOneCar {
public:
	/**
	 * Creates a Roller Coaster implementation using monitor.
	 * @param carCapacity number of passengers per car
	 */
	RollerCoasterOneCar(int carCapacity);
	virtual ~RollerCoasterOneCar();
	/**
	 * Requests to take ride in the car.
	 * @param passengerId identifier of the passenger
	 */
	void takeRide(const int passengerId);
	/**
	 * Requests the car to carry passengers.
	 */
	void load();
	/**
	 * Unload the passengers from the car.
	 */
	void unload();
protected:
	const int CAR_CAPACITY;
private:
	int passengersInCar; //Number of passengers in the car
	bool carIsWaitingPassengers; //True if the car is waiting for passengers
	pthread_mutex_t rollerCoasterLock;
	pthread_cond_t carLoad; //Signaled when the car can load passengers
	pthread_cond_t carUnload; //Signaled when the car is unloading its passengers
	pthread_cond_t carIsFull; //Signaled when the car is full
};

#endif /* ROLLERCOASTERONECAR_H_ */
