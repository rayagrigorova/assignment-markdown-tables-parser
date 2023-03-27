#pragma once

#include "MarkdownTable.h"

class MarkdownTableParser {

private:
	MarkdownTable table;

public:
	MarkdownTableParser();
	MarkdownTableParser(const MarkdownTable& table);

	~MarkdownTableParser();

	void load();
	void interface();
	void change();
	void changeRow();
	void addRow();
	void selectPrint() const;
	bool save() const;
	void print() const;

};