package rafael.concurrent.monitor.readerswriters;

/**
 * This class is an implementation of Readers and Writers concurrency problem
 * 
 * @author Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */
public class ReadersWriters {

	private int data = 0;
	private int numReaders = 0;

	public class Reader {

		private int readerId;

		public Reader(int readerId) {
			this.readerId = readerId;
		}

		private void startRead() {
			synchronized (ReadersWriters.this) {
				numReaders++;
			}
		}

		private void endRead() {
			synchronized (ReadersWriters.this) {
				numReaders--;
				if (numReaders == 0) {
					ReadersWriters.this.notify();
				}
			}
		}

		public void read() {
			startRead();
			System.out.println("Reader No." + readerId + ": read " + data);
			endRead();
		}

		public void read(int numRounds) {
			for (int i = 0; i < numRounds; i++) {
				read();
			}
		}
	}

	public class Writer {

		private int writerId;

		public Writer(int writerId) {
			this.writerId = writerId;
		}

		public void write() {
			synchronized (ReadersWriters.this) {
				while (numReaders > 0) {
					try {
						ReadersWriters.this.wait();
					} catch (InterruptedException ex) {
						return;
					}
				}
				data++;
				System.out.println("Writer No." + writerId + ": wrote " + data);
				ReadersWriters.this.notify();
			}
		}

		public void write(int numRounds) {
			for (int i = 0; i < numRounds; i++) {
				write();
			}
		}
	}
}
