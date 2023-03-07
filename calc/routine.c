#include"common.h"
#include"calc.h"
#include"routine.h"
#include<ctype.h>
#define _USE_MATH_DEFINES
#include<math.h>


char* NewString(FILE *f)//function which creates a new string
{
	char c;
	char* str = NULL;
	int n = 1;

	assert(f);

	c = (char)getc(f);

	if (feof(f))
	{
		return NULL;
	}

	str = (char *) malloc(sizeof(char));
	*str = 0;	

	while ((c != '\n')&&(!feof(f)))//at a lack of memory, we recreate it
	{
		n++;
		str = (char*)realloc(str, n*sizeof(char));
		*(str + n - 2) = c;
		c = (char)getc(f);
	}
	*(str + n - 1) = 0;

	assert((int)strlen(str) == n - 1);

	return str;
}

int EmptyString(char* str)//function which looks for an empty string
{  
	char *p = NULL;

	assert(str);
  
	for (p = str; *p != 0; p++)
	{
		if (!isspace((unsigned char)*p))
		{
			if (*p == '/')
			{     
				if (*(p + 1) == '/')
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else 
			{
				return 0;
			}
		}    
	} 
  return 1;
}

char PrevNotSpace(char* str, int idx)//I look for the first not space symbol
{
	int i = 0;
	
	assert(str);
	assert(!isspace(str[idx]));

	for (i = idx - 1; i >= 0; i--)
	{
		if (!isspace(str[i]))
		{
			return str[i];
		}
	}

	return 0;
}

char* Simplification(char* str)//delete external brackets and external space
{
	int i = 0;
	int j = 0;
		
	assert(str);

	do
	{
		i = FirstSmb(str);
		j = LastSmb(str);

		if (i == -1)
		{
			assert(j == -1);
			return str;
		}

		if ((i != 0) || (j != (int)strlen(str) - 1))
		{
			memcpy(str, str + i, (j - i + 1)*sizeof(char));
			*(str + j - i + 1) = 0;
			str = (char*)realloc(str, (j - i + 2)*sizeof(char));
		}	

		str = DelBrackets(str);
	} while ((int)strlen(str) != j - i + 1);	

	return str;
}

int Brackets(char* str, int idx)//I look for the brackets corresponding each other
{
	int cnt = 1;
	int i = 0;

	assert(str);

	if (str[idx] != '(')
	{
		return -1;
	}

	for (i = idx + 1; str[i] != 0; i++)
	{
		if (str[i] == '(')
		{
			cnt++;
		}
		else if (str[i] == ')')
		{
			cnt--;
		}

		if (cnt == 0)
		{
			return i;
		}
	}

	return -1;
}

char* DelBrackets(char* str)//I delete external brackets
{
	int i = 0;
	int j = 0;

	assert(str);

	for (i = 0, j = strlen(str) - 1; i < (int) strlen(str); i++, j--)
	{
		if (Brackets(str, i) != j)
		{
			break;
		}
	}

	if ( i == 0)
	{
		return str;
	}
	
	//I create a line without external brackets
	memcpy(str, str + i, (j - i + 2)*sizeof(char));
	*(str + j - i + 1) = 0;
	str = (char*)realloc(str, (j - i + 2)*sizeof(char));

	return str;
}

tOperation SmbInOp(char c)
{
	char Ops[] = {0, '+', '-', '*', '/', '^'};
	tOperation i = OP_NO;

	assert(c);

	for (i = OP_NO; i <= MaxOp; i++)
	{
		if (Ops[i] == c)
		{
			return i;
		}
	}

	return OP_NO;	
}

tOperation StrInOp(char *str, int *len)
{
	char* Funck[] = {"sqrt", "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "ln", "floor", "ceil", "sign", "~"};
	tOperation i = OP_NO;
		
	assert(str);

	for (i = MinFunck; i <= MaxFunck; i++)
	{
		*len = strlen(Funck[i - MinFunck]);

		if (_strnicmp(str, Funck[i - MinFunck], *len) == 0)
		{			
			return i;
		}		
	}

	*len = 0;

	return OP_NO; 
}

int FirstSmb(char *str)
{	
	int i = 0;

	assert(str);

	for (i = 0; *(str + i) != 0; i++)
	{
		if (!isspace((unsigned char)*(str + i)))
		{
			return i;
		}
	}

	return -1;
}

int LastSmb(char* str)
{	
	int length = 0;
	int i = 0;
	
	assert(str);

	length = strlen(str);

	for (i = length - 1; i >= 0; i--)
	{
		if (!isspace((unsigned char)*(str + i)))
		{
			return i;
		}
	}

	return -1;
}

double Constants(char* str)
{
	assert(str);
	
	if (_stricmp(str, "pi") == 0)
	{
		return M_PI;
	}
	else if (_stricmp(str, "e") == 0)
	{
		return M_E;
	}

	return 0;
}