/*
 * Main.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <pthread.h>
#include "Honeypot.h"
#include "GenericException.h"

using namespace std;

int main(int argc, char* argv[]) {
	int numHoneybee;
	int maxHoneypotPortions;
	int maxTurns;

	do {
		cout << "Number of honeybees: ";
		cin >> numHoneybee;
	} while (numHoneybee <= 0);
	do {
		cout << "Maximum number of portions of the honeypot: ";
		cin >> maxHoneypotPortions;
	} while (maxHoneypotPortions <= 0);
	do {
		cout << "Maximum number of times the honeypot is filled (-1 for indefinitely times): ";
		cin >> maxTurns;
	} while (maxTurns < -1 || maxTurns == 0);
	cout << endl;

	try {
		Honeypot theBearAndTheHoneyBees(numHoneybee, maxHoneypotPortions, maxTurns);
		theBearAndTheHoneyBees.start();
	} catch (GenericException& ex) {
		cout << "An unexpected error has occurred: " << endl << ex.what() << endl;
	}
	pthread_exit(NULL);
	return 0;
}
