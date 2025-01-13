#include <cstdint>
#include <string>

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
	
};

class Parser {
public:
	Parser();
	void parse();

	void print_header();
	void print_fmt();
	void print_data();
private:
	string filename = "bach.wav";
	WAVHeader header;
	FmtChunk fmt;
	DataChunk data;
};
