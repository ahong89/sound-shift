#include <iostream>
#include <fstream>

#include "parser.h"
#include "processor.h"
#include "player.h"

using namespace std;

int main() {
	// initialize parser
	Parser parser;
	parser.parse();
	float** data = parser.get_audio_data();
	unsigned long num_samples = parser.get_num_samples();
	uint16_t num_channels = parser.get_num_channels();
	uint32_t sample_rate = parser.get_sample_rate();

	// initialize stretcher
	// Processor processor; // should call constructor
	
	// initialize player
	Player player(sample_rate, num_channels);
	player.set_audio(data, num_samples, num_channels);
	player.start_stream();


	// process loop
	while(true) {
		// query getSamplesRequired() -> how many audio sample frames are required

		// provide num of samples to process()

		// read output using availble() or retrieve()
		
		break;
	}
	cout << "End of program" << endl;

	return 0;
}
