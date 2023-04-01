#include <iostream>
#include <fstream>
#include <sstream>

#include "MarkdownTable.h"

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
		// Save position
		size_t currentPos = file.tellg();
		// Move the get pointer to the beginning of the file
		file.seekg(0, std::ios::beg);

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

		file.clear();
		file.seekg(currentPos, std::ios::beg);
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

MarkdownTable::MarkdownTable(){
	for (int i = 0; i < numberOfColumns; i++) {
		alignments[i] = Alignment::left;
	}
}

MarkdownTable::MarkdownTable(const TableRow* rows, size_t numberOfRows) {
	setRows(rows, numberOfRows);
}

MarkdownTable::MarkdownTable(const char** rows, size_t numberOfRows) {
	setRows(rows, numberOfRows);
}

MarkdownTable::MarkdownTable(const char* fileName) {
	loadFromFile(fileName);
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
	// If the number of columns in the table doesn't match the number of 
	// columns of the given row, return. 
	if (columnNames.getNumberOfCells() != this->numberOfColumns) {
		return;
	}

	// The table row containing the column names is rows[0]
	for (int i = 0; i < this->numberOfColumns; i++) {
		this->rows[0].setCellAtIndex(columnNames.getCellAtIndex(i), i);
	}

	initAlignments();
}

void MarkdownTable::setColumnNames(const char* columnNames) {
	// Create new row from the column names 
	TableRow r(columnNames);

	if (r.getNumberOfCells() != this->numberOfColumns) {
		return;
	}

	for (int i = 0; i < this->numberOfColumns; i++) {
		this->rows[0].setCellAtIndex(r.getCellAtIndex(i), i);
	}

	initAlignments();
}

void MarkdownTable::setRows(const TableRow* rows, size_t numberOfRows) {
	if (!rowsAreValid(rows, numberOfRows)) {
		return;
	}

	setNumberOfRows(numberOfRows);
	setNumberOfColumns(rows[0].getNumberOfCells());

	for (int i = 0; i < this->numberOfRows; i++) {
		this->rows[i] = rows[i];
	}

	initAlignments();
}

void MarkdownTable::setRows(const char** rows, size_t numberOfRows) {
	// Create an array of table rows using the TableRow(char* ) constructor 
	TableRow* arr = new TableRow[numberOfRows];
	for (int i = 0; i < numberOfRows; i++) {
		arr[i] = TableRow(rows[i]);
	}

	if (!rowsAreValid(arr, numberOfRows)) {
		delete[] arr;
		return;
	}

	setNumberOfRows(numberOfRows);
	setNumberOfColumns(arr[0].getNumberOfCells());

	for (int i = 0; i < numberOfRows; i++) {
		this->rows[i] = arr[i];
	}

	initAlignments();
	delete[] arr;
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

size_t* MarkdownTable::calculateColumnWidths() const{
	size_t* columnWidths = new size_t[numberOfColumns];

	for (int i = 0; i < numberOfColumns; i++) {
		columnWidths[i] = getLongestValueLength(i);
	}

	return columnWidths;
}

void MarkdownTable::print() const{
	size_t* columnWidths = calculateColumnWidths();

	rows[0].printCells(alignments, columnWidths);
	std::cout << '\n';
	printSecondRow(columnWidths);
	std::cout << '\n';

	for (int i = 2; i < numberOfRows; i++) {
		rows[i].printCells(alignments, columnWidths);
		std::cout << '\n';
	}
	delete[] columnWidths;
}

void MarkdownTable::selectPrint(const char* value, const char* columnName) const {
	size_t* columnWidths = calculateColumnWidths();
	size_t columnIndex = findColumnIndex(columnName, 0);

	//Print all rows which have a given value on a given column 
	for (int i = 0; i < numberOfRows; i++) {
		if (stringsAreEqual(value, rows[i].getCellAtIndex(columnIndex).getValue())) {
			rows[i].printCells(alignments, columnWidths);
			std::cout << "\n";
		}
	}

	delete[] columnWidths;
}

bool MarkdownTable::changeColumnName(const char* oldName, const char* newName) {
	// The row containing the column names is rows[0]
	int columnIndex = findColumnIndex(oldName, 0);

	// Not found
	if (columnIndex < 0) {
		return false;
	}

	return rows[0].setCellAtIndex(newName, columnIndex);
}

bool MarkdownTable::addRow(const TableRow& row) {
	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
		return false;
	}
	rows[numberOfRows++] = row;
	return true;
}

bool MarkdownTable::addRow(const char* row) {
	if (numberOfRows >= MAX_NUMBER_OF_ROWS) {
		return false;
	}

	TableRow r(row);
	if (r.getNumberOfCells() == numberOfColumns) {
		rows[numberOfRows++] = r;
		return true;
	}

	return false;
}

