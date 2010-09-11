/*
 * Main.cpp
 *
 *  Created on: Apr 08, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <pthread.h>
#include "ReadersWriters.h"

using namespace std;


void* readerThreadFunction(int readerId);
void* writerThreadFunction(int writerId);
ReadersWriters readersWriters;
int numRounds;

int main(int argc, char* argv[]) {
	//Initialize the thread attributes:
	pthread_attr_t threadAttributes;
	pthread_attr_init(&threadAttributes);
	pthread_attr_setscope(&threadAttributes, PTHREAD_SCOPE_SYSTEM);

	int numReaders;
	int numWriters;
	do {
		cout << "Number of readers: ";
		cin >> numReaders;
	} while (numReaders <= 0);
	do {
		cout << "Number of writers: ";
		cin >> numWriters;
	} while (numWriters <= 0);
	do {
		cout << "Number of rounds: ";
		cin >> numRounds;
	} while (numRounds <= 0);
	cout << endl;

	for (int i = 0; i < numReaders; i++) {
		pthread_t pthreadReader;
		pthread_create(&pthreadReader, &threadAttributes, (void*(*)(void*)) readerThreadFunction, (void*) i);
	}
	for (int i = 0; i < numWriters; i++) {
		pthread_t pthreadWriter;
		pthread_create(&pthreadWriter, &threadAttributes, (void*(*)(void*)) writerThreadFunction, (void*) i);
	}

	pthread_exit(NULL);
	return 0;
}

void* readerThreadFunction(int readerId) {
	ReadersWriters::Reader reader(readersWriters, readerId);
	reader.read(numRounds);
	return NULL;
}

void* writerThreadFunction(int writerId) {
	ReadersWriters::Writer writer(readersWriters, writerId);
	writer.write(numRounds);
	return NULL;
}
