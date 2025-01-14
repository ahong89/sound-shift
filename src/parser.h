#include <cstdint>
#include <string>
#include <fstream>

using namespace std;

struct WAVHeader {
	char chunk_id[4];
	uint32_t chunk_size;
	char format[4];
};

struct ChunkInfo {
	char subchunk_id[4];
	uint32_t subchunk_size;
};

struct FmtChunk {
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
};

struct DataChunk {
	float** audio_data;
	unsigned long num_samples;
};

class Parser {
public:
	Parser();
	void parse();

	uint32_t get_sample_rate();

	void print_header();
	void print_fmt();
	void print_data();
private:
	string filename = "bach.wav";
	ifstream fin;
	WAVHeader header;
	FmtChunk fmt;
	DataChunk data;

	bool read_header();
	bool read_fmt(ChunkInfo chunk_info);
	bool read_data(ChunkInfo chunk_info);
	bool read_chunk_info(ChunkInfo* chunk_info);
};
