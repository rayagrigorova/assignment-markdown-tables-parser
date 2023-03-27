﻿#include <iostream>
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
	//	rows[i].readCellsFromStream(file);
	//file.close();

	//for (int i = 0; i < rows[0].getNumberOfCells(); i++) {
	//	alignments[i] = Alignment::right;
	//	widths[i] = 18;
	//}

	//for (int i = 0; i < 4; i++)
	//	rows[i].printCells(alignments, widths, '-');

	//std::ofstream file1("file.txt");
	//for (int i = 0; i < 4; i++)
	//	rows[i].writeCellsToStream(file1, alignments, widths, ' ');

	MarkdownTable table("file.txt");
	table.print();
	std::cout << "\n\n";

	//const Alignment* alignments = table.getAlignments();
	//for (int i = 0; i < table.getNumberOfColumns(); i++) {
	//	switch (alignments[i]) {
	//	case Alignment::center:
	//		std::cout << "Center  ";
	//		break;
	//	case Alignment::right:
	//		std::cout << "Right  ";
	//		break;
	//	case Alignment::left:
	//		std::cout << "Left  ";
	//		break;
	//	}
	//}

	//table.changeColumnName("Syntax", "Sin");
	//table.print();

	//table.addRow("My new sdssdsdsd");
	//std::cout << "\n\n";
	//table.print();

	//table.changeCellAtIndex(4, "Paragraph", "hehe");
	//std::cout << "\n\n";
	//table.print();

	//table.changeCell("Text", "MyNewVal", "Description");
	//std::cout << "\n\n";
	//table.print();

	//table.selectPrint("Text", "Description");
}
