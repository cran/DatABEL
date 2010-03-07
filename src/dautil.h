#ifndef __DAUTIL_H__
#define __DAUTIL_H__

#include <Rdefines.h>

#ifdef __cplusplus
extern "C" {
#endif

void error_R (const char * format, ... );
SEXP extract_text_file_column_cpp(SEXP, SEXP);

#ifdef __cplusplus
}
#endif


#endif
