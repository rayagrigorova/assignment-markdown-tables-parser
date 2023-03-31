#include <iostream>
#include <fstream>
#include <sstream>

#include "TableRow.h"

namespace {
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

		file.clear();
		file.seekg(currentPos);
		return ctr;
	}
}

TableRow::TableRow(const TableCell* cells, size_t numberOfCells) {
	setNumberOfCells(numberOfCells);
	setCells(cells, this->numberOfCells);
}

TableRow::TableRow(const char* cells) {
	setCells(cells);
}

void TableRow::setNumberOfCells(size_t numberOfCells) {
	if (numberOfCells > MAX_NUMBER_OF_COLS) {
		this->numberOfCells = 0;
		return;
	}
	this->numberOfCells = numberOfCells;
}

void TableRow::setCells(const char* cells) {
	size_t count = countCharacterOccurances(cells, ' ') + 1;

	setNumberOfCells(count);
	if (this->numberOfCells == 0) {
		return;
	}

	std::stringstream ss(cells);
	char buff[MAX_NUMBER_OF_SYMBOLS + 1];

	for (int i = 0; i < this->numberOfCells - 1; i++) {
		// Get each of the cells from the string stream
		ss.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, ' ');
		this->cells[i].setValue(buff);
	}

	ss.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1);
	this->cells[numberOfCells - 1].setValue(buff);
}

void TableRow::setCells(const TableCell* cells, size_t count) {
	setNumberOfCells(count);

	for (int i = 0; i < this->numberOfCells; i++) {
		setCellAtIndex(cells[i], i);
	}
}

size_t TableRow::getNumberOfCells() const {
	return numberOfCells;
}

const TableCell* TableRow::getCells() const {
	return cells;
}

bool TableRow::setCellAtIndex(const TableCell& cell, size_t index) {
	if (index < 0 || index >= numberOfCells) {
		return false;
	}
	return cells[index].setValue(cell.getValue());
}

bool TableRow::setCellAtIndex(const char* cell, size_t index) {
	if (index >= numberOfCells) {
		return false;
	}
	return cells[index].setValue(cell);
}

const TableCell& TableRow::getCellAtIndex(size_t index) const{
	if (index >= numberOfCells) {
		return nullptr;
	}
	return cells[index];
}

bool TableRow::readCellsFromStream(std::ifstream& ifs){
	if (!ifs.is_open()) {
		return false;
	}

	// Each line in the file is one row in the table
	size_t numberOfValuesInLine = countCharacterOccurancesInFileLine(ifs, PIPE) - 1;

	setNumberOfCells(numberOfValuesInLine);

	char buff[MAX_NUMBER_OF_SYMBOLS + 1];

	//Skip the first '|' symbol of the file
	ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, PIPE);

	for (int i = 0; !ifs.eof() && i < this->numberOfCells; i++) {
		//Get all symbols up to '|'
		ifs >> buff;
		//ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, PIPE);

		cells[i].setValue(buff);
		ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, PIPE);
	}
	
	//If the end of the file hasn't been reached, go to the next line 
	//so that the next row can be read
	if (!ifs.eof()) {
		ifs.getline(buff, MAX_NUMBER_OF_SYMBOLS + 1, '\n');
	}

	return true;
}

bool TableRow::writeCellsToStream(std::ostream& os, const Alignment* alignments, const size_t* widths) const{
	for (int j = 0; j < numberOfCells; j++) {
		if (!cells[j].writeCellToStream(os, alignments[j], widths[j], j, numberOfCells)) {
			return false;
		}
	}
	return true;
}

bool TableRow::printCells(const Alignment* alignments, const size_t* widths) const {
	return writeCellsToStream(std::cout, alignments, widths);
}

