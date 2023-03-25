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
}

//If the given value is nullptr, this->value will not be changed
void TableValue::setValue(const char* value) {
	if (myStrlen(value) > MAX_NUMBER_OF_SYMBOLS) {
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


