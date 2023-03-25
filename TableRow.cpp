#include <iostream>

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

void TableRow::setValues(const char** values, size_t count) {
	if (count > MAX_NUMBER_OF_COLS) {
		return;
	}

	for (int i = 0; i < count; i++) {
		this->values[i].setValue(values[i]);
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

void TableRow::print() const {
	for (int i = 0; i < numberOfValues; i++) {
		std::cout << values[i].getValue() << " ";
	}
	std::cout << "\n";
}
