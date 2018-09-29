#pragma once

#include <istream>
#include <vector>
#include "Exceptions.h"

typedef uint8_t Byte;
typedef unsigned long long ull;

class BitStreamReader {
	static const size_t ARR_SIZE = 1024;

	std::istream &reader;
	std::vector<Byte> buf;
	size_t first = 0;
	size_t bit_number = 0;
	bool end = false;

	void update() {
		buf.clear();
		first = 0;
		bit_number = 0;
		char* character = new char[ARR_SIZE];
		try {
			reader.read(character, ARR_SIZE);
		}
		catch (std::istream::failure const &error) {
			throw damaged_file_exception();
		}
		size_t count = size_t(reader.gcount());
		if (count == 0) {
			end = true;
		}
		buf.resize(count);
		for (size_t i = 0; i < reader.gcount(); i++) {
			buf[i] = character[i];
		}
	}

	void move_byte() {
		first++;
		bit_number = 0;
		if (first >= buf.size()) {
			update();
		}
	}

public:
	explicit BitStreamReader(std::istream &stream) 
		: reader(stream){
		//?
		update();
	}

	bool read_bit() {
		if (buf.size() == 0) {
			throw damaged_file_exception();
		}
		auto res = ((buf[first] << bit_number) >> (7 - bit_number));
		bit_number++;
		if (bit_number == 8) {
			move_byte();
		}
		return res;
	}

	Byte read_byte() { //read current byte, if bit_number != 0 OCHEN ZHALY
		if (buf.size() == 0) {
			throw damaged_file_exception();
		}
		Byte res = buf[first];
		move_byte();
		return res;
	}

	ull read_ull() {
		ull res = 0;
		for (size_t i = 0; i < 8; i++) {
			res *= 256;
			res += ull(read_byte());
		}
		return res;
	}

	bool eof() const {
		return end;
	}
};