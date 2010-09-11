package rafael.concurrent.jscp;

import org.jcsp.lang.AltingChannelInput;
import org.jcsp.lang.CSProcess;
import org.jcsp.lang.Channel;
import org.jcsp.lang.ChannelOutput;
import org.jcsp.lang.One2OneChannel;
import org.jcsp.lang.Parallel;

public class Main {
	public static void main(String[] args) {
		int numClientProcesses = 3;

		One2OneChannel[] one2OneChannel = Channel.one2oneArray(numClientProcesses);
		AltingChannelInput[] altingInputs = Channel.getInputArray(one2OneChannel);
		ChannelOutput[] outputs = Channel.getOutputArray(one2OneChannel);

		CSProcess[] csProcesses = new CSProcess[1 + numClientProcesses];
		csProcesses[csProcesses.length - 1] = new GCDProcess(altingInputs, outputs);
		for (int i = 0; i < numClientProcesses; i++) {
			csProcesses[i] = new ClientProcess(i, altingInputs[i], outputs[i]);
		}

		Parallel parallel = new Parallel(csProcesses);
		parallel.run();
	}
}