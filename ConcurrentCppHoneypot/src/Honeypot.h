/*
 * Honeypot.h
 *
 *  Created on: Apr 30, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef HONEYPOT_H_
#define HONEYPOT_H_

#include <semaphore.h>
#include "GenericException.h"

/**
 * This class is an implementation of a concurrent Honeypot of The Bear And The Honeybees problem.
 * To implement the bear and honeybee operations that uses a shared resource,
 * override the methods doBearStuff() and doHoneybeeStuff().
 *
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
class Honeypot {
public:
	/**
	 * Creates a Honeypot implementation using semaphore.
	 * @param numHoneyBee - the number of honeybees
	 * @param maxHoneypotPortions - the maximum number of portions of the honeypot
	 * @param maxTurns - the maximum number of times the honeypot is filled. -1 for indefinitely times
	 */
	Honeypot(int honeyBeeQty, int maxHoneypotPortions, int maxTurns) throw (GenericException);
	/**
	 * Creates and starts the honeybees and bear threads. This method waits all threads to die.
	 */
	void start();
	virtual ~Honeypot();

	void bearThread();
	void honeybeeThread();
protected:
	static const int SHARED_SEMAPHORE = 1;
	const int NUM_HONEYBEE;
	const int MAX_HONEYPOT_PORTIONS;
	const int MAX_TURNS;

	/**
	 * Method that uses a shared resource like the honeybee behavior.
	 */
	virtual void doHoneybeeStuff();
	/**
	 * Method that uses a shared resource like the bear behavior.
	 */
	virtual void doBearStuff();

private:
	int honeypotPortions; //Number of portions the honeypot contains
	int honeypotFilledCount; //Number of times the honeypot has been filled
	sem_t semaphoreHoneypotIsNotFull;
	sem_t semaphoreHoneypotIsFull;

	void runHoneybeeEntryProtocol();
	void runHoneybeeExitProtocol();
	void runBearEntryProtocol();
	void runBearExitProtocol();
};
#endif /* HONEYPOT_H_ */
