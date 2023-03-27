#pragma once

const char DEFAULT_VAL[] = "Unknown";
const int MAX_NUMBER_OF_SYMBOLS = 20;

enum class Alignment {
	center,
	left,
	right
};

const char HYPHEN = '-';
const char PIPE = '|';
const char COLON = ':';
const char SPACE = ' ';

class TableCell {
private:
	char value[MAX_NUMBER_OF_SYMBOLS + 1];
public:
	TableCell();
	TableCell(const char* value);

	~TableCell();

	void setValue(const char* value);
	const char* getValue() const;
	size_t getValueLength() const;

	void writeCellToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const;
	void printValue(const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const;
};