#include "processor.h"

#include <iostream>

using namespace std;

Processor::Processor() {
	bool performance_engine = true;
	
	options = options | RubberBandStretcher::Option::OptionProcessRealTime; // call process() in this mode
	if(performance_engine) {
		options = options | RubberBandStretcher::Option::OptionEngineFiner;
	} else {
		options = options | RubberBandStretcher::Option::OptionEngineFaster;
	}
	options = options | RubberBandStretcher::Option::OptionPitchHighConsistency; options = options | RubberBandStretcher::Option::OptionChannelsApart; // default
	cout << "Options currently selected: " << options << endl;

}

void Processor::init() {
	stretcher = new RubberBandStretcher(sample_rate, 
									 num_channels, 
									 options, 
									 time_ratio, 
									 pitch_scale);	
}

bool Processor::update() {
	// query getSamplesRequired() -> how many audio sample frames are required
	lock_guard<mutex> lock(mtx);
	// cout << "Update: Locked" << endl;

	size_t num_samples_required = stretcher->getSamplesRequired();
	// cout << "Update: Asking for " << num_samples_required << " samples" << endl;
	bool is_final = false;
	if(num_samples_required >= audio_data.num_samples - curr_sample_index) {
		is_final = true;
		num_samples_required = audio_data.num_samples - curr_sample_index;
	}
	// cout << "Update: is_final " << is_final << endl;
	
	// provide num of samples to process()
	float** input = new float*[num_channels];
	for(size_t i = 0; i < num_channels; i++) {
		input[i] = audio_data.data[i] + curr_sample_index;
	}
	stretcher->process(input, num_samples_required, is_final); 

	delete[] input;
	// cout << "Sent " << num_samples_required << " to process" << endl;
	curr_sample_index += num_samples_required;
	return true;
}

RubberBandStretcher* Processor::get_stretcher() {
	return stretcher;
}

void Processor::set_sample_rate(uint32_t sample_rate) {
	this->sample_rate = sample_rate;	
}

void Processor::set_num_channels(uint16_t num_channels) {
	this->num_channels = num_channels;
}

uint16_t Processor::get_num_channels() {
	return num_channels;
}

void Processor::set_audio_data(float** data, unsigned long num_samples, uint16_t num_channels) {
	AudioData audio_data;
	audio_data.data = data;
	audio_data.num_samples = num_samples;
	audio_data.num_channels = num_channels;
	this->audio_data = audio_data;
	this->num_channels = num_channels;
}

