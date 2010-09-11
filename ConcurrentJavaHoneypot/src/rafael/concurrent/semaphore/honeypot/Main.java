package rafael.concurrent.semaphore.honeypot;

import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		Scanner scannerInt = new Scanner(System.in);
		int numHoneybee;
		int maxHoneypotPortions;
		int maxTurns;
		int fairSemaphoreInt; //1 = true; 0 = false
		boolean fairSemaphore;
		do {
			System.out.print("Number of honeybees: ");
			numHoneybee = scannerInt.nextInt();
		} while (numHoneybee <= 0);
		do {
			System.out.print("Maximum number of portions of the honeypot: ");
			maxHoneypotPortions = scannerInt.nextInt();
		} while (maxHoneypotPortions <= 0);
		do {
			System.out.print("Maximum number of times the honeypot is filled (-1 for indefinitely times): ");
			maxTurns = scannerInt.nextInt();
		} while (maxTurns < -1 || maxTurns == 0);
		do {
			System.out.print("Use fair semaphore (1 = true; 0 = false): ");
			fairSemaphoreInt = scannerInt.nextInt();
		} while (fairSemaphoreInt != 0 && fairSemaphoreInt != 1);
		fairSemaphore = (fairSemaphoreInt == 1) ? true : false;
		System.out.println();
		
		try {
			Honeypot theBearAndTheHoneyBees = new Honeypot(numHoneybee, maxHoneypotPortions, maxTurns, fairSemaphore);
			theBearAndTheHoneyBees.start();
		} catch (Exception e) {
			System.err.println("An unexpected error has occurred: ");
			e.printStackTrace();
		}
	}
}
