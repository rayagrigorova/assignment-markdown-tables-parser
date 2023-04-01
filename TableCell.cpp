#include <iostream>

#include "TableCell.h"

const char HYPHEN = '-';
const char PIPE = '|';
const char COLON = ':';
const char SPACE = ' ';

namespace {
	size_t myStrlen(const char* str) {
		if (str == nullptr) {
			return 0;
		}

		size_t ctr = 0;
		while (*str) {
			ctr++;
			str++;
		}

		return ctr;
	}

	//A destination array size check is required before using this function
	void myStrcpy(char* dest, const char* src) {
		if (dest == nullptr || src == nullptr) {
			return;
		}

		while (*src) {
			*(dest) = *(src);

			dest++;
			src++;
		}
		*(dest) = '\0';
	}

	void printSymbolNTimes(std::ostream& os, const char symbol, const int N) {
		for (int i = 0; i < N; i++) {
			os << symbol;
		}
	}
}

TableCell::TableCell() : TableCell(DEFAULT_VAL) {

}

TableCell::TableCell(const char* value) {
	setValue(value);
}

bool TableCell::setValue(const char* value) {
	//If the new value is invalid, set this->value to "Unknown"
	if (myStrlen(value) > MAX_NUMBER_OF_SYMBOLS || value == nullptr) {
		myStrcpy(this->value, DEFAULT_VAL);
		return false;
	}

	else {
		myStrcpy(this->value, value);
		return true;
	}
}

const char* TableCell::getValue() const {
	return value;
}

size_t TableCell::getValueLength() const {
	return myStrlen(getValue());
}

// Index is the number of the cell's column 
// Width refers to the width of the cell's column. 
bool TableCell::writeCellToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const {
	// Check for error state flags
	if (!os.good()) {
		return false;
	}

	os << SPACE << PIPE << SPACE;

	size_t length = getValueLength();
	size_t charsToPrintCount = width - length;

	if (alignment == Alignment::center) {
		charsToPrintCount /= 2;
	}

	if (alignment != Alignment::left) {
		printSymbolNTimes(os, SPACE, charsToPrintCount);
	}

	os << getValue();

	if (alignment == Alignment::left){
		printSymbolNTimes(os, SPACE, charsToPrintCount);
	}

	//Evenly distribute the spaces on both sides
	else if (alignment == Alignment::center) {
		printSymbolNTimes(os, SPACE, width - length - charsToPrintCount);
	}

	//If the value to be printed is the last one, add one extra '|'
	if (index == numberOfCells - 1) {
		os << SPACE << PIPE;
	}

	return true;
}

bool TableCell::printValue(const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const {
	return writeCellToStream(std::cout, alignment, width, index, numberOfCells);
}
