#include "serverBuffer.h"

void serverBuffer::WriteEndianness(int32_t value)
{
	char A = value >> 24;
	char B = value >> 16;
	char C = value >> 8;
	char D = value >> 0;

	_values->at(this->write) = D;
	_values->at(this->write+1) = C;
	_values->at(this->write+2) = B;
	_values->at(this->write+3) = A;

	this->write += 4;
}

uint32_t serverBuffer::ReadEndianness()
{
	uint32_t readReturn = 0;

	readReturn |= _values->at(this->read) << 0;
	readReturn |= _values->at(this->read+1) << 8;
	readReturn |= _values->at(this->read+2) << 16;
	readReturn |= _values->at(this->read+3) << 24;

	this->read += 4;

	return readReturn;
}

serverBuffer::serverBuffer(size_t size) {
	this->read = 0;
	this->write = 0;
	this->_values = new std::vector<uint8_t>(size, 0);
}

void serverBuffer::writeInt32BE(size_t index, int32_t value)
{
	this->write = index;
	serverBuffer::WriteEndianness(value);
}
void serverBuffer::writeInt32BE(int32_t value)
{
	serverBuffer::WriteEndianness(value);
}
uint32_t serverBuffer::readUInt32LE()
{
	return this->ReadEndianness();
}

void serverBuffer::writeString(size_t index, std::string value)
{
	this->write = index;
	for (int i = 0; i < value.size(); ++i, ++this->write)
	{
		_values->at(this->write) = value.at(i);
	}
}
void serverBuffer::writeString(std::string value)
{
	for (int i = 0; i < value.size(); ++i, ++this->write)
	{
		_values->at(this->write) = value.at(i);
	}
}
std::string serverBuffer::readString(size_t index, size_t length)
{
	std::string readReturn = "";
	this->read = index;
	for (int i = 0; i < length; ++i, ++this->read)
	{
		readReturn += _values->at(this->read);
	}
	return readReturn;
}
std::string serverBuffer::readString(size_t length)
{
	std::string readReturn = "";
	for (int i = 0; i < length; ++i, ++this->read)
	{
		readReturn += _values->at(this->read);
	}
	return readReturn;
}