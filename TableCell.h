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

	bool setValue(const char* value);
	const char* getValue() const;
	size_t getValueLength() const;

	bool writeCellToStream(std::ostream& os, const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const;
	bool printValue(const Alignment& alignment, size_t width, size_t index, size_t numberOfCells) const;
};