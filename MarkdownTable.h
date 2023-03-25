#pragma once

#include "TableRow.h"

const int MAX_NUMBER_OF_ROWS = 50;

class MarkdownTable {
private:
	TableRow rows[MAX_NUMBER_OF_ROWS];

	size_t numberOfRows; 
	size_t numberOfColumns;

	//Each column has an alignment 
	Alignment alignments[MAX_NUMBER_OF_COLS];

	//A helper function
	size_t findColumnIndex(const char* columnName, const size_t rowInd) const;

	//Initialize all alignments to the default value (left)
	void initAlignments();

	//Needed for printing 
	size_t getLongestValueLength(size_t columnIndex) const; 

	const Alignment identifyAlignment(size_t columnIndex) const;
	size_t* calculateColumnWidths() const;

public:
	MarkdownTable();
	MarkdownTable(const TableRow& columnNames, const TableRow* rows, size_t numberOfRows, size_t numberOfColumns);
	MarkdownTable(const char** columnNames, const char** rows, size_t numberOfRows, size_t numberOfColumns);
	MarkdownTable(const char* fileName);

	~MarkdownTable();

	const TableRow& getColumnNames() const;
	const TableRow* getRows() const;
	const size_t getNumberOfRows() const;
	const size_t getNumberOfColumns() const;
	const TableRow& getRowAtIndex(size_t index) const;

	void setColumnNames(TableRow columnNames);
	void setColumnNames(const char** columnNames, size_t numberOfCols);
	void setRows(const TableRow* rows, size_t numberOfRows);
	void setRows(const char** rows, size_t numberOfRows);
	void setNumberOfRows(size_t numberOfRows);
	void setNumberOfColumns(size_t numberOfColumns);

	void print() const;
	void selectPrint(const char* value, size_t columnIndex) const;

	void changeColumnName(const char* oldName, const char* newName);

	void addRow(const TableRow& row);
	//void addRow(const char** row);

	void changeValueAtIndex(size_t rowNumber, const char* columnName, const char* newValue);
	void changeValue(const char* oldValue, const char* newValue, const char* columnName);

	void saveToFile(const char* fileName) const;
	void loadFromFile(const char* fileName);
};