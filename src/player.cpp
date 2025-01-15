#include "player.h"

#include <iostream>

using namespace std;

int Player::pa_callback(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data) {
	AudioData* audio_data = (AudioData*)user_data;
	float* out = (float*)output; // convert output buffer into correct format of sample value
	(void) input; // prevent warning

	for(unsigned long i = 0; i < frames_per_buffer; i++) {
		if(audio_data->sample_index + i < audio_data->num_samples) {
			out[i] = audio_data->data[0][audio_data->sample_index + i];
			out[i] = audio_data->data[1][audio_data->sample_index + i];
		} else {
			out[i] = 0;
			out[i] = 0;
		}
	}
	audio_data->sample_index += frames_per_buffer;
	return 0;
}

Player::Player(uint32_t sample_rate, uint16_t num_channels) {
	PaError err = Pa_Initialize();
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully initialized" << endl;
	}

	err = Pa_OpenDefaultStream(&stream, 0, num_channels, paFloat32, sample_rate, paFramesPerBufferUnspecified, pa_callback, &audio_data);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully opened stream" << endl;
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
	PaError err = Pa_StartStream(stream);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Port Audio successfully started stream" << endl;
	}
	Pa_Sleep(25000);
}

void Player::set_audio(float** data, unsigned long num_samples, uint16_t num_channels) {
	AudioData audio_data;
	audio_data.data = data;
	audio_data.num_samples = num_samples;
	audio_data.num_channels = num_channels;
	audio_data.sample_index = 0;
	this->audio_data = audio_data;
	cout << "This is sample_index rn: " << this->audio_data.sample_index << endl;
}
