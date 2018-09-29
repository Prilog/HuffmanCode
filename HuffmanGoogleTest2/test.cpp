#include "pch.h"

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

TEST(TestTheTest, Putin) {
	EXPECT_TRUE(false);
	EXPECT_TRUE(compare_files("putin.txt", "trap.txt"));
}

/*
TEST(Pre_generated_ASKII_arts, Putin) {
	string test_file = "putin.txt";
	encode_decode(test_file, "code.txt", "result.txt");
	EXPECT_TRUE(compare_files(test_file, "result.txt"));
}

TEST(Pre_generated_ASKII_arts, Digtrio) {
	string test_file = "digtrio.txt";
	encode_decode(test_file, "code.txt", "result.txt");
	EXPECT_TRUE(compare_files(test_file, "result.txt"));
}

TEST(Pre_generated_ASKII_arts, Trap) {
	string test_file = "trap.txt";
	encode_decode(test_file, "code.txt", "result.txt");
	EXPECT_TRUE(compare_files(test_file, "result.txt"));
}

TEST(Pre_generated_ASKII_arts, Uganda) {
	string test_file = "uganda.txt";
	encode_decode(test_file, "code.txt", "result.txt");
	EXPECT_TRUE(compare_files(test_file, "result.txt"));
}

TEST(Pre_generated_ASKII_arts, Caster) {
	string test_file = "caster.txt";
	encode_decode(test_file, "code.txt", "result.txt");
	EXPECT_TRUE(compare_files(test_file, "result.txt"));
}*/