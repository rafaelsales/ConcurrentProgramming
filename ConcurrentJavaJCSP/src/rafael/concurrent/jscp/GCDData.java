package rafael.concurrent.jscp;

/**
 * This class encapsulates the Greatest Common Divisor data
 */
public class GCDData {
	public GCDData(int n1, int n2) {
		this.n1 = n1;
		this.n2 = n2;
	}

	private int n1;
	private int n2;
	private int result; // Greatest common divisor of n1 and n2

	// Computes the GCD(n1, n2) and stores in result
	public void compute() {
		int x = this.n1;
		int y = this.n2;
		while (x > y || x < y) {
			if (x > y) {
				x = x - y;
			} else {
				y = y - x;
			}
		}
		this.result = x;
	}
	public int getN1() {
		return n1;
	}
	public int getN2() {
		return n2;
	}
	public int getResult() {
		return result;
	}
}
