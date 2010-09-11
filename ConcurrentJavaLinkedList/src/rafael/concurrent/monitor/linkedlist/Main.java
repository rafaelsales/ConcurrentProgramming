package rafael.concurrent.monitor.linkedlist;

import java.util.LinkedList;

public class Main {
	public static void main(String[] args) {
		final LinkedList<Integer> linkedList = new LinkedList<Integer>();
		final ConcurrentLinkedList concurrentLinkedList = new ConcurrentLinkedList();

		// Creates two searchers, writers and deleters threads:
		for (int i = 1; i <= 2; i++) {
			final int iF = i;
			new Thread(new Runnable() {
				@Override
				public void run() {
					while (true) {
						try {
							concurrentLinkedList.startRead();

							System.out.printf("'Reader No. %d' says: Reading... %s\n", iF, linkedList);
							Thread.sleep(1000);

							concurrentLinkedList.endRead();
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}).start();
			new Thread(new Runnable() {
				@Override
				public void run() {
					while (true) {
						try {
							concurrentLinkedList.startWrite();

							Thread.sleep(1000);
							int element = (int) (100 * Math.random());
							System.out.printf("'Writer No. %d' says: Writing element %d at position %d\n", iF, element, linkedList.size());
							linkedList.add(element);

							concurrentLinkedList.endWrite();
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}).start();
			
			new Thread(new Runnable() {
				@Override
				public void run() {
					while (true) {
						try {
							concurrentLinkedList.startDelete();

							if (!linkedList.isEmpty()) {
								Thread.sleep(2000);
								Integer element = linkedList.remove(0);
								System.out.printf("'Deleter No. %d' says: Deleting element %d at position 0\n", iF, element);
							}
							
							concurrentLinkedList.endDelete();
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}).start();
		}
	}
}
