#include <iostream>

#include "TableValue.h"

TableValue::TableValue() : TableValue(DEFAULT_VAL) {

}

TableValue::TableValue(const char* value) {
	setValue(value);
}

TableValue::~TableValue() {

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

void TableValue::setValue(const char* value) {
	//If the new value is invalid, set this->value to "Unknown"
	if (myStrlen(value) > MAX_NUMBER_OF_SYMBOLS || value == nullptr) {
		myStrcpy(this->value, DEFAULT_VAL);
	}

	else {
		myStrcpy(this->value, value);
	}
}

const char* TableValue::getValue() const {
	return value;
}

size_t TableValue::getValueLength() const {
	return myStrlen(getValue());
}

// Index is the number of the value's column 
void TableValue::writeValueToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index, size_t numberOfValues) const {
	//If the value to be printed isn't the rightmost one
	if (index + 1 != numberOfValues) {
		os << PIPE << " ";
	}

	size_t numberOfSpaces = width - getValueLength();

	//Evenly distribute the spaces on both sides
	if (alignment == Alignment::center) {
		numberOfSpaces /= 2;
	}

	if (alignment != Alignment::left) {
		printSymbolNTimes(os, ' ', numberOfSpaces);
	}

	os << getValue();

	if (alignment == Alignment::right) {
		return;
	}

	printSymbolNTimes(os, ' ', numberOfSpaces);

	//If the value to be printed isn't the leftmost one
	if (index != 0) {
		os << PIPE << " ";
	}
}

void TableValue::printValue(const Alignment& alignment, size_t width, size_t index, size_t numberOfValues) const {
	writeValueToStream(std::cout, alignment, width, index, numberOfValues);
}
