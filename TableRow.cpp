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
	setValues(values, numberOfValues);
}

TableRow::~TableRow() {

}

void TableRow::setNumberOfValues(size_t numberOfValues) {
	if (numberOfValues > MAX_NUMBER_OF_COLS) {
		return;
	}
	this->numberOfValues = numberOfValues;
}

void TableRow::setValues(const char** values, size_t numberOfValues) {
	setNumberOfValues(numberOfValues);

	for (int i = 0; i < this->numberOfValues; i++) {
		setValueAtIndex(values[i], i);
	}
}

void TableRow::setValues(const TableValue* values, size_t count) {
	setNumberOfValues(count);

	for (int i = 0; i < this->numberOfValues; i++) {
		setValueAtIndex(values[i], i);
	}
}

size_t TableRow::getNumberOfValues() const {
	return numberOfValues;
}

const TableValue* TableRow::getValues() const {
	return values;
}

void TableRow::setValueAtIndex(const TableValue& value, size_t index) {
	if (index >= numberOfValues) {
		return;
	}
	values[index].setValue(value.getValue());
}

void TableRow::setValueAtIndex(const char* value, size_t index) {
	if (index >= numberOfValues) {
		return;
	}
	values[index].setValue(value);
}

const TableValue& TableRow::getValueAtIndex(size_t index) const{
	if (index >= numberOfValues) {
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

	size_t countCharacterOccurances(const char* str, char ch) {
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

	size_t countCharacterOccurancesInFileLine(std::ifstream& file, char ch) {
		size_t currentPos = file.tellg();

		char current = '\0';
		size_t ctr = 0;

		while (current != '\n') {
			file.get(current);

			if (file.eof()) {
				break;
			}

			if (current == ch) {
				ctr++;
			}
		}

		file.seekg(currentPos);
		return ctr;
	}
}

// Width refers to the width of a column a value is in. 
// The width of the column is determined by the longest value in it.
// The class MarkdownTable takes care of the widths. 

// Index is the number of the value's column 
void TableRow::writeValueToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index) const{
	//If the value to be printed isn't the rightmost one
	if (index + 1 != numberOfValues) {
		os << PIPE << " ";
	}

	size_t numberOfSpaces = width - getValueAtIndex(index).getValueLength();

	//Evenly distribute the spaces on both sides
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

void TableRow::readValuesFromStream(std::ifstream& ifs){
	// Each line in the file is one row in the table
	size_t numberOfValuesInLine = countCharacterOccurancesInFileLine(ifs, PIPE) - 1;

	//Skip the first '|' symbol of the file
	ifs.get();

	char buff[MAX_NUMBER_OF_SYMBOLS + 1];
	for (int i = 0; !ifs.eof() && i < numberOfValuesInLine; i++) {
		//Get all symbols up to '|'
		ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, PIPE);

		values[i].setValue(buff);
	}
	
	//If the end of the file hasn't been reached, go to the next line 
	//so that the next row can be read
	if (!ifs.eof()) {
		ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, '\n');
	}

}

void TableRow::printValue(const Alignment& alignment, size_t width, size_t index) const {
	writeValueToStream(std::cout, alignment, width, index);
}


