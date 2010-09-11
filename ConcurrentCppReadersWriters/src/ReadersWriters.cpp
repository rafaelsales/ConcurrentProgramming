/*
 * ReadersWriters.cpp
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <pthread.h>
#include "ReadersWriters.h"

using namespace std;

ReadersWriters::ReadersWriters() {
	data = 0;
	numReaders = 0;
}

ReadersWriters::~ReadersWriters() {
}

ReadersWriters::Reader::Reader(ReadersWriters& readersWriters, int readerId) :
	readersWriters(readersWriters), readerId(readerId) {
}

void ReadersWriters::Reader::read() {
	startRead();
	cout << "Reader No." << readerId << ": read " << readersWriters.data << endl;
	endRead();
}

void ReadersWriters::Reader::read(int numRounds) {
	for (int i = 0; i < numRounds; i++) {
		read();
	}
}

void ReadersWriters::Reader::startRead() {
	pthread_mutex_lock(&readersWriters.lock);
	readersWriters.numReaders++;
	pthread_mutex_unlock(&readersWriters.lock);
}

void ReadersWriters::Reader::endRead() {
	pthread_mutex_lock(&readersWriters.lock);
	readersWriters.numReaders--;
	if (readersWriters.numReaders == 0) {
		pthread_cond_signal(&readersWriters.canWrite);
	}
	pthread_mutex_unlock(&readersWriters.lock);
}

ReadersWriters::Writer::Writer(ReadersWriters& readersWriters, int writerId) :
	readersWriters(readersWriters), writerId(writerId) {
}

void ReadersWriters::Writer::write() {
	pthread_mutex_lock(&readersWriters.lock);
	while (readersWriters.numReaders > 0) {
		pthread_cond_wait(&readersWriters.canWrite, &readersWriters.lock);
	}
	readersWriters.data++;
	cout << "Writer No." << writerId << ": wrote " << readersWriters.data << endl;
	pthread_cond_signal(&readersWriters.canWrite);
	pthread_mutex_unlock(&readersWriters.lock);
}

void ReadersWriters::Writer::write(int numRounds) {
	for (int i = 0; i < numRounds; i++) {
		write();
	}
}
