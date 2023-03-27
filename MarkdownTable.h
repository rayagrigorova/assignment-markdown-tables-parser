#pragma once

#include "TableRow.h"

const int MAX_NUMBER_OF_ROWS = 50;

class MarkdownTable {
private:
	TableRow rows[MAX_NUMBER_OF_ROWS];

	size_t numberOfRows = 0;  // The actual number of rows and columns in the table
	size_t numberOfColumns = 0;

	// Each column has an alignment 
	Alignment alignments[MAX_NUMBER_OF_COLS];

	// A helper function
	// Find the column of a cell using its value and row index
	int findColumnIndex(const char* value, const size_t rowInd) const;

	// Using the second row of the table, determine the alignments of all columns 
	void initAlignments();

	// Needed for printing - the longest value in a column 
	// determines the width of the column. 
	size_t getLongestValueLength(size_t columnIndex) const; 

	// Find the alignment of a given column (the row containing information about alignment is rows[1])
	// This function is used in initAlignments(). 
	const Alignment identifyAlignment(size_t columnIndex) const;

	// Create an array containg the widths of the table columns
	// Used in printing/writing functions.
	size_t* calculateColumnWidths() const;

	// A seperate function to print the second row. 
	// The second row seperates the column names from the rest of the table.
	void printSecondRow(const size_t* widths) const;

public:
	MarkdownTable();
	MarkdownTable(const TableRow* rows, size_t numberOfRows);
	MarkdownTable(const char** rows, size_t numberOfRows);
	MarkdownTable(const char* fileName);

	~MarkdownTable();

	const TableRow* getRows() const;
	const size_t getNumberOfRows() const;
	const size_t getNumberOfColumns() const;
	const TableRow& getRowAtIndex(size_t index) const;

	// Set the first row of the table
	void setColumnNames(TableRow columnNames);
	void setColumnNames(const char* columnNames);

	void setRows(const TableRow* rows, size_t numberOfRows);
	void setRows(const char** rows, size_t numberOfRows);
	void setNumberOfRows(size_t numberOfRows);
	void setNumberOfColumns(size_t numberOfColumns);

	void print() const;
	void selectPrint(const char* value, const char* columnName) const;

	void changeColumnName(const char* oldName, const char* newName);

	void addRow(const TableRow& row);
	void addRow(const char* row);

	void changeCellAtIndex(size_t rowNumber, const char* columnName, const char* newValue);
	void changeCell(const char* oldValue, const char* newValue, const char* columnName);

	void saveToFile(const char* fileName) const;
	void loadFromFile(const char* fileName);
};

// All rows must have the same number of values in them
bool rowsAreValid(const TableRow* rows, size_t numberOfRows);