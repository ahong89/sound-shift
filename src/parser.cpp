#include "parser.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <cmath>

using namespace std;

Parser::Parser() {
	// read .wav input	
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
	uint16_t max_value = pow(2, (bytes_per_sample * 8)) / 2 - 1;
	cout << "subchunk_size: " << chunk_info.subchunk_size << endl;
	data.num_samples = chunk_info.subchunk_size / fmt.num_channels / fmt.bits_per_sample * 8;
	data.audio_data = new float*[fmt.num_channels];
	for(int i = 0; i < fmt.num_channels; i++) {
		data.audio_data[i] = new float[data.num_samples];
	}

	for(unsigned long sample = 0; sample < data.num_samples; sample++) {
		for(int channel = 0; channel < fmt.num_channels; channel++) {
			char buffer[bytes_per_sample];
			int64_t sample_value = 0;
			for(int i = 0; i < bytes_per_sample; i++) {
				fin.read(buffer, bytes_per_sample);
				sample_value = sample_value | ((buffer[i]) << (i * 8));
			}
			data.audio_data[channel][sample] = (float)sample_value / (float)max_value;	
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
	for(unsigned long sample = 0; sample < data.num_samples; sample++) {
		for(int channel = 0; channel < fmt.num_channels; channel++) {
			cout << data.audio_data[channel][sample] << " ";
		}
		cout << endl;
	}
}

void Parser::parse() {
	cout << "I am parsing!!!" << endl;
}
