/*
 * Main.cpp
 *
 *  Created on: Apr 08, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include <iostream>
#include <cstdlib>
#include <list>
#include <pthread.h>
#include <sstream>
#include "ConcurrentLinkedList.h"
#include "GenericException.h"

using namespace std;

#define listType int
list<listType> linkedList; //The shared linkedlist
ConcurrentLinkedList concurrentLinkedList;
pthread_mutex_t lockerWriteConsole; //Mutex for use with non-atomic std::cout

void* readerThreadFunction(int readerId);
void* writerThreadFunction(int writerId);
void* deleterThreadFunction(int deleterId);
//Converts a list to string:
string convertListToString(list<listType> linkedList);

int main(int argc, char* argv[]) {
	//Initialize the thread attributes:
	pthread_attr_t threadAttributes;
	pthread_attr_init(&threadAttributes);
	pthread_attr_setscope(&threadAttributes, PTHREAD_SCOPE_SYSTEM);
	pthread_mutex_init(&lockerWriteConsole, NULL);

	pthread_t threads[6];
	for (int i = 0; i < 2; i++) {
		pthread_create(&threads[i], &threadAttributes, (void*(*)(void*)) readerThreadFunction, (void*) (i + 1));
		pthread_create(&threads[i + 2], &threadAttributes, (void*(*)(void*)) writerThreadFunction, (void*) (i + 1));
		pthread_create(&threads[i + 4], &threadAttributes, (void*(*)(void*)) deleterThreadFunction, (void*) (i + 1));
	}
	pthread_exit(NULL);
	return 0;
}

void* readerThreadFunction(int readerId) {
	while (true) {
		concurrentLinkedList.startRead();

		pthread_mutex_lock(&lockerWriteConsole);
		cout << "'Reader No. " << readerId << "' says: Reading... " << convertListToString(linkedList) << endl;
		pthread_mutex_unlock(&lockerWriteConsole);
		usleep(1000000);

		concurrentLinkedList.endRead();
		usleep(1000000);
	}
	return NULL;
}

void* writerThreadFunction(int writerId) {
	while (true) {
		concurrentLinkedList.startWrite();

		usleep(1000000);
		int element = rand() % 100;
		pthread_mutex_lock(&lockerWriteConsole);
		cout << "'Writer No. " << writerId << "' says: Writing element " << element << " at position " << linkedList.size() << endl;
		pthread_mutex_unlock(&lockerWriteConsole);
		linkedList.push_back(element);

		concurrentLinkedList.endWrite();
		usleep(1000000);
	}
	return NULL;
}

void* deleterThreadFunction(int deleterId) {
	while (true) {
		concurrentLinkedList.startRemove();

		if (!linkedList.empty()) {
			usleep(1000000);
			int element = linkedList.front();
			linkedList.pop_front();
			pthread_mutex_lock(&lockerWriteConsole);
			cout << "'Deleter No. " << deleterId << "' says: Deleting element " << element << " at position 0" << endl;
			pthread_mutex_unlock(&lockerWriteConsole);
		}

		concurrentLinkedList.endRemove();
		usleep(1000000);
	}
	return NULL;
}

string convertListToString(list<listType> linkedList) {
	stringstream listAsString;
	listAsString << "[";
	for (list<listType>::iterator it = linkedList.begin(); it != linkedList.end(); ++it) {
		if (it != linkedList.begin()) {
			listAsString << " ";
		}
		listAsString << *it;
	}
	listAsString << "]";
	return listAsString.str();
}