bool MarkdownTable::changeRow(size_t rowNumber, const char* columnName, const char* newValue) {
	// The first 2 rows of the table don't contain values
	rowNumber++;

	int columnIndex = findColumnIndex(columnName, 0);
	if (columnIndex < 0) {
		return false;
	}

	return rows[rowNumber].setCellAtIndex(newValue, columnIndex);
}

bool MarkdownTable::changeCell(const char* oldValue, const char* newValue, const char* columnName) {
	// First, find the index of the column which corresponds to the columnName
	int columnInd = findColumnIndex(columnName, 0);

	if (columnInd < 0) {
		return false;
	}

	for (int i = 0; i < numberOfRows; i++) {
		if (stringsAreEqual(rows[i].getCellAtIndex(columnInd).getValue(), oldValue)) {
			return rows[i].setCellAtIndex(newValue, columnInd);
			break;
		}
	}
	return false;
}

bool MarkdownTable::saveToFile(const char* fileName) const {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		return false;
	}

	size_t* columnWidths = calculateColumnWidths();

	for (int i = 0; i < numberOfRows; i++) {
		if (!rows[i].writeCellsToStream(file, alignments, columnWidths)) {
			file.close();
			delete[] columnWidths;
			return false;
		}
		if (i != numberOfRows - 1) {
			file << "\n";
		}
	}

	file.close();
	delete[] columnWidths;
	return true;
}

bool MarkdownTable::loadFromFile(const char* fileName) {
	std::ifstream file(fileName);

	if (!file.is_open()) {
		return false;
	}

	size_t numberOfRows = countCharacterOccurancesInFile(file, '\n') + 1;

	setNumberOfRows(numberOfRows);

	for (size_t i = 0; i < this->numberOfRows; i++) {
		if (!rows[i].readCellsFromStream(file)) {
			file.close();
			return false;
		}
	}
	
	if (!rowsAreValid(this->rows, numberOfRows)) {
		file.close();
		return false;
	}

	setNumberOfColumns(rows[0].getNumberOfCells());

	initAlignments();
	file.close();
	return true;
}

// Find the column index of a cell on a given row using the cell's value
int MarkdownTable::findColumnIndex(const char* value, const size_t rowInd) const{
	for (int j = 0; j < numberOfColumns; j++) {
		if (stringsAreEqual(rows[rowInd].getCellAtIndex(j).getValue(), value)) {
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

// Get the length of the longest value in a column to determine the column width
size_t MarkdownTable::getLongestValueLength(size_t columnIndex) const{
	if (numberOfRows == 0) {
		return 0;
	}

	size_t maxLen = myStrlen(rows[0].getCellAtIndex(columnIndex).getValue());

	for (int i = 1; i < numberOfRows; i++) {
		size_t temp = myStrlen(rows[i].getCellAtIndex(columnIndex).getValue());
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
	// rows[1] is the row containing information about alignment 
	const char* str = rows[1].getCellAtIndex(columnIndex).getValue();

	int foundInd = findSymbolInString(str, COLON);

	// No ':' symbols found. Default alignment 
	if (foundInd == -1) {
		return Alignment::left;
	}

	bool hasLeftAlignment = false;

	// :---
	if (str[foundInd + 1] == HYPHEN) {
		hasLeftAlignment = true;
	}

	// Search for the next ':' symbol
	str += foundInd;

	// If the '\0' character hasn't been reached, go past the found position so that ':' isn't read again
	if (*(str)) {
		str++;
	}

	foundInd = findSymbolInString(str, COLON);

	// A second ':' symbol wasn't found. 
	if (foundInd < 0) {
		if (!hasLeftAlignment) {
			return Alignment::right;
		}
		else {
			return Alignment::left;
		}
	}

	return Alignment::center;
}

bool rowsAreValid(const TableRow* rows, size_t numberOfRows) {
	if (numberOfRows == 0 || rows == nullptr) {
		return false;
	}

	size_t numberOfCellsInFirstRow = rows[0].getNumberOfCells();

	// Compare the number of cells in the first row to all other 
	for (int i = 1; i < numberOfRows; i++) {
		if (rows[i].getNumberOfCells() != numberOfCellsInFirstRow) {
			return false;
		}
	}
	return true;
}

void MarkdownTable::printSecondRow(const size_t* widths) const{
	if (numberOfRows <= 1) {
		return;
	}

	for (int j = 0; j < numberOfColumns; j++) {
		std::cout << SPACE << PIPE << SPACE;
		for (int i = 0; i < widths[j]; i++) {
			std::cout << HYPHEN;
		}
	}
	std::cout << SPACE << PIPE;
}