#include"common.h"
#include"calc.h"
#include"routine.h"
#include<math.h>
#include<ctype.h>
#include<float.h>
//#include<limits.h>

#define DBL_ZERO 1E-DBL_DIG //(DBL_DIG -  Number of significant digits in a floating point number.)

tCalcError g_CalcError = ERROR_NO;

Node_t* _AddNode(tOperation type, double value, Node_t* parent)//we create a new branch of a tree 
{
	Node_t* tree = NULL;

	tree = (Node_t*)malloc(sizeof(Node_t));

	(*tree).type = type;
	(*tree).value = value;
	(*tree).parent = parent;
	(*tree).left = NULL;
	(*tree).right = NULL;
	
	if (parent)  //if there is a root, create the right and left leaf
	{
		if ((*parent).left != NULL)
		{
			(*parent).right = tree;
		}
		else
		{
			(*parent).left = tree;
		}
	}
	
	return tree;
}

Node_t* MakeATree(char* str, Node_t* root)//We create a tree
{	
	int priority[] = {0, 1, 1, 2, 2, 3};
	char *p = NULL;
	char *q = NULL;
	char *subStr = NULL;
	char *subStrLeft = NULL;
	char *subStrRight = NULL;	
	int j = 0;	
	int idx = 0;
	Node_t* tree = NULL;
	double value = 0;
	char* error = NULL;
	int length = 0;
	tOperation lastOps = OP_NO;
	tOperation calcOp = OP_NO;
	tOperation funck = OP_NO;

	assert(str != NULL);

	if (*str == 0)
	{	
		g_CalcError = ERROR_OPD;
		return NULL;
	}

	//Compliance of brackets
	p = str;

	while (*p != 0)
	{		
		if (*p == '(')
		{
			j = Brackets(p, 0);

			if (j == -1)
			{
				g_CalcError = ERROR_BR;
				return NULL;
			}

			p += (j + 1);
		}
		else if (*p == ')')
		{
			g_CalcError = ERROR_BR;
			return NULL;
		}
		else
		{
			// We look for the last operation on a priority
			calcOp = SmbInOp(*p);

			if (calcOp != OP_NO)
			{
				if (!(  (p - str > 1) &&
						((*(p - 1) == 'e') || (*(p - 1) == 'E')) &&
						((*(p - 2) == '.') || (isdigit((unsigned char)*(p - 2))))
					) )		   
				{
					if ((lastOps == OP_NO)  ||  ((priority[calcOp] <= priority[lastOps]) &&
							(calcOp != OP_EXT) && (SmbInOp(PrevNotSpace(str, p - str)) == OP_NO))) 
					{
						lastOps = calcOp;
						q = p;					
					}	
				}
			}
			p++;
		}			
	}

	// The line doesn't contain operators = a line contains number
	if (q == NULL)
	{
		/*1)*/
		//if we have the function
		funck = StrInOp(str, &length);

		if (funck != OP_NO)
		{
			p = str + length;	
			idx = FirstSmb(p);

			j = Brackets(p, idx);

			if (j == -1)
			{
				g_CalcError = ERROR_BR;
				return NULL;
			}		

			if (p[j + 1] != 0)
			{
				g_CalcError = ERROR_FUNCOPD;
				return NULL;
			}

			//After all checks we build a tree
			tree = _AddNode(funck, 0, root);
			subStr = (char*)malloc((strlen(p) + 1)*sizeof(char));
			strcpy(subStr, p);
			subStr = Simplification(subStr);
			MakeATree(subStr, tree);
			free(subStr);
			return tree;
						
		}
	
		/*2)*/
		//if we have a constannt
		value = Constants(str);
		
		if (value != 0)
		{
			tree = _AddNode(OP_NO, value, root);	
			return tree;
		}

		/*3)*/
		//if we have nothing interessting
		value = strtod(str, &error);

		if (*error == 0)
		{
			tree = _AddNode(OP_NO, value, root);
			return tree;
		}
		else
		{
			g_CalcError = ERROR_SMB;
			return NULL;
		}
	}

	assert((lastOps > OP_NO)&&(lastOps <= MaxOp));

	tree = _AddNode(lastOps, 0, root);

	assert(q >= str);

	// we create a tree of the left operand
	subStrLeft = (char*)malloc((q - str + 1)*sizeof(char));
	*(subStrLeft + (q - str)) = 0;
	memcpy(subStrLeft, str, (q - str)*sizeof(char));	
	subStrLeft = Simplification(subStrLeft);

	if (((tree->type == OP_PL)||(tree->type == OP_MIN)) && (*subStrLeft == 0))//unary minus
	{
		_AddNode(OP_NO, 0, tree);		
	}	
	else
	{
		MakeATree(subStrLeft, tree);
	}
	free(subStrLeft);

	if (g_CalcError == ERROR_NO)
	{
		// we create a tree of the right operand		
		length = strlen(str);
		subStrRight = (char*)malloc((length - (q - str))*sizeof(char));
		strcpy(subStrRight, q + 1);		
		subStrRight = Simplification(subStrRight);

		MakeATree(subStrRight, tree);
		free(subStrRight);
	}

	return tree;
}

