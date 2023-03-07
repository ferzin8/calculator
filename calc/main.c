#include"common.h"
#include"calc.h"
#include"routine.h"

void PrintCalcError(tCalcError err, char* str)
{
	assert(err != ERROR_NO);
	assert(str);

	//I take out error messages
	switch (err)
	{
	case ERROR_DIV:
		printf("%s == ERROR: Division by zero!\n", str);
		break;		
	case ERROR_ITD:
		printf("%s == ERROR: Indeterminacy!\n", str);
		break;
	case ERROR_OPD:
		printf("%s == ERROR: You haven't the left operand!\n", str);
		break;
	case ERROR_SMB:
		printf("%s == ERROR: You entered the wrong symbols!:(\n", str);
		break;
	case ERROR_BR:
		printf("%s == ERROR: Wrong brakets in your string!\n", str);
		break;	
	case ERROR_FUNCOPD:
		printf("%s == ERROR: Wrong function operand!\n", str);
		break;
	case ERROR_ROOT:
		printf("%s == ERROR: Negative root!\n", str);
		break;
	case ERROR_WR:
		printf("%s == ERROR: Wrong operang value!\n", str);
		break;
	default:
		assert(0);		
	}
}

int main(int argc, char *argv[])
{	
	FILE *f = NULL;
	char *str = NULL;//initial string
	char* cpystr = NULL;//copy of initial string
	Node_t* ourTree = NULL;
	
	//Will remove detailed error messages
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

  //I check on quantity of arguments
	if (argc > 2)
	{
		printf("ERROR: Too many arguments!\n");
		return 1;
	}

	if (argc == 1)
	{
		f = stdin;
	}
	else
	{
		f = fopen(argv[1], "r");
	}

	if (f == NULL)
	{
		printf("ERROR: Can't open this file!\n");
		return -1;
	}
	//We start working with the file
	while (!feof(f))
	{ 
		str = NewString(f);    

		if (str != NULL)
		{	 
			//we do check for an empty string
			if (EmptyString(str))
			{
				printf("%s\n", str);
			}
			else
			{
				//I create the copy of an initial string and I start working with it
				cpystr = (char*)malloc((strlen(str) + 1)*sizeof(char));
				strcpy(cpystr, str);				
				cpystr = Simplification(cpystr); 

				//I build a tree
				g_CalcError = ERROR_NO;				
				ourTree = MakeATree(cpystr, NULL);

				//I check on mistakes
				if (g_CalcError != ERROR_NO)
				{
					PrintCalcError(g_CalcError, str);					
				}				
				else
				{
					DO(Solve, ourTree); 

					if (g_CalcError != ERROR_NO)
					{
						PrintCalcError(g_CalcError, str);							
					}
					else
					{
						//If the program didn't give out mistakes, we output result
						printf("%s == %g\n", str, ourTree->value);
					}										
				}

				if (ourTree != NULL)
				{
					g_CalcError = ERROR_NO;
					//I release memory
					DO(DelNode, ourTree);
				}
				free(cpystr);
			}
			free(str);			
		}  
	}  

  _CrtDumpMemoryLeaks();

	return 0;
}