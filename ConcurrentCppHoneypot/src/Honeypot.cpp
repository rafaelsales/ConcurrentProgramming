/*
 * Honeypot.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <limits>
#include "Honeypot.h"
#include "GenericException.h"

using namespace std;

Honeypot::Honeypot(int numHoneyBee, int maxHoneypotPortions, int maxTurns) throw (GenericException) :
	NUM_HONEYBEE(numHoneyBee), MAX_HONEYPOT_PORTIONS(maxHoneypotPortions), MAX_TURNS(maxTurns) {
	if (numHoneyBee < 1 || maxHoneypotPortions < 1 || maxTurns < -1 || maxTurns == 0) {
		throw GenericException("The values of numHoneyBee and maxPortionsInHoneypot must be positive and maxTurns must be positive or -1");
	}
}

Honeypot::~Honeypot() {

}

void* honeybeeThreadFunction(void* arg) {
	Honeypot* honeypot = (Honeypot*) arg;
	honeypot->honeybeeThread();
	return NULL;
}

void* bearThreadFunction(void* arg) {
	Honeypot* honeypot = (Honeypot*) arg;
	honeypot->bearThread();
	return NULL;
}

void Honeypot::start() {
	//Initially, the honeypot is empty:
	honeypotFilledCount = 0;
	honeypotPortions = 0;
	sem_init(&semaphoreHoneypotIsNotFull, SHARED_SEMAPHORE, 1);
	sem_init(&semaphoreHoneypotIsFull, SHARED_SEMAPHORE, 0);

	//Initialize the thread attributes:
	pthread_attr_t threadAttributes;
	pthread_attr_init(&threadAttributes);
	pthread_attr_setscope(&threadAttributes, PTHREAD_SCOPE_SYSTEM);

	pthread_t honeybeeThreads[NUM_HONEYBEE];
	for (int i = 0; i < NUM_HONEYBEE; i++) {
		pthread_create(&honeybeeThreads[i], &threadAttributes, honeybeeThreadFunction, this);
	}
	pthread_t bearThread;
	pthread_create(&bearThread, &threadAttributes, bearThreadFunction, this);

	pthread_join(bearThread, NULL);
	for (int i = 0; i < NUM_HONEYBEE; i++) {
		pthread_join(honeybeeThreads[i], NULL);
	}
	pthread_join(bearThread, NULL);
}

void Honeypot::honeybeeThread() {
	while (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS) {
		runHoneybeeEntryProtocol();
		//Verifies if the maximum number of turns has not been reached after acquire permission:
		if (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS) {
			doHoneybeeStuff();
			runHoneybeeExitProtocol();
		}
	}
}

void Honeypot::bearThread() {
	do {
		runBearEntryProtocol();
		doBearStuff();
		runBearExitProtocol();
	} while (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS || honeypotPortions != 0);
}

void Honeypot::doHoneybeeStuff() {
	cout << "'Honeybee Thread Id " << pthread_self() << "' says: Filling the honeypot. The honeypot have " << honeypotPortions << " portion(s) of honey." << endl;
}

void Honeypot::doBearStuff() {
	cout << "'Bear Thread Id " << pthread_self() << "' says: The honeypot is empty." << endl;
}

void Honeypot::runHoneybeeEntryProtocol() {
	sem_wait(&semaphoreHoneypotIsNotFull);
	//Checks if the maximum number of turns has been reached:
	if (honeypotFilledCount == MAX_TURNS) {
		//Signals any honeybee waiting for the empty honeypot:
		sem_post(&semaphoreHoneypotIsNotFull);
		return;
	}
	honeypotPortions++;
}

void Honeypot::runHoneybeeExitProtocol() {
	//If the honeypot is full, awake the bear. Otherwise, signal another honeybee.
	if (honeypotPortions == MAX_HONEYPOT_PORTIONS) {
		if (honeypotFilledCount == numeric_limits<int>::max()) {
			honeypotFilledCount = 0;
		} else {
			honeypotFilledCount++;
		}
		sem_post(&semaphoreHoneypotIsFull);
	} else {
		sem_post(&semaphoreHoneypotIsNotFull);
	}
}

void Honeypot::runBearEntryProtocol() {
	sem_wait(&semaphoreHoneypotIsFull);
}

void Honeypot::runBearExitProtocol() {
	//Empties the portion count in the honeypot:
	honeypotPortions = 0;
	sem_post(&semaphoreHoneypotIsNotFull);
}
