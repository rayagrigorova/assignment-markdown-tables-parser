#include <iostream>
#include <fstream>
#include <sstream>

#include "TableRow.h"

TableRow::TableRow(){
	numberOfValues = 0;

	for (int i = 0; i < MAX_NUMBER_OF_COLS; i++) {
		values[i].setValue(DEFAULT_VAL);
	}
}

TableRow::TableRow(const TableValue* values, size_t numberOfValues) {
	setNumberOfValues(numberOfValues);
	setValues(values, this->numberOfValues);
}

TableRow::TableRow(const char** values, size_t numberOfValues) {
	setNumberOfValues(numberOfValues);
	setValues(values, this->numberOfValues);
}

TableRow::~TableRow() {

}

void TableRow::setNumberOfValues(size_t numberOfValues) {
	if (numberOfValues > MAX_NUMBER_OF_COLS) {
		return;
	}
	this->numberOfValues = numberOfValues;
}

void TableRow::setValues(const char* values, size_t count) {
	if (count > MAX_NUMBER_OF_COLS) {
		return;
	}

	std::stringstream ss(values);
	// Skip the first pipe symbol
	ss.get();

	char buff[(MAX_NUMBER_OF_SYMBOLS + 1) * MAX_NUMBER_OF_COLS];
	for (int i = 0; i < count; i++) {
		ss.getline(buff, MAX_NUMBER_OF_SYMBOLS, PIPE);
		this->values[i].setValue(buff);
	}
}

void TableRow::setValues(const TableValue* values, size_t count) {
	if (count > MAX_NUMBER_OF_COLS) {
		return;
	}

	for (int i = 0; i < this->numberOfValues; i++) {
		this->values[i].setValue(values[i].getValue());
	}
}

size_t TableRow::getNumberOfValues() const {
	return numberOfValues;
}

const TableValue* TableRow::getValues() const {
	return values;
}

//void TableRow::print() const {
//	for (int i = 0; i < numberOfValues; i++) {
//		std::cout << values[i].getValue() << " ";
//	}
//	std::cout << "\n";
//}

void TableRow::setValueAtIndex(const TableValue& value, size_t index) {
	if (index > numberOfValues) {
		return;
	}
	values[index].setValue(value.getValue());
}

void TableRow::setValueAtIndex(const char* value, size_t index) {
	if (index > numberOfValues) {
		return;
	}
	values[index].setValue(value);
}


const TableValue& TableRow::getValueAtIndex(size_t index) const{
	if (index > numberOfValues) {
		return nullptr;
	}
	return values[index];
}

namespace {
	void printSymbolNTimes(std::ostream& os, const char symbol, const int N) {
		for (int i = 0; i < N; i++) {
			os << symbol;
		}
	}

	size_t countCharacterOccurance(const char* str, char ch) {
		if (str == nullptr) {
			return 0;
		}

		size_t ctr = 0;
		while (*str) {
			if (*(str) == ch) {
				ctr++;
			}
			str++;
		}
		return ctr;
	}
}

void TableRow::writeValueToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index) const{
	//If the value to be printed isn't the rightmost one
	if (index + 1 != numberOfValues) {
		os << PIPE << " ";
	}

	size_t numberOfSpaces = width - getValueAtIndex(index).getValueLength();

	if (alignment == Alignment::center) {
		numberOfSpaces /= 2;
	}

	if (alignment != Alignment::left) {
		printSymbolNTimes(os, ' ', numberOfSpaces);
	}

	os << getValueAtIndex(index).getValue();
	
	if (alignment == Alignment::right) {
		return;
	}

	printSymbolNTimes(os, ' ', numberOfSpaces);

	//If the value to be printed isn't the leftmost one
	if (index != 0) {
		os << PIPE << " ";
	}
}


void TableRow::readValueFromStream(std::ifstream& ifs){
	// Each of the values contains a maximum of 20 symbols + '\0'
	const size_t BUFF_SIZE = (MAX_NUMBER_OF_SYMBOLS + 1) * MAX_NUMBER_OF_COLS;
	char buff[BUFF_SIZE];
	ifs.getline(buff, BUFF_SIZE);

	setValues(buff, countCharacterOccurance(buff, PIPE) - 1);	
}

void TableRow::printValue(const Alignment& alignment, size_t width, size_t index) const {
	writeValueToStream(std::cout, alignment, width, index);
}


