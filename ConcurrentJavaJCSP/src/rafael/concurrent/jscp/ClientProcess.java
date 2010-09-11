package rafael.concurrent.jscp;

import org.jcsp.lang.CSProcess;
import org.jcsp.lang.ChannelInput;
import org.jcsp.lang.ChannelOutput;

public class ClientProcess implements CSProcess {
	
	private final ChannelInput input;
	private final ChannelOutput output;
	private final int clientId;

	public ClientProcess(int clientId, ChannelInput input, ChannelOutput output) {
		this.clientId = clientId;
		this.input = input;
		this.output = output;
	}

	@Override
	public void run() {
		while (true) {
			int n1, n2;
			n1 = 1 + (int) (100 * Math.random());
			n2 = 1 + (int) (100 * Math.random());

			GCDData gcd = new GCDData(n1, n2);
			output.write(gcd); // GCD!args(id, gcd)

			gcd = (GCDData) input.read(); // GCD?result(gcd);
			System.out.printf("Cliente[%d]: Servidor respondeu MDC(%d, %d) = %d \n", 
					clientId, gcd.getN1(), gcd.getN2(), gcd.getResult());
		}
	}
}