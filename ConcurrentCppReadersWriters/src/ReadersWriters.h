/*
 * ReadersWriters.h
 *
 *  Created on: May 9, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef READERSWRITERS_H_
#define READERSWRITERS_H_

#include <pthread.h>

/**
 * This class is an implementation of Readers and Writers concurrency problem
 *
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
class ReadersWriters {
public:
	ReadersWriters();
	virtual ~ReadersWriters();

	class Reader {
	public:
		Reader(ReadersWriters& readersWriters, int readerId);
		void read();
		void read(int numRounds);
	private:
		ReadersWriters& readersWriters;
		int readerId;
		void startRead();
		void endRead();
	};

	class Writer {
	public:
		Writer(ReadersWriters& readersWriters, int writerId);
		void write();
		void write(int numRounds);
	private:
		ReadersWriters& readersWriters;
		int writerId;
	};
private:
	int data;
	int numReaders;
	pthread_mutex_t lock;
	pthread_cond_t canWrite;
};

#endif /* READERSWRITERS_H_ */
