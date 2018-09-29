#pragma once

#include <ostream>
#include <vector>
#include "Exceptions.h"

typedef uint8_t Byte;
typedef unsigned long long ull;

class BitStreamWriter {
	static const size_t ARR_SIZE = 1024;

	std::ostream &writer;
	std::vector<Byte> buf;
	size_t cur = 0;
	size_t bit_number = 0;

	void write() {
		if (bit_number != 0) {
			cur++;
		}
		char* buffer = new char[cur];
		for (size_t i = 0; i < cur; i++) {
			buffer[i] = buf[i];
		}
		try {
			writer.write(buffer, cur);
		}
		catch (std::istream::failure const &error) {
			throw cant_write_exception();
		}
		cur = 0;
		bit_number = 0;
		buf.clear();
		buf.resize(ARR_SIZE);
	}

	void move_byte() {
		cur++;
		bit_number = 0;
		if (cur >= buf.size()) {
			write();
		}
	}

public:
	explicit BitStreamWriter(std::ostream &stream)
		: writer(stream) {
		//?
		buf.clear();
		buf.resize(ARR_SIZE);
	}

	~BitStreamWriter() {
		write();
		writer.flush();
	}

	void write_bit(bool bit) {
		buf[cur] |= (bit << (7 - bit_number));
		//buf[cur] |= bit << bit_number;
		bit_number++;
		if (bit_number == 8) {
			move_byte();
		}
	}

	void write_byte(Byte byte) { //write byte, if bit_number != 0, move_byte first
		if (bit_number != 0) {
			move_byte();
		}
		buf[cur] = byte;
		move_byte();
	}

	void write_ull(ull arg) {
		for (size_t i = 0; i < 8; i++) {
			write_byte(Byte((arg << (8 * i)) >> (7 * 8)));
		}
	}
};