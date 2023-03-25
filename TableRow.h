#pragma once

#include "TableValue.h"

//The number of values in a row shouldn't exceed 10 
const int MAX_NUMBER_OF_COLS = 10;

class TableRow {
private:
	TableValue values[MAX_NUMBER_OF_COLS];
	size_t numberOfValues;
public:
	TableRow();
	TableRow(const TableValue* values, size_t numberOfValues);
	TableRow(const char** values, size_t numberOfValues);

	~TableRow();

	void setNumberOfValues(size_t numberOfValues);
	void setValues(const char** values, size_t count);
	void setValues(const TableValue* values, size_t count);

	const TableValue* getValues() const;
	size_t getNumberOfValues() const;

	void print() const;
};
