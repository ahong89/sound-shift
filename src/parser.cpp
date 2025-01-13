#include "parser.h"

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

Parser::Parser() {
	// read .wav input
	WAVHeader header;
	
	ifstream fin("./media/" + filename, ios::binary);
	fin.read((char*)(&header), sizeof(header));
	
	ChunkInfo chunk_info;
	char fmt_id[4] = {'f', 'm', 't', ' '};
	char data_id[4] = {'d', 'a', 't', 'a'};
	while(fin.read((char*)(&chunk_info), sizeof(chunk_info))){
		if(memcmp(chunk_info.subchunk_id, fmt_id, 4) == 0) {
			fin.read((char*)(&fmt), sizeof(fmt));
			cout << "We found the fmt" << endl;
			print_fmt();
		} else if(memcmp(chunk_info.subchunk_id, data_id, 4) == 0) {
			cout << "We found the data" << endl;
		} else {
			cout << "We found a different chunk of size: " << chunk_info.subchunk_size << endl;
			cout << "Subchunk id: " << chunk_info.subchunk_id << endl;
			break;
		}
	}
}

void Parser::print_fmt() {
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
