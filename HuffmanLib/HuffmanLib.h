#pragma once

#include <istream>
#include <ostream>
#include "HuffmanRealisation.h"
#include "BitStreamWriter.h"

namespace HuffmanLib {
	void encode(std::istream &input, std::ostream &output);
	void decode(std::istream &input, std::ostream &output);
}