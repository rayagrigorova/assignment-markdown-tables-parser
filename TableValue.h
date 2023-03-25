#pragma once

const char DEFAULT_VAL[] = "Unknown";
const int MAX_NUMBER_OF_SYMBOLS = 20;

class TableValue {
private:
	char value[MAX_NUMBER_OF_SYMBOLS + 1];
public:
	TableValue();
	TableValue(const char* value);

	~TableValue();

	void setValue(const char* value);
	const char* getValue() const;
};