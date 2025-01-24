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

	RubberBandStretcher* get_stretcher();
	mutex mtx;
	void set_num_channels(uint16_t num_channels);
	uint16_t get_num_channels();
	void set_sample_rate(uint32_t sample_rate);
	void set_audio_data(float** data, unsigned long num_samples, uint16_t num_channels);

private:
	RubberBandStretcher* stretcher;

	AudioData audio_data;
	int curr_sample_index = 0;

	size_t num_channels;
	size_t sample_rate;
	int options = 0;
	double time_ratio = 1.0;
	double pitch_scale = 1.0;
};

#endif
