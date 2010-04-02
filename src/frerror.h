/*
	This function should be reimplemented using Rprintf 
	if compiled as a part of R library
*/

#ifndef __ERROR__
#define __ERROR__

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void error(const char * format, ... );
void warning(const char * format, ... );
void message(const char * format, ... );
//void messageOnOff(int bit);

#endif
