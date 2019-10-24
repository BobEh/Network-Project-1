#pragma once

#include <iostream>
#include <bitset>
#include <vector>

class serverBuffer
{
public:
	serverBuffer(size_t size);
	void writeInt32BE(size_t index, int32_t value);
	void writeInt32BE(int32_t value);
	uint32_t readUInt32LE();
	void writeString(size_t index, std::string value);
	void writeString(std::string value);
	std::string readString(size_t index, size_t length);
	std::string readString(size_t length);
	void WriteEndianness(int32_t value);
	uint32_t ReadEndianness();

private:
	unsigned int read;
	unsigned int write;
	std::vector<uint8_t>* _values;
};