#include <iostream>
#include <fstream>

#include <iomanip>
#pragma warning (disable : 4996)

#include "MarkdownTable.h"

int main()
{

	//Alignment alignments[10];
	//TableRow rows[4];
	//size_t widths[10];

	//std::ifstream file("file2.txt");

	//for(int i = 0; i < 4; i++)
	//	rows[i].readValuesFromStream(file);
	//file.close();

	//for (int i = 0; i < rows[0].getNumberOfValues(); i++) {
	//	alignments[i] = Alignment::right;
	//	widths[i] = 18;
	//}

	//for (int i = 0; i < 4; i++)
	//	rows[i].printValues(alignments, widths, '-');

	//std::ofstream file1("file.txt");
	//for (int i = 0; i < 4; i++)
	//	rows[i].writeValuesToStream(file1, alignments, widths, ' ');

	MarkdownTable table("file.txt");
	table.print();
}
