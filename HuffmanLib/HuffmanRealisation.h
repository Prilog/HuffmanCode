#pragma once

#include "BitStreamReader.h"
#include <map>
#include <vector>

typedef unsigned long long ull;

const size_t ARR_SIZE = 1024;
const size_t CHAR_SIZE = 256;

struct HuffmanRealisation {
private:
	std::map <Byte, std::pair<ull, Byte> > result;

	struct Node {
		Byte character;
		ull val;
		Node* left;
		Node* right;

		Node() {
			character = 0;
			val = 0;
			left = nullptr;
			right = nullptr;
		}

		Node(Byte c, ull v) {
			character = c;
			val = v;
			left = nullptr;
			right = nullptr;
		}

		~Node() {
			delete left;
			delete right;
		}
	};

	std::vector<Node*> roots;
	size_t size;
	ull symbols;

	void add_node(Byte c, ull v);

	void dfs(Node* node, Byte len, ull val);

public:
	HuffmanRealisation();

	void encode(std::istream &stream);

	~HuffmanRealisation();

	std::map <Byte, std::pair<ull, Byte> > get_result();

	ull get_size() {
		return symbols;
	}

	void clear();
};