#pragma once

#include "TableValue.h"

//The number of values in a row shouldn't exceed 10 
const int MAX_NUMBER_OF_COLS = 10;

class TableRow {
private:
	TableElement values[MAX_NUMBER_OF_COLS];
	size_t numberOfValues; // In the table, this is equal to the number of columns a row has

public:
	TableRow();
	TableRow(const TableElement* values, size_t numberOfValues);
	TableRow(const char* values); /* This constructor may be easier to use because it
													         does't require creating a TableValue array*/
	~TableRow();

	void setNumberOfValues(size_t numberOfValues);
	void setValues(const TableElement* values, size_t numberOfValues);
	void setValues(const char* values);

	void setValueAtIndex(const TableElement& value, size_t index);
	void setValueAtIndex(const char* value, size_t index);

	const TableElement* getValues() const;
	size_t getNumberOfValues() const;

	const TableElement& getValueAtIndex(size_t index) const;

	void readValuesFromStream(std::ifstream& ifs);
	void writeValuesToStream(std::ostream& os, const Alignment* alignments, const size_t* widths) const;
	void printValues(const Alignment* alignments, const size_t* widths) const;
};
