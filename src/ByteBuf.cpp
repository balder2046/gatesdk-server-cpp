#include "ByteBuf.h"

void CByteBuf::write(const char *buf, int count) {
	memcpy(&buffer[writeIndex], buf, count);
	writeIndex += count;
}
int CByteBuf::read(char *buf, int count) {
	int available = availableBytes();
	int realcount = available < count ? available : count;
	memcpy(buf, &buffer[readIndex], realcount);
	return realcount;
}
void CByteBuf::trunc() {
	if (readIndex == writeIndex) {
		readIndex = 0;
		writeIndex = 0;
	}
	
	else if (readIndex > 0)
	{
		std::copy(buffer.begin() + readIndex, buffer.begin() + writeIndex, buffer.begin());
		writeIndex = writeIndex - readIndex;
		readIndex = 0;
	}
}