#include "parser.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <cmath>

using namespace std;

Parser::Parser() {

}

float** Parser::get_audio_data() {
	return audio_data;
}

unsigned long Parser::get_num_samples() {
	return num_samples;
}

uint16_t Parser::get_num_channels() {
	return fmt.num_channels;
}

uint32_t Parser::get_sample_rate() {
	return fmt.sample_rate;
}

void Parser::print_fmt() {
	cout << "---FMT DATA---" << endl;
	cout << "Audio format: " << fmt.audio_format << endl;
	cout << "Channels: " << fmt.num_channels << endl;
	cout << "Sample Rate: " << fmt.sample_rate << endl;
	cout << "Byte Rate: " << fmt.byte_rate << endl;
	cout << "Block Align: " << fmt.block_align << endl;
	cout << "Bits per sample: " << fmt.bits_per_sample << endl;
}

void Parser::print_header() {
	cout << "---HEADER DATA---" << endl;
	cout << "chunk_id: " << string(header.chunk_id, 4) << endl;
	cout << "chunk_size: " << header.chunk_size << endl;
	cout << "format: " << string(header.format, 4) << endl;
}

void Parser::print_data() {
	cout << "---DATA---" << endl;
	for(unsigned long sample = 0; sample < num_samples; sample++) {
		for(int channel = 0; channel < fmt.num_channels; channel++) {
			cout << audio_data[channel][sample] << " ";
		}
		cout << endl;
	}
}

void Parser::parse() {
	fin = ifstream("./media/" + filename, ios::binary);	

	if(!read_header()) {
		cout << "Header failed to read" << endl;
	} else {
		print_header();
	}

	char fmt_id[4] = {'f', 'm', 't', ' '};
	char data_id[4] = {'d', 'a', 't', 'a'};
	ChunkInfo* chunk_info = new ChunkInfo;
	bool found_fmt = false;
	bool found_data = false;
	while(true){
		bool succeed = read_chunk_info(chunk_info);
		if(succeed) {
			if(memcmp(chunk_info->subchunk_id, fmt_id, 4) == 0) {
				if(read_fmt(*chunk_info)) {
					found_fmt = true;
				} else {
					cout << "FMT failed to read" << endl;
				}
				print_fmt();
			} else if(memcmp(chunk_info->subchunk_id, data_id, 4) == 0) {
				if(read_data(*chunk_info)) {
					found_data = true;
				} else {
					cout << "Data failed to read" << endl;
				}
				// print_data();
			} else {
				cout << "We found a different chunk of size: " << chunk_info->subchunk_size << endl;
				cout << "Subchunk id: " << chunk_info->subchunk_id << endl;
				fin.seekg(chunk_info->subchunk_size, ios_base::cur);
			}
		} else {
			break;
		}
	}
	if(found_data && found_fmt) {
		cout << "Parse was successful !!" << endl;
	}
	delete chunk_info;
}

bool Parser::read_header() {
	fin.read((char*)(&header.chunk_id), sizeof(header.chunk_id));
	fin.read((char*)(&header.chunk_size), sizeof(header.chunk_size));
	fin.read((char*)(&header.format), sizeof(header.format));
	return !fin.fail() && !fin.eof();
}

bool Parser::read_fmt(ChunkInfo chunk_info) {
	fin.read((char*)(&fmt.audio_format), sizeof(fmt.audio_format));
	fin.read((char*)(&fmt.num_channels), sizeof(fmt.num_channels));
	fin.read((char*)(&fmt.sample_rate), sizeof(fmt.sample_rate));
	fin.read((char*)(&fmt.byte_rate), sizeof(fmt.byte_rate));
	fin.read((char*)(&fmt.audio_format), sizeof(fmt.block_align));
	fin.read((char*)(&fmt.bits_per_sample), sizeof(fmt.bits_per_sample));
	return !fin.fail();
}

bool Parser::read_data(ChunkInfo chunk_info) {
	uint16_t bytes_per_sample = fmt.bits_per_sample / 8;
	int64_t max_value = pow(2, (bytes_per_sample * 8)) / 2;
	cout << "Max_value: " << max_value <<  endl;
	num_samples = chunk_info.subchunk_size / fmt.num_channels / fmt.bits_per_sample * 8;
	audio_data = new float*[fmt.num_channels];
	for(int i = 0; i < fmt.num_channels; i++) {
		audio_data[i] = new float[num_samples];
	}
	
	for(unsigned long sample = 0; sample < num_samples; sample++) {
		/* for(int channel = 0; channel < fmt.num_channels; channel++) { // fixed 2 byte size
			int16_t sample_value = 0;
			fin.read((char*)(&sample_value), sizeof(sample_value));
			audio_data[channel][sample] = (float)sample_value / (float)max_value;
			fout << sample_value << ", ";
		} */
		for(int channel = 0; channel < fmt.num_channels; channel++) {
			char buffer[bytes_per_sample];
			int64_t sample_value = 0;
			fin.read(buffer, bytes_per_sample);
			for(uint16_t i = 0; i < bytes_per_sample; i++) {
				uint16_t byte_shift = i;
				uint64_t next = (buffer[i] & 0x00ff) << (byte_shift * 8);
				sample_value |= next;
			}
			if(sample_value > max_value) {
				sample_value = (max_value * 2 - sample_value) * -1;
			}
			audio_data[channel][sample] = (float)sample_value / (float)max_value;
		}
		if(fin.fail()) {
			break;
		}
	}
	return !fin.fail();
}

bool Parser::read_chunk_info(ChunkInfo* chunk_info) { // returns bool for whether it succeeds
	if(fin.read((char*)(chunk_info), sizeof(chunk_info))) {
		return true;
	} else {
		return false;
	}
}
