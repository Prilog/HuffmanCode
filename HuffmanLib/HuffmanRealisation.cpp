#include "BitStreamReader.h"
#include "HuffmanRealisation.h"
#include <map>
#include <set>
#include <vector>

typedef unsigned long long ull;
typedef uint8_t Byte;

HuffmanRealisation::HuffmanRealisation() {
	size = 0;
	symbols = 0;
	roots.clear();
	roots.resize(ARR_SIZE);
}

void HuffmanRealisation::dfs(Node* node, Byte len, ull val) {
	if (node->left == nullptr) {
		result[node->character] = std::make_pair(val, len);
		return;
	}
	dfs(node->left, len + 1, val * 2);
	dfs(node->right, len + 1, val * 2 + 1);
}

void HuffmanRealisation::encode(std::istream &stream) {
	BitStreamReader input(stream);
	std::vector<ull> counter;
	counter.resize(ARR_SIZE);
	while (!input.eof()) {
		Byte cur = input.read_byte();
		counter[cur]++;
		symbols++;
	}
	std::set<std::pair<ull, Node*> > space;
	for (size_t i = 0; i < CHAR_SIZE; i++) {
		if (counter[i] > 0) {
			add_node(Byte(i), counter[i]);
			space.insert(std::make_pair(counter[i], roots[size - 1]));
		}
	}
	if (space.size() == 1) {
		result[Byte(space.begin()->second->character)] = std::make_pair(ull(0), Byte(1));
		return;
	}
	while (space.size() > 1) {
		Node* left = space.begin()->second;
		space.erase(space.begin());
		Node* right = space.begin()->second;
		space.erase(space.begin());
		add_node(0, left->val + right->val);
		roots[size - 1]->left = left;
		roots[size - 1]->right = right;
		space.insert(std::make_pair(left->val + right->val, roots[size - 1]));
	}
	if (space.size() > 0) {
		dfs(space.begin()->second, 0, 0);
	}
}

HuffmanRealisation::~HuffmanRealisation() {
	roots.clear();
	result.clear();
}

std::map <Byte, std::pair<ull, Byte> > HuffmanRealisation::get_result() {
	return result;
}

void HuffmanRealisation::add_node (Byte c, ull v) {
	roots[size++] = new Node(c, v);
}

void HuffmanRealisation::clear() {
	roots.clear();
	size = 0;
	symbols = 0;
	result.clear();
}
