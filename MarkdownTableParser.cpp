#include <iostream>

#include "MarkdownTableParser.h"

#include "Commands.h"
#include "Messages.h"

namespace {
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

	// This function returns the number of times a string contains a character. 
	size_t countCharOccurances(const char* str, char ch) {
		if (str == nullptr) {
			return 0;
		}

		int ctr = 0;
		while (*str) {
			if (*str == ch) {
				ctr++;
			}
			str++;
		}

		return ctr;
	}
}

MarkdownTableParser::MarkdownTableParser(const MarkdownTable& table) : table(table) {

}

const int MAX_FILENAME_LEN = 150;

bool MarkdownTableParser::load() {
	char fileName[MAX_FILENAME_LEN + 1];
	std::cin.getline(fileName, MAX_FILENAME_LEN + 1);

	bool loadedSuccessfully = table.loadFromFile(fileName);

	if (!loadedSuccessfully) {
		std::cout << ERROR_MSG << "\n";
		return false;
	}

	else {
		std::cout << LOAD_SUCCESS << "\n";
		return true;
	}
}

const int MAX_INPUT_SIZE = 150;

void MarkdownTableParser::interface() {
	if (!load()) {
		return;
	}
	
	//The loop will be exited after the table is saved to a file.
	bool isSaved = false;

	while (!isSaved) {
		std::cout << ENTER_COMMAND_MSG << "\n";
		char buff[MAX_INPUT_SIZE + 1];
		std::cin >> buff;
		std::cin.ignore();

		int i = 0;
		for (; i < NUMBER_OF_COMMANDS; i++) {
			if (stringsAreEqual(ALL_COMMANDS[i], buff)) {
				switch (i) {
				case 0: print(); break;
				case 1: change(); break;
				case 2: changeRow(); break;
				case 3: addRow(); break;
				case 4: selectPrint(); break;
				case 5: isSaved = save(); break;
				default: std::cout << INVALID_OPERATION_MSG << "\n";
				}
				break;
			}
		}
		if (i >= NUMBER_OF_COMMANDS) {
			std::cout << INVALID_OPERATION_MSG << "\n";
		}
	}
}

void MarkdownTableParser::change() {
	char columnName[MAX_NUMBER_OF_SYMBOLS + 1];
	std::cin.getline(columnName, MAX_NUMBER_OF_SYMBOLS + 1, ' ');

	// There are two possible operations which can be performed with the 
	// command change: changing a name of a column and changing the first
	// matching value in a column. 

	char buff[MAX_INPUT_SIZE + 1];
	std::cin.getline(buff, MAX_INPUT_SIZE + 1);

	int numberOfSpaces = countCharOccurances(buff, ' ');
	bool operationWasSuccessful = false;

	// Change the name of a column
	if (numberOfSpaces == 0) {
		operationWasSuccessful = table.changeColumnName(columnName, buff);
	}

	// Change the first matching value in a column with a given value
	else if (numberOfSpaces == 1){
		char* ptr = buff;
		size_t spaceFoundPos = 0;

		for (; *(ptr) != SPACE; spaceFoundPos++, ptr++);
		// Write over the found space 
		*ptr = '\0';

		operationWasSuccessful = table.changeCell(ptr - spaceFoundPos, ptr + 1, columnName);
	}

	if (operationWasSuccessful) {
		std::cout << OPERATION_SUCCESS << "\n";
	}

	else {
		std::cout << ERROR_MSG << "\n";
	}
}

void MarkdownTableParser::changeRow() {
	size_t rowNumber;
	std::cin >> rowNumber;

	char columnName[MAX_INPUT_SIZE + 1];
	char newValue[MAX_INPUT_SIZE + 1];

	std::cin >> columnName >> newValue;
	std::cin.ignore();

	if (table.changeRow(rowNumber, columnName, newValue)) {
		std::cout << OPERATION_SUCCESS << "\n";
	}

	else {
		std::cout << ERROR_MSG << "\n";
	}
}

void MarkdownTableParser::addRow() {
	size_t numberOfCells = table.getNumberOfColumns(); 
	size_t numberOfSymbols = (MAX_NUMBER_OF_SYMBOLS + 1) * numberOfCells;

	char* buff = new char[numberOfSymbols];
	std::cin.getline(buff, numberOfSymbols);
	TableRow r(buff);

	if (table.addRow(r)) {
		std::cout << OPERATION_SUCCESS << "\n";
	}

	else {
		std::cout << ERROR_MSG << "\n";
	}

	delete[] buff;
}

void MarkdownTableParser::selectPrint() const{
	char columnName[MAX_NUMBER_OF_SYMBOLS + 1];
	std::cin.getline(columnName, MAX_NUMBER_OF_SYMBOLS + 1, ' ');

	char value[MAX_NUMBER_OF_SYMBOLS + 1];
	std::cin.getline(value, MAX_NUMBER_OF_SYMBOLS + 1, '\n');

	table.selectPrint(value, columnName);
}

bool MarkdownTableParser::save() const{
	char fileName[MAX_FILENAME_LEN + 1];
	std::cin.getline(fileName, MAX_FILENAME_LEN + 1);
	return table.saveToFile(fileName);
}

void MarkdownTableParser::print() const {
	table.print();
}