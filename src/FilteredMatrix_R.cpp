#include "Rstaff.h"
#include <Rdefines.h>

#include "FilteredMatrix.h"

#ifdef __cplusplus
extern "C" {
#endif

    static void FilteredMatrixRFinalizer(SEXP x) {
   		CHECK_PTR(x);
   		if (x == R_NilValue) return;
   		FilteredMatrix* p = (FilteredMatrix *) EXTPTR_PTR(x);
   		if (p == NULL) return;
   		Rprintf("finalizing FilteredMatrix: %p\n", p);
   		delete p;
   	}

	SEXP create_FilteredMatrixFromAbstractMatrix_R(SEXP abstractMatrixSEXP) {
	    AbstractMatrix *abstractMatrix = (AbstractMatrix*)R_ExternalPtrAddr(abstractMatrixSEXP);

		FilteredMatrix * p = NULL;
//        unsigned long nvars = (unsigned long) INTEGER(selectedRows)[0];

/*
        , SEXP selectedRows, SEXP selectedCols
        unsigned long i;
        vector<unsigned long> rowMask;
        for(i=0;i<length(selectedRows);i++){
            rowMask.push_back(INTEGER(selectedRows)[i]);
        }
        vector<unsigned long> colMask;
        for(i=0;i<length(selectedCols);i++){
            rowMask.push_back(INTEGER(selectedCols)[i]);
        }*/

		try {
			p = new FilteredMatrix(*abstractMatrix);
		} catch (int errcode) {
			return R_NilValue;
		}

		if (p == NULL) {
			error_R("Error creating FilteredMatrix.\n");
			return R_NilValue;
		}
		SEXP val = R_MakeExternalPtr(p, type_tag, R_NilValue);
		R_RegisterCFinalizerEx(val, FilteredMatrixRFinalizer, (Rboolean) TRUE);
		return val;
	}


#ifdef __cplusplus
}
#endif
