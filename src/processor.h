#include <rubberband/RubberBandStretcher.h>
#include <deque>
#include <mutex>

using namespace RubberBand;
using namespace std;

#ifndef PROCESSOR
#define PROCESSOR

struct AudioData {
	float** data;
	unsigned long num_samples;
	uint16_t num_channels;
};

class Processor {
public:
	Processor();
	void init();
	bool update();

	float** retrieve_audio(unsigned long num_samples);
	mutex mtx;
	void set_num_channels(uint16_t num_channels);
	uint16_t get_num_channels();
	void set_sample_rate(uint32_t sample_rate);
	void set_audio_data(float** data, unsigned long num_samples, uint16_t num_channels);

private:
	RubberBandStretcher* stretcher;

	AudioData audio_data;
	unsigned long curr_sample_index = 0;
	bool is_final = false;
	deque<float*> processed_audio;

	size_t num_channels;
	size_t sample_rate;
	int options = 0;
	double time_ratio = 1.0; // inverted btw 2.0 is half the speed? (double the duration)
	double pitch_scale = 1.0;
};

#endif
