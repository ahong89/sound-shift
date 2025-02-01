#include <cstdint>
#include "portaudio.h"
#include "processor.h"

#ifndef PLAYER
#define PLAYER

class Player {
public:
	Player();
	~Player();

	void start_stream();
	void set_audio(float** data, unsigned long num_samples, uint16_t num_channels);
	void set_processor(Processor* processor);
	void set_sample_rate(uint32_t sample_rate);
	void set_num_channels(uint16_t num_channels);
	static int pa_callback_audio(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data);
	static int pa_callback_processor(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data);
	struct AudioData {
		float** data;
		unsigned long num_samples;
		unsigned long sample_index;
		uint16_t num_channels;
	};

private:
	PaStream* stream;

	// either audio_data or processor is required
	AudioData audio_data;
	Processor* processor = nullptr;

	uint32_t sample_rate;
	uint16_t num_channels;
};

#endif
