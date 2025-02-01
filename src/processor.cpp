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
	set_state(Processing);
}

bool Processor::update() {
	// process data
	if(!processing_complete) {
		size_t num_samples_required = stretcher->getSamplesRequired();
		if(num_samples_required >= audio_data.num_samples - curr_sample_index) {
			processing_complete = true;
			num_samples_required = audio_data.num_samples - curr_sample_index;
		}	
		float** input = new float*[num_channels];
		for(size_t i = 0; i < num_channels; i++) {
			input[i] = audio_data.data[i] + curr_sample_index;
		}
		stretcher->process(input, num_samples_required, processing_complete); 	
		curr_sample_index += num_samples_required;
		delete[] input;
	}

	// get processed data
	int num_available = stretcher->available();
	if(num_available == -1) {
		// cout << "Processing complete, num of samples left " << processed_audio.size() << endl;
		if(get_state() == Stopped) {
			return false;
		}
		return true;
	}

	lock_guard<mutex> lock(audio_mutex);
	float** retrieved = new float*[num_channels];
	for(size_t i = 0; i < num_channels; i++) {
		retrieved[i] = new float[num_available];
	}
	unsigned long num_retrieved = stretcher->retrieve(retrieved, num_available);
	for(size_t i = 0; i < num_retrieved; i++) {
		float* sample = new float[num_channels];
		for(uint16_t j = 0; j < num_channels; j++) {
			sample[j] = retrieved[j][i];
		}
		processed_audio.push_back(sample);
	}
	for(uint16_t i = 0; i < num_channels; i++) {
		delete[] retrieved[i];
	}
	delete[] retrieved;
	return true;
}

float** Processor::retrieve_audio(unsigned long num_samples) {
	float** audio_output = new float*[num_channels];
	for(uint16_t i = 0; i < num_channels; i++) {
		audio_output[i] = new float[num_samples]();
	}
	if(processed_audio.size() < num_samples) {
		return audio_output;
	}

	if(get_state() == Processing) {
		set_state(Playing);
	}
	for(unsigned long i = 0; i < num_samples; i++) {
		float* sample = processed_audio.front();
		processed_audio.pop_front();
		for(uint16_t j = 0; j < num_channels; j++) {
			audio_output[j][i] = sample[j];
		}
		delete[] sample;
	}
	return audio_output;
}

State Processor::get_state() {
	lock_guard<mutex> lock(state_mutex);
	return curr_state;
}

void Processor::set_state(State next_state) {
	lock_guard<mutex> lock(state_mutex);
	curr_state = next_state;
	cout << "Current State: " << curr_state << endl;
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

