#pragma once

#include "TableCell.h"

//The number of columns in a row shouldn't exceed 10 
const int MAX_NUMBER_OF_COLS = 10;

class TableRow {
private:
	TableCell cells[MAX_NUMBER_OF_COLS];
	size_t numberOfCells; // In the table, this is equal to the number of columns a row has

public:
	TableRow();
	TableRow(const TableCell* cells, size_t numberOfCells);
	TableRow(const char* cells); /* This constructor may be easier to use because it
													         does't require creating a TableValue array*/
	~TableRow();

	void setNumberOfCells(size_t numberOfCells);
	void setCells(const TableCell* cells, size_t numberOfCells);
	void setCells(const char* cells);

	bool setCellAtIndex(const TableCell& value, size_t index);
	bool setCellAtIndex(const char* value, size_t index);

	const TableCell* getCells() const;
	size_t getNumberOfCells() const;

	const TableCell& getCellAtIndex(size_t index) const;

	bool readCellsFromStream(std::ifstream& ifs);

	bool writeCellsToStream(std::ostream& os, const Alignment* alignments, const size_t* widths) const;
	bool printCells(const Alignment* alignments, const size_t* widths) const;
};
