#include <iostream>
#include <fstream>

#include "MarkdownTable.h"

MarkdownTable::MarkdownTable() : rows(){
	numberOfRows = 0;
	numberOfColumns = 0;
	initAlignments();
}

MarkdownTable::MarkdownTable(const TableRow& columnNames, const TableRow* rows, size_t numberOfRows, size_t numberOfColumns) {
	setColumnNames(columnNames);
	setRows(rows, numberOfRows);
	setNumberOfRows(numberOfRows);
	initAlignments();
}

MarkdownTable::MarkdownTable(const char** columnNames, const char** rows, size_t numberOfRows, size_t numberOfColumns) {
	setColumnNames(columnNames, numberOfRows);
	setRows(rows, numberOfRows);
	setNumberOfRows(numberOfRows);
	initAlignments();
}
MarkdownTable::MarkdownTable(const char* fileName) {
	loadFromFile(fileName);
}

MarkdownTable::~MarkdownTable() {

}

const TableRow* MarkdownTable::getRows() const {
	return rows;
}

const size_t MarkdownTable::getNumberOfRows() const {
	return numberOfRows;
}

const size_t MarkdownTable::getNumberOfColumns() const {
	return numberOfColumns;
}

void MarkdownTable::setColumnNames(TableRow columnNames) {
	//If the number of columns in the table doesn't match the number of 
	//columns of the given value, return. 
	size_t numberOfCols = columnNames.getNumberOfValues();
	if (numberOfCols != this->numberOfColumns) {
		return;
	}

	for (int i = 0; i < numberOfCols; i++) {
		this->rows[0].setValueAtIndex(columnNames.getValueAtIndex(i), i);
	}
}

void MarkdownTable::setColumnNames(const char** columnNames, size_t numberOfCols) {
	if (numberOfCols != this->numberOfColumns) {
		return;
	}
	for (int i = 0; i < numberOfCols; i++) {
		this->rows[0].setValueAtIndex(columnNames[i], i);
	}
}

void MarkdownTable::setRows(const TableRow* rows, size_t numberOfRows) {
	if (numberOfRows > MAX_NUMBER_OF_ROWS) {
		return;
	}

	for (int i = 0; i < numberOfRows; i++) {
		this->rows[i].setValueAtIndex(rows->getValueAtIndex(i), i);
	}
}

void MarkdownTable::setRows(const char** rows, size_t numberOfRows) {
	///
}

void MarkdownTable::setNumberOfRows(size_t numberOfRows) {
	if (numberOfRows <= MAX_NUMBER_OF_ROWS) {
		this->numberOfRows = numberOfRows;
	}
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

	int findSymbolInString(const char* str, const char symbol) {
		if (str == nullptr) {
			return -1;
		}

		int foundInd = -1, ctr = 0;;
		while (*str) {
			if (*(str) == symbol) {
				foundInd = ctr;
				break;
			}
			ctr++;
			str++;
		}
		return foundInd;
	}

	bool stringsAreEqual(const char* str1, const char* str2) {
		if (str1 == nullptr || str2 == nullptr) {
			return false;
		}

		while (*str1 && *str2) {
			if (*str1 != *str2) {
				return false;
			}
			str1++;
			str2++;
		}

		return *str1 == *str2;
	}
}

size_t* MarkdownTable::calculateColumnWidths() const{
	size_t* columnWidths = new size_t[numberOfColumns];
	for (int i = 0; i < numberOfColumns; i++) {
		columnWidths[i] = getLongestValueLength(i);
	}
	return columnWidths;
}

void MarkdownTable::print() const {
	size_t* columnWidths = calculateColumnWidths();

	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < numberOfColumns; j++) {
			rows[i].printValue(alignments[j], columnWidths[j], j);
		}
	}

	delete[] columnWidths;
}

