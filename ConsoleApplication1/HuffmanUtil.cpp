#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <fstream>
#include "../HuffmanLib/HuffmanLib.h"
#include "../HuffmanLib//Exceptions.h"

using namespace std;

const string help_message[4] = { "To encode file type \"encode\" + (input file name) + (output file name)\n",
"To decode file type \"decode\" + (input file name) + (output file name)\n",
"To call help type \"help\"\n",
"To stop the programm type \"POI\"\n" };
const string wrong_command_message = "Wrong command, read message above or type \"help\" to do something usefull.\n";
const string bad_file_message = "Cant open/read files.\n";
const string ready_for_action_message = "-->";
const string end_of__action_message = "-------success---------\n";
const string exception_found_message = "----exception-found----\n";

bool neko_huffman(int type, string f1, string f2) {
	try {
		ifstream input(f1.c_str(), std::ifstream::binary);
		input.exceptions(ifstream::failbit);
		ofstream output(f2.c_str(), std::ofstream::binary);
		output.exceptions(ofstream::failbit);
	}
	catch (std::ifstream::failure const &error) {
		cerr << bad_file_message;
		return false;
	}
	try {
		ifstream input(f1.c_str(), std::ifstream::binary);
		ofstream output(f2.c_str(), std::ofstream::binary);
		if (type == 1) {
			HuffmanLib::encode(input, output);
		}
		else {
			HuffmanLib::decode(input, output);
		}
	}
	catch (damaged_file_exception const &e) {
		cerr << e.what() << "\n";
		return false;
	}
	return true;
}

void write_help() {
	for (size_t i = 0; i < 4; i++) {
		cerr << help_message[i];
	}
}

int do_action() {
	string action;
	cin >> action;
	int type = 0;
	if (action == "encode") {
		type = 1;
	}
	else if (action == "decode") {
		type = 2;
	}
	else if (action == "help") {
		write_help();
		type = 4;
	}
	else if (action == "POI") {
		type = 3;
	}
	if (type == 0) {
		cerr << wrong_command_message;
	}
	else if (type == 3) {
		return 2;
	}
	else if (type < 3) {
		string input, output;
		cin >> input >> output;
		if (!neko_huffman(type, input, output)) {
			return 0;
		}
	}
	return 1;
}

int main() {
	write_help();
	int status = 1;
	while (status != 2) {
		cerr << ready_for_action_message;
		status = do_action();
		if (status == 1) {
			cerr << end_of__action_message;
		}
		else if (status == 0) {
			cerr << exception_found_message;
		}
	}
	return 0;
}