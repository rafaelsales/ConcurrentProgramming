/*
 * ConcurrentLinkedList.cpp
 *
 *  Created on: May 8, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "ConcurrentLinkedList.h"
#include "GenericException.h"

using namespace std;

ConcurrentLinkedList::ConcurrentLinkedList() {
	hasActiveWriter = false;
	hasActiveDeleter = false;
	numReaders = 0;

	pthread_mutex_init(&locker, NULL);
	pthread_cond_init(&canExecute, NULL);
}

ConcurrentLinkedList::~ConcurrentLinkedList() {
}

void ConcurrentLinkedList::startRead() {
	pthread_mutex_lock(&locker);
	while (hasActiveDeleter) {
		pthread_cond_wait(&canExecute, &locker);
	}
	numReaders++;
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::endRead() {
	pthread_mutex_lock(&locker);
	//Prevents calling exit protocol at illegal state:
	if (numReaders == 0) {
		pthread_mutex_unlock(&locker);
		throw GenericException("There are not any active readers.");
	}
	numReaders--;
	if (numReaders == 0) {
		pthread_cond_signal(&canExecute);
	}
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::read(void(*readerAction)(void)) {
	startRead();
	exception* exceptionThrown = NULL;
	try {
		readerAction();
	} catch (exception e) {
		exceptionThrown = &e;
	}
	endRead();
	if (exceptionThrown != NULL) {
		throw *exceptionThrown;
	}
}

void ConcurrentLinkedList::startWrite() {
	pthread_mutex_lock(&locker);
	while (hasActiveWriter || hasActiveDeleter) {
		pthread_cond_wait(&canExecute, &locker);
	}
	hasActiveWriter = true;
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::endWrite() {
	pthread_mutex_lock(&locker);
	//Prevents calling exit protocol at illegal state:
	if (!hasActiveWriter) {
		pthread_mutex_unlock(&locker);
		throw GenericException("There is no active writer.");
	}
	hasActiveWriter = false;
	pthread_cond_signal(&canExecute);
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::write(void(*writerAction)(void)) {
	startWrite();
	exception* exceptionThrown = NULL;
	try {
		writerAction();
	} catch (exception e) {
		exceptionThrown = &e;
	}
	endWrite();
	if (exceptionThrown != NULL) {
		throw *exceptionThrown;
	}
}

void ConcurrentLinkedList::startRemove() {
	pthread_mutex_lock(&locker);
	while (hasActiveDeleter || hasActiveWriter || numReaders > 0) {
		pthread_cond_wait(&canExecute, &locker);
	}
	hasActiveDeleter = true;
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::endRemove() {
	pthread_mutex_lock(&locker);
	//Prevents calling exit protocol at illegal state:
	if (!hasActiveDeleter) {
		pthread_mutex_unlock(&locker);
		throw GenericException("There is no active deleter.");
	}
	hasActiveDeleter = false;
	pthread_cond_broadcast(&canExecute);
	pthread_mutex_unlock(&locker);
}

void ConcurrentLinkedList::remove(void(*deleterAction)(void)) {
	startRemove();
	exception* exceptionThrown = NULL;
	try {
		deleterAction();
	} catch (exception e) {
		exceptionThrown = &e;
	}
	endRemove();
	if (exceptionThrown != NULL) {
		throw *exceptionThrown;
	}
}
