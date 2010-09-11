package rafael.concurrent.semaphore.honeypot;

import java.util.concurrent.Semaphore;

/**
 * This class is an implementation of a concurrent Honeypot of The Bear And The Honeybees problem.
 * To implement the bear and honeybee operations that uses a shared resource, 
 * override the methods doBearStuff() and doHoneybeeStuff().
 *  
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
public class Honeypot {
	
	protected final int NUM_HONEYBEE;
	protected final int MAX_HONEYPOT_PORTIONS;
	protected final int MAX_TURNS;
	private final boolean FAIR_SEMAPHORE;
	
	private int honeypotPortions; //Number of portions the honeypot contains
	private int honeypotFilledCount; //Number of times the honeypot has been filled
	
	private Semaphore semaphoreHoneypotIsNotFull;
	private Semaphore semaphoreHoneypotIsFull;

	/**
	 * Creates a Honeypot implementation using semaphore
	 * @param numHoneybee - the number of honeybees
	 * @param maxHoneypotPortions - the maximum number of portions of the honeypot
	 * @param maxTurns - the maximum number of times the honeypot is filled. -1 for indefinitely times
	 * @param fairSemaphore - true if the semaphore will guarantee first-in first-out granting of permits under contention, else false
	 */
	public Honeypot(int numHoneybee, int maxHoneypotPortions, int maxTurns, boolean fairSemaphore) {
		if (numHoneybee < 1 || maxHoneypotPortions < 1 || maxTurns < -1 || maxTurns == 0) {
			throw new IllegalArgumentException("The values of numHoneyBee and maxPortionsInHoneypot must be positive and maxTurns must be positive or -1");
		}
		this.NUM_HONEYBEE = numHoneybee;
		this.MAX_HONEYPOT_PORTIONS = maxHoneypotPortions;
		this.MAX_TURNS = maxTurns;
		this.FAIR_SEMAPHORE = fairSemaphore;
	}
	
	/**
	 * Creates and starts the honeybees and bear threads. This method waits all threads to die.
	 * @throws InterruptedException
	 */
	public synchronized void start() throws InterruptedException {
		//Initially, the honeypot is empty:
		honeypotFilledCount = 0;
		honeypotPortions = 0;
		semaphoreHoneypotIsNotFull = new Semaphore(1, FAIR_SEMAPHORE);
		semaphoreHoneypotIsFull = new Semaphore(0);
		
		Thread[] honeybeeThreads = new Thread[NUM_HONEYBEE];
		for (int i = 0; i < NUM_HONEYBEE; i++) {
			honeybeeThreads[i] = new Thread(new HoneybeeThread(), "Honeybee Thread No. " + i);
			honeybeeThreads[i].start();
		}
		Thread bearThread = new Thread(new BearThread(), "Bear Thread");
		bearThread.start();
		
		for (int i = 0; i < NUM_HONEYBEE; i++) {
			honeybeeThreads[i].join();
		}
		bearThread.join();
	}
	
	/**
	 * Method that uses a shared resource like the honeybee behavior.
	 */
	protected void doHoneybeeStuff() {
		System.out.printf("'%s' says: Filling the honeypot. The honeypot have %d portion(s) of honey.\n", Thread.currentThread().getName(), honeypotPortions);
	}
	
	/**
	 * Method that uses a shared resource like the bear behavior.
	 */
	protected void doBearStuff() {
		System.out.printf("'%s' says: The honeypot is empty.\n", Thread.currentThread().getName());
	}
	
	private void runHoneybeeEntryProtocol() {
		try {
			semaphoreHoneypotIsNotFull.acquire();
			//Checks if the maximum number of turns has been reached:
			if (honeypotFilledCount == MAX_TURNS) {
				//Signals any honeybee waiting for the empty honeypot:
				semaphoreHoneypotIsNotFull.release();
				return;
			}
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}
		honeypotPortions++;
	}
	
	private void runHoneybeeExitProtocol() {
		//If the honeypot is full, awake the bear. Otherwise, signal another honeybee.
		if (honeypotPortions == MAX_HONEYPOT_PORTIONS) {
			if (honeypotFilledCount == Integer.MAX_VALUE) {
				honeypotFilledCount = 0;
			} else {
				honeypotFilledCount++;				
			}
			semaphoreHoneypotIsFull.release();
		} else {
			semaphoreHoneypotIsNotFull.release();
		}
	}
	
	private void runBearEntryProtocol() {
		try {
			semaphoreHoneypotIsFull.acquire();
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}
	}
	
	private void runBearExitProtocol() {
		//Empties the portion count in the honeypot:
		honeypotPortions = 0;
		semaphoreHoneypotIsNotFull.release();
	}
	
	private class HoneybeeThread implements Runnable {
		@Override
		public void run() {
			while (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS) {
				runHoneybeeEntryProtocol();
				//Verifies if the maximum number of turns has not been reached after acquire permission:
				if (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS) {
					try {
						doHoneybeeStuff();					
					} finally {
						runHoneybeeExitProtocol();					
					}
				}
			}
		}
	}
	
	private class BearThread implements Runnable {
		@Override
		public void run() {
			do {
				runBearEntryProtocol();
				try {
					doBearStuff();
				} finally {
					runBearExitProtocol();
				}
			} while (MAX_TURNS == -1 || honeypotFilledCount < MAX_TURNS || honeypotPortions != 0);
		}
	}
	
}
