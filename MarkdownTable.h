#pragma once

#include "TableRow.h"

const int MAX_NUMBER_OF_ROWS = 50;

class MarkdownTable {
private:
	TableRow columnNames;
	// -1 because of the row with column names
	TableRow rows[MAX_NUMBER_OF_ROWS - 1];
	size_t numberOfRows; 

	//A helper function
	size_t findColumnIndex(const char* columnName);
public:
	MarkdownTable();
	MarkdownTable(const TableRow& columnNames, const TableRow* rows, size_t numberOfRows);
	MarkdownTable(const char** columnNames, const char** rows, size_t numberOfRows);

	~MarkdownTable();

	const TableRow& getColumnNames() const;
	const TableRow* getRows() const;
	const size_t getNumberOfRows() const;

	void setColumnNames(TableRow columnNames);
	void setColumnNames(const char** columnNames);
	void setRows(const TableRow* rows);
	void setRows(const char** rows);
	void setNumberOfRows(size_t numberOfRows);

	void print() const;
	void changeColumnName(const char* oldName, const char* newName);
	void addRow(const TableRow& row);
	void addRow(const char** row);
	void changeValueAtIndex(size_t rowNumber, const char* columnName, const char* newValue);
	void changeValue(const char* oldValue, const char* newValue, const char* columnName);
};