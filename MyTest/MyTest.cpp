#include "../HuffmanLib/HuffmanLib.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void encode_decode(string file1, string file2, string file3) {
	ifstream input(file1.c_str(), std::ifstream::binary);
	ofstream output(file2.c_str(), std::ofstream::binary);
	try {
		HuffmanLib::encode(input, output);
	}
	catch (damaged_file_exception const &e) {
		throw e;
	}
	input.close();
	output.close();
	input = ifstream(file2.c_str(), std::ifstream::binary);
	output = ofstream(file3.c_str(), std::ofstream::binary);
	try {
		HuffmanLib::decode(input, output);
	}
	catch (damaged_file_exception const &e) {
		throw e;
	}
}

bool compare_files(string file1, string file2) {
	ifstream f1(file1.c_str(), std::ifstream::binary);
	ifstream f2(file2.c_str(), std::ifstream::binary);
	char* c1 = new char;
	char* c2 = new char;
	bool flag = true;
	while (flag) {
		f1.read(c1, 1);
		f2.read(c2, 1);
		size_t r1 = f1.gcount();
		size_t r2 = f2.gcount();
		if (r1 == 0 && r2 == 0) {
			break;
		}
		if (r1 == 0 || r2 == 0 || (*c1 != *c2)) {
			flag = false;
		}
	}
	return flag;
}

bool test_the_test_not_equal() {
	return compare_files("putin.txt", "trap.txt");
}

bool test_the_test_equal() {
	return compare_files("putin.txt", "putin_reptile.txt");
}

bool test_something(string name) {
	encode_decode(name, "code.txt", "result.txt");
	return compare_files(name, "result.txt");
}

string const polochka = "----------------------------\n";

string askii[10] = {
"putin",
"trap",
"digtrio",
"uganda",
"caster",
"empty",
"abacaba",
"war_and_peace",
"doklad_po_istorii",
"a"
};

string exceptions[4] = {
"empty",
"broken_alphabet",
"broken_code",
"no_last_byte"
};

void multitest(size_t amount) {
	cerr << "Testing different cases\n";
	for (size_t i = 0; i < amount; i++) {
		cerr << "Testing " << askii[i] << "\n";
		try {
			if (test_something(askii[i] + ".txt")) {
				cerr << "DONE";
			}
			else {
				cerr << "WRONG ENCODE/DECODE";
			}
		}
		catch (exception const &error) {
			cerr << "Found an exception: " << error.what();
		}
		cerr << "\n";
	}
	cerr << polochka;
}

void test_as(size_t maxx) {
	char* a = new char;
	*a = 'a';
	cerr << "Testing a lot of a\n";
	for (size_t i = 0; i < maxx; i++) {
		cerr << i + 1 << " a\n";
		ofstream f1("a_test.txt", std::ofstream::binary);
		for (size_t i = 0; i < maxx; i++) {
			f1.write(a, 1);
		}
		f1.close();
		try {
			if (test_something("a_test.txt")) {
				cerr << "DONE";
			}
			else {
				cerr << "WRONG ENCODE/DECODE";
			}
		}
		catch (exception const &error) {
			cerr << "Found an exception: " << error.what();
		}
		cerr << "\n";
	}
	cerr << polochka;
}

void random_test(size_t size) {
	cerr << "Testing random " << size << " length\n";
	ofstream f1("random.txt", std::ofstream::binary);
	for (size_t i = 0; i < size; i++) {
		char* a = new char;
		*a = char(ull(rand()) % ull(256));
		f1.write(a, 1);
	}
	f1.close();
	try {
		if (test_something("random.txt")) {
			cerr << "DONE";
		}
		else {
			cerr << "WRONG ENCODE/DECODE";
		}
	}
	catch (exception const &error) {
		cerr << "Found an exception: " << error.what();
	}
	cerr << "\n" << polochka;
}

void random_tests(size_t maxx) {
	cerr << "Testing random tests\n";
	for (size_t i = 0; i < maxx; i++) {
		cerr << i + 1 << " length\n";
		ofstream f1("random.txt", std::ofstream::binary);
		for (size_t i = 0; i < maxx; i++) {
			char* a = new char;
			*a = char(ull(rand()) % ull(256));
			f1.write(a, 1);
		}
		f1.close();
		try {
			if (test_something("random.txt")) {
				cerr << "DONE";
			}
			else {
				cerr << "WRONG ENCODE/DECODE";
			}
		}
		catch (exception const &error) {
			cerr << "Found an exception: " << error.what();
		}
		cerr << "\n";
	}
	cerr << polochka;
}

void exception_test(size_t amount) {
	cerr << "Testing different exceptions\n";
	for (size_t i = 0; i < amount; i++) {
		cerr << "Testing " << exceptions[i] << "\n";
		ifstream input(exceptions[i] + ".txt", std::ifstream::binary);
		ofstream output("result.txt", std::ofstream::binary);
		try {
			HuffmanLib::decode(input, output);
			cerr << "NO EXCEPTION...NOT GOOD";
		}
		catch (exception const &error) {
			cerr << "Found an exception: " << error.what() << " AND THATS GOOD";
		}
		cerr << "\n";
	}
	cerr << polochka;
}

int main()
{
	multitest(10);
	test_as(500);
	random_tests(100);
	random_test(100000);
	exception_test(4);
}
