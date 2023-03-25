#include <iostream>

#include "TableColumn.h"

TableColumn::TableColumn() {
	numberOfValues = 0;

	for (int i = 0; i < MAX_NUMBER_OF_ROWS; i++) {
		values[i].setValue(DEFAULT_VAL);
	}

	alignment = Alignment::left;
}

TableColumn::TableColumn(const TableValue* values, size_t numberOfValues) {
	setNumberOfValues(numberOfValues);
	setValues(values, this->numberOfValues);
}

TableColumn::TableColumn(const char** values, size_t numberOfValues) {
	setNumberOfValues(numberOfValues);
	setValues(values, this->numberOfValues);
}

TableColumn::~TableColumn() {

}

void TableColumn::setNumberOfValues(size_t numberOfValues) {
	if (numberOfValues > MAX_NUMBER_OF_ROWS) {
		return;
	}
	this->numberOfValues = numberOfValues;
}

void TableColumn::setValues(const char** values, size_t count) {
	if (count > MAX_NUMBER_OF_ROWS) {
		return;
	}

	for (int i = 0; i < count; i++) {
		this->values[i].setValue(values[i]);
	}

}

void TableColumn::setValues(const TableValue* values, size_t count) {
	if (count > MAX_NUMBER_OF_ROWS) {
		return;
	}

	for (int i = 0; i < this->numberOfValues; i++) {
		this->values[i].setValue(values[i].getValue());
	}
}

void TableColumn::setAlignment(const Alignment& alignment) {
	this->alignment = alignment;
}

size_t TableColumn::getNumberOfValues() const {
	return numberOfValues;
}

const Alignment& TableColumn::getAlignment() const {
	return this->alignment;
}

const TableValue* TableColumn::getValues() const {
	return values;
}

void TableColumn::print() const {
	for (int i = 0; i < numberOfValues; i++) {
		std::cout << values[i].getValue() << " ";
	}
	std::cout << "\n";
}

void TableColumn::setValueAtIndex(const TableValue& value, size_t index) {
	if (index > numberOfValues) {
		return;
	}
	values[index].setValue(value.getValue());
}

void TableColumn::setValueAtIndex(const char* value, size_t index) {
	if (index > numberOfValues) {
		return;
	}
	values[index].setValue(value);
}


const TableValue& TableColumn::getValueAtIndex(size_t index) const {
	if (index > numberOfValues) {
		return nullptr;
	}
	return values[index];
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
}


size_t TableColumn::getLongestValueLength(size_t columnIndex) {
	if (numberOfValues == 0) {
		return 0;
	}

	size_t maxLen = myStrlen(values[0].getValue());
	for (int i = 1; i < numberOfValues; i++) {
		size_t temp = myStrlen(values[i].getValue());
		if (temp > maxLen) {
			maxLen = temp;
		}
	}
	return maxLen;
}