void DO(fNode* Calculate, Node_t* root)
{
	assert(root);
	assert(!g_CalcError);

	//Left-side round of a tree
	if (root->left)
	{
		DO(Calculate, root->left);
	}

	if (g_CalcError == ERROR_NO)
	{
		if (root->right)
		{
			DO(Calculate, root->right);
		}

		if (g_CalcError == ERROR_NO)
		{
			Calculate(root);
		}
	}
}

void Solve(Node_t* const tree)
{
	assert(tree);	

	//I calculate expression
	switch (tree->type)
	{
	case OP_PL:
		tree->value = tree->left->value + tree->right->value;
		break;
	case OP_MIN:
		tree->value = tree->left->value - tree->right->value;
		break;
	case OP_INC:
		tree->value = tree->left->value * tree->right->value;
		break;
	case OP_DEL:	
		if (tree->right->value == 0)
		{
			g_CalcError = ERROR_DIV;
		}
		else
		{
			tree->value = tree->left->value / tree->right->value;			
		}
		break;
	case OP_EXT:
		if (!tree->left->value && !tree->right->value)
		{
			g_CalcError = ERROR_ITD;
		}
		else
		{
			tree->value = pow(tree->left->value, tree->right->value);			
		}
		break;
	case OP_SQRT:
		if (tree->left->value < 0)
		{
			g_CalcError = ERROR_ROOT;
		}
		else
		{
			tree->value = sqrt(tree->left->value);
		}
		break;
	case OP_SIN:
		tree->value = sin(tree->left->value);
		break;
	case OP_COS:
		tree->value = cos(tree->left->value);
		break;
	case OP_TG:
		if (fabs(cos(tree->left->value)) < DBL_ZERO)
		{
			g_CalcError = ERROR_WR;
		}
		else
		{
			tree->value = tan(tree->left->value);			
		}
		break;
	case OP_CTG:
		if (fabs(sin(tree->left->value)) < DBL_ZERO)
		{
			g_CalcError = ERROR_WR;
		}
		else
		{
			tree->value = 1/tan(tree->left->value);
		}
		break;
	case OP_ARCSIN:
		if (fabs(tree->left->value) > 1)
		{
			g_CalcError = ERROR_WR;
		}
		else
		{
			tree->value = asin(tree->left->value);
		}
		break;
	case OP_ARCCOS:
		if (fabs(tree->left->value) > 1)
		{
			g_CalcError = ERROR_WR;
		}
		else
		{
			tree->value = acos(tree->left->value);
		}
		break;
	case OP_ARCTG:
		tree->value = atan(tree->left->value);
		break;
	case OP_LN:
		if (tree->left->value <= 0)
		{
			g_CalcError = ERROR_WR;
		}
		else
		{
			tree->value = log(tree->left->value);
		}
		break;
	case OP_FLOOR:
		tree->value = floor(tree->left->value);
		break;
	case OP_CEIL:
		tree->value = ceil(tree->left->value);
		break;
	case OP_SIGN:
		if (tree->left->value < 0)
		{
			tree->value = -1;
		}
		else if (tree->left->value > 0)
		{
			tree->value = 1;
		}
		else
		{
			tree->value = 0;
		}
		break;
	case OP_TILDA:
		tree->value = floor(tree->left->value);
		break;
	default:
		assert(tree->type == OP_NO);
	}

	if (!_finite(tree->value))
	{
		g_CalcError = ERROR_WR;
	}
}

void DelNode(Node_t* const tree)
{
	assert(tree);
	free(tree);
}

