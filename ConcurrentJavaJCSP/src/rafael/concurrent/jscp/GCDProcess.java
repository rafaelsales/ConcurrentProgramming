package rafael.concurrent.jscp;

import org.jcsp.lang.Alternative;
import org.jcsp.lang.AltingChannelInput;
import org.jcsp.lang.CSProcess;
import org.jcsp.lang.ChannelOutput;

public class GCDProcess implements CSProcess {

	private final AltingChannelInput[] inputs;
	private final ChannelOutput[] outputs;

	public GCDProcess(AltingChannelInput[] inputs, ChannelOutput[] outputs) {
		this.inputs = inputs;
		this.outputs = outputs;
	}

	@Override
	public void run() {
		final Alternative altInputs = new Alternative(inputs);
		while (true) {
			int clientId = altInputs.select();
			// Faz uma query para um cliente:
			GCDData gcd = (GCDData) inputs[clientId].read(); // Client[*]?args(id, gcd)
			System.out.printf("Servidor: Cliente[%d] solicita MDC(%d, %d) \n", 
					clientId, gcd.getN1(), gcd.getN2());
			//Calcula o MDC:
			gcd.compute();

			// Retorna o resultado para o cliente que forneceu os dados:
			outputs[clientId].write(gcd); // Client[id]!result(gcd);
		}
	}
}