void MarkdownTable::selectPrint(const char* value, size_t columnIndex) const {
	size_t* columnWidths = calculateColumnWidths();

	//Print all rows which have a given value on a given column 
	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < numberOfColumns; j++) {
			if (stringsAreEqual(value, rows[i].getValueAtIndex(j).getValue())) {
				rows[i].printValue(alignments[j], columnWidths[j], j);
			}
		}
	}

	delete[] columnWidths;
}

void MarkdownTable::changeColumnName(const char* oldName, const char* newName) {
	size_t columnIndex = findColumnIndex(oldName, 0);
	rows[0].setValueAtIndex(newName, columnIndex);
}

void MarkdownTable::addRow(const TableRow& row) {
	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
		return;
	}
	rows[numberOfRows++] = row;
}

//void MarkdownTable::addRow(const char** row) {
//	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
//		return;
//	}
//	TableRow r(row);
//	rows[numberOfRows++];
//}

void MarkdownTable::changeValueAtIndex(size_t rowNumber, const char* columnName, const char* newValue) {
	size_t columnIndex = findColumnIndex(columnName, rowNumber);

	rows[rowNumber].setValueAtIndex(newValue, columnIndex);
}

void MarkdownTable::changeValue(const char* oldValue, const char* newValue, const char* columnName) {
	for (int j = 0; j < numberOfColumns; j++) {
		for (int i = 0; i < numberOfRows; i++) {
			if (stringsAreEqual(rows[i].getValueAtIndex(j).getValue(), oldValue)) {
				rows[i].setValueAtIndex(newValue, j);
				return;
			}
		}
	}
}

void MarkdownTable::saveToFile(const char* fileName) const {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		return;
	}

	size_t* columnWidths = calculateColumnWidths();

	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < numberOfColumns; j++) {
			rows[i].writeValueToStream(file, alignments[j], columnWidths[j], j);
		}
	}

	file.close();
	delete[] columnWidths;
}

void MarkdownTable::loadFromFile(const char* fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return;
	}

	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < numberOfColumns; j++) {
			rows[i].readValueFromStream(file);
		}
	}

	size_t* columnWidths = calculateColumnWidths();

	file.close();
	delete[] columnWidths;
}

size_t MarkdownTable::findColumnIndex(const char* columnName, const size_t rowInd) const{
	for (int j = 0; j < numberOfColumns; j++) {
		if (stringsAreEqual(rows[rowInd].getValueAtIndex(j).getValue(), columnName)) {
			return j;
		}
	
	}
	return -1;
}

void MarkdownTable::initAlignments() {
	for (int i = 0; i < numberOfColumns; i++) {
		alignments[i] = Alignment::left;
	}
}



size_t MarkdownTable::getLongestValueLength(size_t columnIndex) const{
	if (numberOfRows == 0) {
		return 0;
	}

	size_t maxLen = myStrlen(this->getRowAtIndex(0).getValueAtIndex(columnIndex).getValue());
	for (int i = 1; i < numberOfRows; i++) {
		size_t temp = myStrlen(getRowAtIndex(i).getValueAtIndex(columnIndex).getValue());
		if (temp > maxLen) {
			maxLen = temp;
		}
	}
	return maxLen;
}

const TableRow& MarkdownTable::getRowAtIndex(size_t index) const{
	return rows[index];
}

const Alignment MarkdownTable::identifyAlignment(size_t columnIndex) const{
	// A hyphen is the symbol '-'
	
	const char* ptr = rows[1].getValueAtIndex(columnIndex).getValue();

	int foundInd = findSymbolInString(ptr, COLON);

	// No ':' symbols found. Default alignment 
	if (foundInd == -1) {
		return Alignment::left;
	}

	bool left = false;

	// :---
	if (foundInd == 0 || ptr[foundInd + 1] == HYPHEN) {
		left = true;
	}

	// Search for the next ':' symbol
	ptr += foundInd;
	foundInd = findSymbolInString(ptr, COLON);

	//No central alignment 
	if (foundInd == -1) {
		return Alignment::left;
	}

	else if (!left) {
		return Alignment::right;
	}
	
	else {
		return Alignment::center;
	}

}