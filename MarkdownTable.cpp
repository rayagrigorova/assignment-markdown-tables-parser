#include <iostream>
#include <fstream>
#include <sstream>

#include "MarkdownTable.h"

MarkdownTable::MarkdownTable() : rows(){
	numberOfRows = 0;
	numberOfColumns = 0;

	for (int i = 0; i < numberOfColumns; i++) {
		alignments[i] = Alignment::left;
	}
}

MarkdownTable::MarkdownTable(const TableRow* rows, size_t numberOfRows, size_t numberOfColumns) {
	if (!rowsAreValid(rows, numberOfRows)) {
		return;
	}
	setNumberOfRows(numberOfRows);
	setRows(rows, this->numberOfRows);
	setNumberOfColumns(numberOfColumns);
	initAlignments();

}

MarkdownTable::MarkdownTable(const char** rows, size_t numberOfRows, size_t numberOfColumns) {
	//// Create rows from the 2D char array and use the other parametarized constructor
	//TableRow* arr = new TableRow[numberOfRows];
	//for (int i = 0; i < numberOfRows; i++) {
	//	arr[i] = TableRow(rows[i]);
	//}

	//if (!rowsAreValid(arr, numberOfRows)) {
	//	return;
	//}

	MarkdownTable(arr, numberOfRows, numberOfColumns);

	delete[] arr;
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
	size_t numberOfCols = columnNames.getNumberOfValues();

	// If the number of columns in the table doesn't match the number of 
	// columns of the given value, return. 
	if (numberOfCols != this->numberOfColumns) {
		return;
	}

	// The table row containing the column names is rows[0]
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
	if (!rowsAreValid(rows, numberOfRows)) {
		return;
	}

	setNumberOfRows(numberOfRows);
	setNumberOfColumns(rows[0].getNumberOfValues());

	for (int i = 0; i < this->numberOfRows; i++) {
		this->rows[i].setValues(rows[i].getValues(), this->numberOfColumns);
	}
}

void MarkdownTable::setRows(const char** rows, size_t numberOfRows) {
	TableRow* arr = new TableRow[numberOfRows];
	for (int i = 0; i < numberOfRows; i++) {
		arr[i] = TableRow(rows[i]);
	}

	if (!rowsAreValid(arr, numberOfRows)) {
		return;
	}

	setNumberOfRows(numberOfRows);

	// Each row is a one-dimentional char array. The end of a row is marked by a '\n' symbol and 
	// the values are seperated by the ' ' symbol. 

	//Assuming the the rows are valid (meaning that they have an equal number of columns)
	size_t numberOfCols = countCharacterOccurancesInString(rows[0], ' ') + 1;
	setNumberOfColumns(numberOfCols);

	if (this->numberOfRows == 0 || this->numberOfColumns == 0) {
		return;
	}

	const size_t BUFF_SIZE = (MAX_NUMBER_OF_SYMBOLS + 1) * numberOfCols;
	char* buff = new char[BUFF_SIZE];

	for (int i = 0; i < this->numberOfRows; i++) {
		std::stringstream ss(rows[i]);

		for (int j = 0; j < this->numberOfColumns - 1; j++) {
			ss.getline(buff, BUFF_SIZE, ' ');
			this->rows[i].setValueAtIndex(buff, j);
		}
		
		// Save the last value of the row which isn't seperated by the symbol ' ' 
		ss.getline(buff, BUFF_SIZE);
		this->rows[i].setValueAtIndex(buff, this->numberOfRows - 1);
		
	}

	delete[] buff;
}

void MarkdownTable::setNumberOfRows(size_t numberOfRows) {
	if (numberOfRows <= MAX_NUMBER_OF_ROWS) {
		this->numberOfRows = numberOfRows;
	}
}

void MarkdownTable::setNumberOfColumns(size_t numberOfColumns) {
	if (numberOfColumns <= MAX_NUMBER_OF_COLS) {
		this->numberOfColumns = numberOfColumns;
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

	size_t countCharacterOccurancesInFile(std::ifstream& file, char ch) {
		size_t currentPos = file.tellg();

		char current;
		size_t ctr = 0;

		while (1) {
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

	size_t countCharacterOccurancesInString(const char* str, char ch) {
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
		if (stringsAreEqual(value, rows[i].getValueAtIndex(columnIndex).getValue())) {
			rows[i].printValue(alignments[columnIndex], columnWidths[columnIndex], columnIndex);
		}
	}

	delete[] columnWidths;
}

void MarkdownTable::changeColumnName(const char* oldName, const char* newName) {
	// The row containing the column names is rows[0]
	size_t columnIndex = findColumnIndex(oldName, 0);

	rows[0].setValueAtIndex(newName, columnIndex);
}

void MarkdownTable::addRow(const TableRow& row) {
	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
		return;
	}
	rows[numberOfRows++] = row;
}

void MarkdownTable::addRow(const char* row) {
	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
		return;
	}
	TableRow r;
	r.setValues(row);
	rows[numberOfRows++] = r;
}

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

	size_t numberOfRows = countCharacterOccurancesInFile(file, '\n') + 1;

	setNumberOfRows(numberOfRows);

	for (size_t i = 0; i < this->numberOfRows; i++) {
		rows[i].readValuesFromStream(file);
	}

	setNumberOfColumns(rows[0].getNumberOfValues());

	file.close();
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
		alignments[i] = identifyAlignment(i);
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

bool rowsAreValid(const TableRow* rows, size_t numberOfRows) {
	size_t numberOfColumnsInFirstRow = rows[0].getNumberOfValues();

	// Compare the number of columns in the first row to all other 
	for (int i = 1; i < numberOfRows; i++) {
		if (rows[i].getNumberOfValues() != numberOfColumnsInFirstRow) {
			return false;
		}
	}

	return true;
}