#ifndef __RSTAFF_H__
#define __RSTAFF_H__

#include <string>
#include <cstring>
#include <R.h>


//#include "const.h"
#include "convert_util.h"
////#include "convert_util.cpp"
#include "AbstractMatrix.h"
////#include "AbstractMatrix.cpp"
//#include "FileVector.h"
//#include "FileVector.cpp"
//#include "frerror.h"
////#include "frerror.cpp"
//#include "frutil.h"
////#include "frutil.cpp"
//#include "frversion.h"
#include "Transposer.h"
////#include "Transposer.cpp"
//#include "Logger.h" //9096728933
////#include "Logger.cpp"
//#include "CastUtils.h"
////#include "CastUtils.cpp"

#include "dautil.h"




// maximal number of file-matrices allowed
// #define MAX_FM_OBJECTS 10

#ifdef __cplusplus
extern "C" {
#endif

	static SEXP type_tag = install("AbstractMatrix");

	// macro to check if ptr valid
#define CHECK_PTR(s) do { \
		if (TYPEOF(s) != EXTPTRSXP || \
				R_ExternalPtrTag(s) != type_tag) \
				error("External pointer not valid!"); \
} while (0)

	//SEXP AbstractMatrix_init(void);
	typedef void (myfunctiontype)(double *, unsigned long int,
			double *, unsigned long int &, unsigned long int &, unsigned int, double *);

	typedef struct MethodConvStruct {
		char *methodName;
		myfunctiontype *functionPtr;
	};


#ifdef __cplusplus
}
#endif

#endif
