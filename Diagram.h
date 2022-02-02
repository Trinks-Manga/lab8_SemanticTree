#pragma once
#include "SemanticTree.h"

class Diagram
{
private:
	LEX lex;
	Scan* _sc;

	SemanticTree* st;
	int type;
	void Main();
	void Data();
	void Class();
	void While();
	void Equ();
	void SimpleOper();
	void ComplexOper();
	void Mass();
	void Expression();
	void Datas();
	void Func();
	SemanticTree* Field();

	int lookforward(int k);


public:
	Diagram(Scan* sc);
	void setTree(SemanticTree* s);
	bool Process();
};

