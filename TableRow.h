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
	void setValues(const char* values, size_t count);
	void setValues(const TableValue* values, size_t count);

	void setValueAtIndex(const TableValue& value, size_t index);
	void setValueAtIndex(const char* value, size_t index);

	const TableValue* getValues() const;
	size_t getNumberOfValues() const;
	const TableValue& getValueAtIndex(size_t index) const;

	//void print() const;
	void writeValueToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index) const;
	void printValue(const Alignment& alignment, size_t width, size_t index) const;

	void readValueFromStream(std::ifstream& ifs);
};
