#include <istream>
#include <ostream>
#include "HuffmanRealisation.h"
#include "BitStreamReader.h"
#include "BitStreamWriter.h"
#include "HuffmanLib.h"
#include "Exceptions.h"

void HuffmanLib::encode(std::istream &input, std::ostream &output) {
	auto start = input.tellg();
	HuffmanRealisation magic;
	magic.encode(input);
	std::map <Byte, std::pair<ull, Byte> > res = magic.get_result();
	input.clear();
	input.seekg(start);
	BitStreamWriter writer(output);
	//writing header
	writer.write_ull(magic.get_size());
	if (res.size() == 0) {
		res[0] = std::make_pair(0, 1);
	}
	writer.write_byte(Byte(res.size() - 1));
	for (auto code : res) {
		writer.write_byte(code.first);
		writer.write_byte(code.second.second);
		for (int i = (int(code.second.second) + 7) / 8; i > 0; i--) {
			writer.write_byte(Byte((code.second.first >> (ull(i - 1) * 8)) % ull(256)));
		}
	}
	//
	ull length = 0;
	BitStreamReader reader(input);
	while (!reader.eof()) {
		Byte c = reader.read_byte();
		length += res[c].second;
		for (size_t i = 0; i < res[c].second; i++) {
			//std::clog << *c << ' ' << res[*c].first << "\n";
			//writer.write_byte('a');
			size_t move = res[c].second - i - 1;
			writer.write_bit((res[c].first & (1ull << move)) >> move);
		}
	}
	length %= 8;
	writer.write_byte(char(length));
}

void parse_byte(Byte &last2, size_t i, ull &buf, ull &buf_size, std::map <Byte, std::pair<ull, Byte> > &code, BitStreamWriter &writer, ull &balance) {
	ull bit = ull((last2 >> (7 - i)) % 2);
	buf *= 2;
	buf |= bit;
	buf_size++;
	for (auto object : code) {
		if (object.second.first == buf && buf_size == object.second.second) {
			writer.write_byte(object.first);
			balance += 1;
			buf = 0;
			buf_size = 0;
			break;
		}
	}
}

void HuffmanLib::decode(std::istream &input, std::ostream &output) {
	std::map <Byte, std::pair<ull, Byte> > code;
	BitStreamReader reader(input);
	BitStreamWriter writer(output);
	//reading of header
	ull symbols;
	try {
		symbols = reader.read_ull();
		size_t amount = size_t(reader.read_byte()) + 1;
		for (size_t i = 0; i < amount; i++) {
			Byte symbol = reader.read_byte();
			Byte length = reader.read_byte();
			ull s_code = 0;
			for (size_t i = 0; i < (size_t(length) + 7) / 8; i++) {
				Byte cur = reader.read_byte();
				s_code *= 256;
				s_code += ull(cur);
			}
			code[symbol] = std::make_pair(s_code, length);
		}
	}
	catch (std::istream::failure const &e) {
		throw damaged_file_exception();
	}
	//
	int flag = 0;
	Byte last = 0, last2 = 0;
	ull buf = 0, buf_size = 0;
	ull balance = 0;
	while (!reader.eof()) {
		if (flag < 2) {
			flag++;
		}
		else {
			for (size_t i = 0; i < 8; i++) {
				parse_byte(last2, i, buf, buf_size, code, writer, balance);
			}
		}
		last2 = last;
		try {
			last = reader.read_byte();
		}
		catch (std::istream::failure const &e) {
			throw damaged_file_exception();
		}
	}
	if (flag == 2) {
		if (last == 0) {
			last = 8;
		}
		for (size_t i = 0; i < last; i++) {
			parse_byte(last2, i, buf, buf_size, code, writer, balance);
		}
	}
	if (balance != symbols) {
		throw unexpected_end_of_file_exception();
	}
}
