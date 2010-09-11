/*
 * ConcurrentLinkedList.h
 *
 *  Created on: May 8, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef CONCURRENTLINKEDLIST_H_
#define CONCURRENTLINKEDLIST_H_

#include <pthread.h>
#include <string>

using std::string;

/**
 * This class is an implementation of the concurrent Search/Write/Delete to a linked list problem.
 * Invariant: (nw <= 1 and nd <= 1 and nd == 1 => (nr == 0 and n == 0))
 *  - nr: number of active readers
 *  - nw: number of active writers
 *  - nd: number of active deleters
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
class ConcurrentLinkedList {
public:
	ConcurrentLinkedList();
	virtual ~ConcurrentLinkedList();
	/**
	 * Entry protocol for the Reader. After reading call endRead().
	 */
	virtual void startRead();
	/**
	 * Exit protocol for the Reader.
	 */
	virtual void endRead();
	/**
	 * This method executes sequentially the reader entry protocol, readerAction() and the reader exit protocol.
	 * @param readerAction - function that implements a reader action
	 */
	virtual void read(void(*readerAction)(void));
	/**
	 * Entry protocol for the Writer. After reading call endWrite().
	 */
	virtual void startWrite();
	/**
	 * Exit protocol for the Writer.
	 */
	virtual void endWrite();
	/**
	 * This method executes sequentially the writer entry protocol, writerAction() and the writer exit protocol
	 * @param writerAction - function that implements a writer action
	 */
	virtual void write(void(*writerAction)(void));
	/**
	 * Entry protocol for the Deleter. After reading call endDelete().
	 */
	virtual void startRemove();
	/**
	 * Exit protocol for the Deleter.
	 */
	virtual void endRemove();
	/**
	 * This method executes sequentially the deleter entry protocol, deleterAction() and the deleter exit protocol.
	 * @param deleterAction - function that implements a deleter action
	 */
	virtual void remove(void(*deleterAction)(void));
private:
	bool hasActiveWriter;
	bool hasActiveDeleter;
	int numReaders;

	pthread_cond_t canExecute;
	pthread_mutex_t locker;
};

#endif /* CONCURRENTLINKEDLIST_H_ */
