#pragma once
#include <vector>
class CByteBuf {
public:
	CByteBuf(int maxcapcity) {
		buffer.resize(maxcapcity);
		writeIndex = 0;
		readIndex = 0;
	}
	int availableBytes() {
		return writeIndex - readIndex;
	}
	void write(const char *buf, int count);
	void writeSkip(int bytes) {
		writeIndex += bytes;
	}
	int read(char *buf, int count);
	void trunc();
	void reset() {
		readIndex = 0;
		writeIndex = 0;
	}
public:
	char *getWriteBuffer() {
		return &buffer[writeIndex];
	}
	int getCapcityForWrite() {
		return (int)buffer.size() - writeIndex;
	}
private:

	int writeIndex;
	int readIndex;
	std::vector<char> buffer;
};


