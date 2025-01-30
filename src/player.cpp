#include "player.h"

#include <iostream>
#include <mutex>

using namespace std;

// used for if audio is set prior to starting audio and does not change
int Player::pa_callback_audio(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data) {
	AudioData* audio_data = (AudioData*)user_data;
	float* out = (float*)output; // convert output buffer into correct format of sample value
	(void) input; // prevent warning

	for(unsigned long i = 0; i < frames_per_buffer; i++) {
		if(audio_data->sample_index + i < audio_data->num_samples) {
			out[i*2] = audio_data->data[0][audio_data->sample_index + i];
			out[i*2+1] = audio_data->data[1][audio_data->sample_index + i];
		} else {
			out[i*2] = 0;
			out[i*2+1] = 0;
		}
	}
	audio_data->sample_index += frames_per_buffer;
	return 0;
}

// uses processor class
int Player::pa_callback_processor(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data) {
	Processor* callback_processor = (Processor*)user_data;
	float* out = (float*)output;
	(void) input;

	lock_guard<mutex> lock(callback_processor->mtx);
	// cout << "Callback: Locked" << endl;
	
	float** audio_data = callback_processor->retrieve_audio(frames_per_buffer);
	for(unsigned long i = 0; i < frames_per_buffer; i++) {
		out[i*2] = audio_data[0][i];
		out[i*2 + 1] = audio_data[1][i];
	}
	for(uint16_t i = 0; i < callback_processor->get_num_channels(); i++) {
		delete[] audio_data[i];
	}
	delete[] audio_data;
	return 0;
}


Player::Player() {
	PaError err = Pa_Initialize();
	if(err != paNoError) {
		cout << "Error in initializing: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully initialized" << endl;
	}	
}

Player::~Player() {
	if(!Pa_IsStreamStopped(stream)) {
		PaError err = Pa_StopStream(stream);
		if(err != paNoError) {
			cout << "Error: " << Pa_GetErrorText(err) << endl;
		} else {
			cout << "Port Audio successfully ended stream" << endl;
		}
	}

	PaError err = Pa_CloseStream(stream);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully closed stream" << endl;
	}

	err = Pa_Terminate();
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully terminated" << endl;
	}
}

void Player::start_stream() {
	PaError err;
	if(processor == nullptr) {
		err = Pa_OpenDefaultStream(&stream,
							 0,
							 num_channels,
							 paFloat32,
							 sample_rate,
							 paFramesPerBufferUnspecified,
							 pa_callback_audio,
							 &audio_data);
	} else {
		err = Pa_OpenDefaultStream(&stream,
							 0,
							 num_channels,
							 paFloat32,
							 sample_rate,
							 256,
							 pa_callback_processor,
							 processor);
	}
	if(err != paNoError) {
		cout << "Error in starting stream: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully opened stream" << endl;
	}

	err = Pa_StartStream(stream);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully started stream" << endl;
	}
	if(processor == nullptr) {
		Pa_Sleep(25000);
	}
}

void Player::set_audio(float** data, unsigned long num_samples, uint16_t num_channels) {
	AudioData audio_data;
	audio_data.data = data;
	audio_data.num_samples = num_samples;
	audio_data.num_channels = num_channels;
	audio_data.sample_index = 0;
	this->audio_data = audio_data;
}

void Player::set_processor(Processor* processor) {
	this->processor = processor;
}

void Player::set_sample_rate(uint32_t sample_rate) {
	this->sample_rate = sample_rate;
}

void Player::set_num_channels(uint16_t num_channels) {
	this->num_channels = num_channels;
}
