package inet.ofomnet.openflow.attackDetection.methods;

simple Entropy 
{
	parameters:
    	double threshold = default(-0.5);
    	@signal[entropy](type=double);
        @statistic[entropy](title="Entropy Value"; source=entropy; record=stats);
}
