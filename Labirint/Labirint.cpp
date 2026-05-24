#include "Header.h"

bool Node::operator>(const Node& other) const {
	return dist > other.dist;
}