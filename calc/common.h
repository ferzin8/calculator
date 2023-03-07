#ifndef COMMON_H_INCLUDED__
	#define COMMON_H_INCLUDED__

	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
	#endif
	
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<assert.h>

#endif
