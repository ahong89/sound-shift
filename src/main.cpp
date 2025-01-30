#include <iostream>
#include <fstream>

#include "parser.h"
#include "processor.h"
#include "player.h"

using namespace std;

int main() {
	// initialize parser
	Parser parser("guitar.wav");
	parser.parse();
	float** data = parser.get_audio_data();
	unsigned long num_samples = parser.get_num_samples();
	uint16_t num_channels = parser.get_num_channels();
	uint32_t sample_rate = parser.get_sample_rate();

	// initialize stretcher
	Processor processor;
	processor.set_sample_rate(sample_rate);
	processor.set_audio_data(data, num_samples, num_channels);
	processor.init();
	
	// initialize player
	Player player;
	player.set_sample_rate(sample_rate);
	player.set_num_channels(num_channels);
	player.set_processor(&processor);
	player.start_stream();


	// process loop
	cout << endl << "-----REACHED PROCESS LOOP----" << endl;
	while(true) {
		if(!processor.update()) {
			break;
		}
	}
	cout << "End of program" << endl;

	return 0;
}
