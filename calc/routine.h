#ifndef ROUTINE_H_INCLUDED__
	#define ROUTINE_H_INCLUDED__

	#include<stdio.h>
	#include"calc.h"

	char* NewString(FILE*);
	int EmptyString(char*);
	int Brackets(char*, int);
	char* DelBrackets(char*);
	tOperation SmbInOp(char);
	tOperation StrInOp(char*, int*);
	int FirstSmb(char*);
	int LastSmb(char*);
	char* Simplification(char*);
	char PrevNotSpace(char*, int);	
	double Constants(char*);

#endif