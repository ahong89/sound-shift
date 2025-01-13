#include "processor.h"

#include <iostream>

using namespace std;

Processor::Processor() {
	bool performance_engine = true;
	
	size_t sample_rate = 8000;
	size_t channels = 2;
	double time_ratio = 1.0;
	double pitch_scale = 1.0;
	int options = 0;

	options = options | RubberBandStretcher::Option::OptionProcessRealTime; // call process() in this mode
	if(performance_engine) {
		options = options | RubberBandStretcher::Option::OptionEngineFiner;
	} else {
		options = options | RubberBandStretcher::Option::OptionEngineFaster;
	}
	options = options | RubberBandStretcher::Option::OptionPitchHighConsistency;
	options = options | RubberBandStretcher::Option::OptionChannelsApart; // default
	cout << "Options currently selected: " << options << endl;

	processor = new RubberBandStretcher(sample_rate, 
									 channels, 
									 options, 
									 time_ratio, 
									 pitch_scale);	
}
