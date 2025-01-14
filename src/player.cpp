#include "player.h"
#include <iostream>

using namespace std;

int pa_callback(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data) {
	Sample* data = (Sample*)user_data; // tell pa what format our audio will be?
	float* out = (float*)output; // convert output buffer into correct format of sample value
	(void) input; // prevent warning

	for(unsigned long i = 0; i < frames_per_buffer; i++) {
		out[i] = data->left_data;
		out[i] = data->right_data;
		data->left_data += 0.01f;
		if(data->left_data >= 1.0f) data->left_data -= 2.0f;
		data->right_data += 0.03f; // higher freq to distinguish
		if(data->right_data >= 1.0f) data->right_data -= 2.0f;
	}
	return 0;
}

Player::Player(uint32_t sample_rate) {
	PaError err = Pa_Initialize();
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Pulse Audio successfully initialized" << endl;
	}

	PaStream* stream;
	err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, sample_rate, paFramesPerBufferUnspecified, pa_callback, &sample);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Pulse Audio successfully opened stream" << endl;
	}

	err = Pa_StartStream(stream);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Pulse Audio successfully started stream" << endl;
	}

	Pa_Sleep(5000);

	err = Pa_StopStream(stream);
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	} else {
		cout << "Pulse Audio successfully ended stream" << endl;
	}
}

Player::~Player() {
	PaError err = Pa_Terminate();
	if(err != paNoError) {
		cout << "Error: " << Pa_GetErrorText(err) << endl;
	}
}
