#include <iostream>

#include "TableValue.h"

TableCell::TableCell() : TableCell(DEFAULT_VAL) {

}

TableCell::TableCell(const char* value) {
	setValue(value);
}

TableCell::~TableCell() {

}

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

void TableCell::setValue(const char* value) {
	//If the new value is invalid, set this->value to "Unknown"
	if (myStrlen(value) > MAX_NUMBER_OF_SYMBOLS || value == nullptr) {
		myStrcpy(this->value, DEFAULT_VAL);
	}

	else {
		myStrcpy(this->value, value);
	}
}

const char* TableCell::getValue() const {
	return value;
}

size_t TableCell::getValueLength() const {
	return myStrlen(getValue());
}

// Index is the number of the value's column 
void TableCell::writeCellToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index, size_t numberOfCells, char charToWrite) const {
	os << ' ' << PIPE << ' ';

	size_t length = getValueLength();
	size_t numberOfChars = width - length;

	if (alignment == Alignment::center) {
		numberOfChars /= 2;
	}

	if (alignment != Alignment::left) {
		printSymbolNTimes(os, charToWrite, numberOfChars);
	}

	os << getValue();

	if (alignment == Alignment::left){
		printSymbolNTimes(os, charToWrite, numberOfChars);
	}

	//Evenly distribute the spaces on both sides
	else if (alignment == Alignment::center) {
		printSymbolNTimes(os, charToWrite, width - length - numberOfChars);
	}

	//If the value to be printed is the last one, add one extra '|'
	if (index == numberOfCells - 1) {
		os << " " << PIPE << "\n";
	}
}

void TableCell::printValue(const Alignment& alignment, size_t width, size_t index, size_t numberOfCells, char charToWrite) const {
	writeCellToStream(std::cout, alignment, width, index, numberOfCells, charToWrite);
}
