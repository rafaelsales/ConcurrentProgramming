package rafael.concurrent.monitor.rollercoaster;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * This class is an implementation of The Roller Coaster concurrent problem for one car.
 * Invariant: (pc <= cc and cr => (pc == cc))
 *  - pc: number of passengers in the car
 *  - cc: number of passengers the car can carry
 *  - cr: true if the car is riding, false otherwise
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
public class RollerCoasterOneCar {
	
	protected final int CAR_CAPACITY;
	
	private int passengersInCar = 0; //Number of passengers in the car 
	private boolean carIsWaitingPassengers = false; //True if the car is waiting for passengers
	
	private final ReentrantLock rollerCoasterLock;
	private final Condition carLoad; //Signaled when the car can load passengers
	private final Condition carUnload; //Signaled when the car is unloading its passengers
	private final Condition carIsFull; //Signaled when the car is full
	
	/**
	 * Creates a Roller Coaster implementation using monitor.
	 * @param carCapacity number of passengers per car
	 */
	public RollerCoasterOneCar(int carCapacity) {
		this.CAR_CAPACITY = carCapacity;
		
		rollerCoasterLock = new ReentrantLock();
		carLoad = rollerCoasterLock.newCondition();
		carUnload = rollerCoasterLock.newCondition();
		carIsFull = rollerCoasterLock.newCondition();
	}
	
	/**
	 * Requests to take ride in the car.
	 * @param passengerId identifier of the passenger
	 */
	public void takeRide(final int passengerId) {
		rollerCoasterLock.lock();
		
		//Waits the car become available:
		while (!carIsWaitingPassengers || passengersInCar == CAR_CAPACITY) {
			System.out.println("Passenger " + passengerId + ": Waiting for the car");
			carLoad.awaitUninterruptibly();
		}
		
		passengersInCar++;
		System.out.println("Passenger " + passengerId + ": Will take ride in the car");
		if (passengersInCar == CAR_CAPACITY) {
			//Notifies the car that it is full:
			carIsFull.signal();
		}		
		//Waits until the car gets full:
		carIsFull.awaitUninterruptibly();
		//Waits the end of the ride:
		carUnload.awaitUninterruptibly();
		System.out.println("Passenger " + passengerId + ": Left the car");
		
		rollerCoasterLock.unlock();
	}
	
	/**
	 * Requests the car to carry passengers.
	 */
	public void load() {
		rollerCoasterLock.lock();
		
		//Waits for the car if it is riding:
		while (passengersInCar > 0) {
			carUnload.awaitUninterruptibly();
		}
		carIsWaitingPassengers = true;
		//Notifies the passengers the car is ready to load them:
		carLoad.signalAll();
		System.out.println("Car: Waiting for passengers");
		//Waits until the car gets full:
		carIsFull.awaitUninterruptibly();
		carIsWaitingPassengers = false;
		//Notifies the passengers that the car is full:
		carIsFull.signalAll();
		System.out.println("Car: The car is full. Going around...\n");
		
		rollerCoasterLock.unlock();
	}
	
	/**
	 * Unload the passengers from the car.
	 */
	public void unload() {
		rollerCoasterLock.lock();
		
		//Unload the passengers:
		carUnload.signalAll();
		passengersInCar = 0;
		System.out.println("Car: Unloaded its passengers");
		
		rollerCoasterLock.unlock();
	}
}