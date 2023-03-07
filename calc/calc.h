#ifndef CALC_H_INCLUDED__
	#define CALC_H_INCLUDED__

	#define MaxOp OP_TILDA
	#define MinFunck OP_SQRT
	#define MaxFunck OP_TILDA	
	
	typedef enum tag_Operations
	{
		OP_NO,//No operation
		OP_PL,//Plus
		OP_MIN,//Minus
		OP_INC,//Increase
		OP_DEL,//Delete
		OP_EXT, //Extent		
		OP_SQRT,//
		OP_SIN,//
		OP_COS,//
		OP_TG,//
		OP_CTG,//
		OP_ARCSIN,//
		OP_ARCCOS,//
		OP_ARCTG,//
		OP_LN,//Natural logarithm
		OP_FLOOR,//Down
		OP_CEIL,//Up
		OP_SIGN,//Sign
		OP_TILDA,//'~'
	}tOperation;	

	typedef struct tag_Node //Components of each branch of a tree. Definition
	{
		tOperation type;
		double value;
		struct tag_Node* left;
		struct tag_Node* right;
		struct tag_Node* parent;
	} Node_t;

	typedef void fNode(Node_t* const);

	typedef enum tag_CalcError
	{
		ERROR_NO = 0,
		ERROR_DIV = -1, //  Division by zero
		ERROR_ITD = -2, // Indeterminacy
		ERROR_ROOT = -3,//Negative root
		ERROR_OPD = -101,//Haven't the left operand
		ERROR_SMB = -102,//Wrong symbols
		ERROR_BR = -103,//Wrong Brackets
		ERROR_FUNCOPD = -104,//Wrong operand			
		ERROR_WR= -200//For all
	} tCalcError;

	extern tCalcError g_CalcError;
	
	void DO(fNode*, Node_t*);
	Node_t* MakeATree(char*, Node_t*);
	void Solve(Node_t* const);
	void DelNode(Node_t* const);	

#endif