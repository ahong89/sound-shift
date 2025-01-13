#include "parser.h"

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

Parser::Parser() {
	// read .wav input	
	fin = ifstream("./media/" + filename, ios::binary);
	read_header();
	print_header();

	char fmt_id[4] = {'f', 'm', 't', ' '};
	char data_id[4] = {'d', 'a', 't', 'a'};
	ChunkInfo* chunk_info = new ChunkInfo;
	while(true){
		bool succeed = read_chunk_info(chunk_info);
		if(succeed) {
			if(memcmp(chunk_info->subchunk_id, fmt_id, 4) == 0) {
				read_fmt(*chunk_info);
				print_fmt();
			} else if(memcmp(chunk_info->subchunk_id, data_id, 4) == 0) {
				read_data(*chunk_info);
				cout << "We found the data" << endl;
			} else {
				cout << "We found a different chunk of size: " << chunk_info->subchunk_size << endl;
				cout << "Subchunk id: " << chunk_info->subchunk_id << endl;
				break;
			}
		} else {
			break;
		}
	}
	delete chunk_info;
}

void Parser::read_header() {
	fin.read((char*)(&header.chunk_id), sizeof(header.chunk_id));
	fin.read((char*)(&header.chunk_size), sizeof(header.chunk_size));
	fin.read((char*)(&header.format), sizeof(header.format));
}

void Parser::read_fmt(ChunkInfo chunk_info) {
	fin.read((char*)(&fmt.audio_format), sizeof(fmt.audio_format));
	fin.read((char*)(&fmt.num_channels), sizeof(fmt.num_channels));
	fin.read((char*)(&fmt.sample_rate), sizeof(fmt.sample_rate));
	fin.read((char*)(&fmt.byte_rate), sizeof(fmt.byte_rate));
	fin.read((char*)(&fmt.audio_format), sizeof(fmt.block_align));
	fin.read((char*)(&fmt.bits_per_sample), sizeof(fmt.bits_per_sample));
}

void Parser::read_data(ChunkInfo chunk_info) {

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
	cout << "chunk_id: " << string(header.chunk_id, 4) << endl;
	cout << "chunk_size: " << header.chunk_size << endl;
	cout << "format: " << string(header.format, 4) << endl;
}

void Parser::print_data() {

}

void Parser::parse() {
	cout << "I am parsing!!!" << endl;
}
