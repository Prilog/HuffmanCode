#pragma once

#include <string>
#include <exception>

struct damaged_file_exception : std::exception {
	const char *what() const noexcept override {
		return "File damaged";
	}
};

struct cant_write_exception : std::exception {
	const char *what() const noexcept override {
		return "Cant write in file.";
	}
};

struct unexpected_end_of_file_exception : damaged_file_exception {
	const char *what() const noexcept override {
		return "Unexpected end of file. It can be damaged";
	}
};