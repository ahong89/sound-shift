#include "portaudio.h"
#include <cstdint>

struct Sample {
	float left_data;
	float right_data;
};

class Player {
public:
	Player(uint32_t sample_rate);
	~Player();

private:
	Sample sample;
};
