package rafael.concurrent.monitor.linkedlist;


/**
 * This class is an implementation of the concurrent Search/Write/Delete to a linked list problem.
 * Invariant: (nw <= 1 and nd <= 1 and nd == 1 => (nr == 0 and n == 0))
 *  - nr: number of active readers
 *  - nw: number of active writers
 *  - nd: number of active deleters
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
public class ConcurrentLinkedList {
	
	private boolean hasActiveWriter = false;
	private boolean hasActiveDeleter = false;
	private int numReaders = 0;

	/**
	 * Entry protocol for the Reader. After reading call endRead().
	 */
	public synchronized void startRead() {
		while (hasActiveDeleter) {
			try {
				wait();
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}
		}
		numReaders++;
	}
	
	/**
	 * Exit protocol for the Reader.
	 */
	public synchronized void endRead() {
		//Prevents calling exit protocol at illegal state:
		if (numReaders == 0) {
			throw new IllegalStateException("There are not any active readers.");
		}
		numReaders--;
		if (numReaders == 0) {
			notify();
		}
	}
	
	/**
	 * This method executes sequentially the reader entry protocol, readerAction.run() and the reader exit protocol.
	 * @param readerAction - the Runnable object whose run method implements a reader action
	 */
	public void read(Runnable readerAction) {
		startRead();
		try {
			readerAction.run();			
		} finally {
			endRead();			
		}
	}
	
	/**
	 * Entry protocol for the Writer. After reading call endWrite().
	 */
	public synchronized void startWrite() {
		while (hasActiveWriter || hasActiveDeleter) {
			try {
				wait();
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}
		}
		hasActiveWriter = true;
	}
	
	/**
	 * Exit protocol for the Writer.
	 */
	public synchronized void endWrite() {
		//Prevents calling exit protocol at illegal state:
		if (!hasActiveWriter) {
			throw new IllegalStateException("There is no active writer.");
		}
		hasActiveWriter = false;
		notify();
	}

	/**
	 * This method executes sequentially the writer entry protocol, writerAction.run() and the writer exit protocol 
	 * @param writerAction - the Runnable object whose run method implements a writer action
	 */
	public void write(Runnable writerAction) {
		startWrite();
		try {
			writerAction.run();			
		} finally {
			endWrite();			
		}
	}
	
	/**
	 * Entry protocol for the Deleter. After reading call endDelete().
	 */
	public synchronized void startDelete() {
		while (hasActiveDeleter || hasActiveWriter || numReaders > 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}
		}
		hasActiveDeleter = true;
	}

	/**
	 * Exit protocol for the Deleter.
	 */
	public synchronized void endDelete() {
		//Prevents calling exit protocol at illegal state:
		if (!hasActiveDeleter) {
			throw new IllegalStateException("There is no active deleter.");
		}
		hasActiveDeleter = false;
		notifyAll();
	}
	
	/**
	 * This method executes sequentially the deleter entry protocol, deleterAction.run() and the deleter exit protocol.
	 * @param deleterAction - the Runnable object whose run method implements a deleter action
	 */
	public void delete(Runnable deleterAction) {
		startDelete();
		try {
			deleterAction.run();
		} finally {
			endDelete();
		}
	}
}