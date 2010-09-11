package rafael.concurrent.monitor.readerswriters;

import java.util.Scanner;

import rafael.concurrent.monitor.readerswriters.ReadersWriters.Reader;
import rafael.concurrent.monitor.readerswriters.ReadersWriters.Writer;

public class Main {
	public static void main(String[] args) {
		Scanner scannerInt = new Scanner(System.in);
		int numRounds;
		final int numRoundsFinal;
		int numReaders;
		int numWriters;
		do {
			System.out.print("Number of readers: ");
			numReaders = scannerInt.nextInt();
		} while (numReaders <= 0);
		do {
			System.out.print("Number of writers: ");
			numWriters = scannerInt.nextInt();
		} while (numWriters <= 0);
		do {
			System.out.print("Number of rounds: ");
			numRounds = scannerInt.nextInt();
		} while (numRounds <= 0);
		numRoundsFinal = numRounds;
		System.out.println();

		final ReadersWriters readersWriters = new ReadersWriters();

		for (int i = 0; i < numReaders; i++) {
			final int readerId = i;
			new Thread(new Runnable() {
				@Override
				public void run() {
					Reader reader = readersWriters.new Reader(readerId);
					reader.read(numRoundsFinal);
				}
			}).start();
		}
		for (int i = 0; i < numWriters; i++) {
			final int writerId = i;
			new Thread(new Runnable() {
				@Override
				public void run() {
					Writer writer = readersWriters.new Writer(writerId);
					writer.write(numRoundsFinal);
				}
			}).start();
		}
	}
}
