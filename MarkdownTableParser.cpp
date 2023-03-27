#include <iostream>

#include "MarkdownTableParser.h"

const char* OPERATION_SUCCESS = "Operation successfully executed!";
const char* LOAD_SUCCESS = "Table loaded successfully!";
const char* SAVE_SUCCESS = "File successfully saved!";
const char* ERROR_MSG = "Operation failed.";
const char* ENTER_COMMAND_MSG = "Enter a command.";
const char* INVALID_OPERATION_MSG = "Invalid operation.";

const int NUMBER_OF_COMMANDS = 6;

const char* PRINT_COMMAND = "print";
const char* CHANGE_COMMAND = "change";
const char* CHANGE_ROW_COMMAND = "changeRow";
const char* ADD_ROW_COMMAND = "addRow";
const char* SELECT_COMMAND = "selectWhere";
const char* SAVE_COMMAND = "save";

const char* ALL_COMMANDS[] = { PRINT_COMMAND,
CHANGE_COMMAND, CHANGE_ROW_COMMAND,
ADD_ROW_COMMAND, SELECT_COMMAND, SAVE_COMMAND };


MarkdownTableParser::MarkdownTableParser() : table(){

}
MarkdownTableParser::MarkdownTableParser(const MarkdownTable& table) : table(table.getRows(), table.getNumberOfRows()) {

}

MarkdownTableParser::~MarkdownTableParser() {

}

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
	int countCharOccurances(const char* str, char ch) {
		if (str == nullptr) {
			return -1;
		}

		int ctr = 0;
		while (*str) {
			if (*str == ch) {
				ctr++;
			}
		}

		return ctr;
	}
}

const int MAX_FILENAME_LEN = 150;

void MarkdownTableParser::load() {
	char fileName[MAX_FILENAME_LEN + 1];
	std::cin.getline(fileName, MAX_FILENAME_LEN + 1);

	bool loadedSuccessfully = table.loadFromFile(fileName);

	if (!loadedSuccessfully) {
		std::cout << ERROR_MSG << "\n";
	}

	else {
		std::cout << LOAD_SUCCESS << "\n";
	}
}

const int MAX_INPUT_SIZE = 150;

void MarkdownTableParser::interface() {
	load();
	
	//This loop will be exited after the table is saved to a file.
	bool isSaved = false;

	while (!isSaved) {
		std::cout << ENTER_COMMAND_MSG << "\n";
		char buff[MAX_INPUT_SIZE + 1];
		std::cin.getline(buff, MAX_INPUT_SIZE + 1, ' ');

		for (int i = 0; i < NUMBER_OF_COMMANDS; i++) {
			if (stringsAreEqual(ALL_COMMANDS[i], buff)) {
				switch (i) {
				case 0:
					print();
					break;
				case 1:
					change();
					break;
				case 2:
					changeRow();
					break;
				case 3:
					addRow();
					break;
				case 4:
					selectPrint();
					break;
				case 5:
					isSaved = save();
					break;
				default:
					std::cout << INVALID_OPERATION_MSG << "\n";
				}
			}
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

	// Change the name of a column
	if (numberOfSpaces == 0) {
		table.changeColumnName(columnName, buff);
		return;
	}

	// Change the first matching value in a column with a given value
	else if (numberOfSpaces == 2){
		char* ptr = buff;
		size_t pos = 0;

		for (; *(ptr++) != ' '; pos++);

		*ptr = '\0';
		table.changeCell(ptr, ptr + pos + 1, columnName);
		return;
	}

	else {
		std::cout << ERROR_MSG << "\n";
	}
}

void MarkdownTableParser::changeRow() {
	size_t rowNumber;
	std::cin >> rowNumber;

	char columnName[MAX_NUMBER_OF_SYMBOLS + 1];
	char newValue[MAX_NUMBER_OF_SYMBOLS + 1];

	std::cin >> columnName >> newValue;
	std::cin.ignore();

	table.changeRow(rowNumber, columnName, newValue);
}

void MarkdownTableParser::addRow() {
	// The number of cells in the row is equal to the
	// number of columns in the table
	size_t numberOfCells = table.getNumberOfColumns(); 

	char* buff = new char[(MAX_NUMBER_OF_SYMBOLS + 1) * numberOfCells];
	TableRow r(buff);
	table.addRow(r);

	delete[] buff;
}

void MarkdownTableParser::selectPrint() const{
	char columnName[MAX_NUMBER_OF_SYMBOLS + 1];
	std::cin.getline(columnName, MAX_NUMBER_OF_SYMBOLS + 1, ' ');

	char value[MAX_NUMBER_OF_SYMBOLS + 1];
	std::cin.getline(value, MAX_NUMBER_OF_SYMBOLS + 1, ' ');

	table.selectPrint(value, columnName);
}

bool MarkdownTableParser::save() const{
	char fileName[MAX_FILENAME_LEN + 1];
	table.saveToFile(fileName);
}

void MarkdownTableParser::print() const {
	table.print();
}