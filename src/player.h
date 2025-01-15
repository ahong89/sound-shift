#include "portaudio.h"
#include <cstdint>

struct AudioData {
	float** data;
	unsigned long num_samples;
	unsigned long sample_index;
	uint16_t num_channels;
};

class Player {
public:
	Player(uint32_t sample_rate, uint16_t num_channels);
	~Player();

	void start_stream();
	void set_audio(float** data, unsigned long num_samples, uint16_t num_channels);
	static int pa_callback(const void* input, void* output, unsigned long frames_per_buffer, 
				const PaStreamCallbackTimeInfo* time_info,
				PaStreamCallbackFlags status_flags,
				void* user_data);


private:
	PaStream* stream;
	AudioData audio_data;
};
