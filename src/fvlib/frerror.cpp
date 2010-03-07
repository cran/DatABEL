/*
	This function should be re-implemented using Rprintf
	if compiled as a part of R library
*/
#include "frerror.h"

void error (const char * format, ... )
{
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
#ifdef R_R_H
	Rprintf("ERROR: %s",buffer);
	throw 1;
#else
	printf("ERROR: %s\n",buffer);
	exit(EXIT_FAILURE);
#endif
}

void warning (const char * format, ... )
{
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
#ifdef R_R_H
	Rprintf("WARNING: %s",buffer);
#else
	printf("WARNING: %s\n",buffer);
#endif
}

void message (const char * format, ... )
{
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
#ifdef R_R_H
	Rprintf("%s",buffer);
#else
	printf("%s",buffer);
#endif
}

void messageOnOff(int bit)
{
    message(bit?"ON":"OFF");
}
