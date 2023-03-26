#include <iostream>
#include <fstream>

#include <iomanip>
#pragma warning (disable : 4996)

#include "MarkdownTable.h"

int main()
{
	//TableValue val1("Someval");

	//std::cout << val1.getValue() << "\n";
	//val1.setValue("123456");
	//std::cout << val1.getValue() << "\n";

	Alignment alignments[10];
	TableRow rows[4];
	size_t widths[10];

	//std::ofstream file("file.txt");
	//row.writeValuesToStream(file, alignments, widths);

	std::ifstream file("file2.txt");

	for(int i = 0; i < 4; i++)
		rows[i].readValuesFromStream(file);


	for (int i = 0; i < rows[0].getNumberOfValues(); i++) {
		alignments[i] = Alignment::center;
		widths[i] = 18;
	}

	for (int i = 0; i < 4; i++)
		rows[i].printValues(alignments, widths, '-');
}
