
#include "Rstaff.h"
#include "iterator_functions.h"


#ifdef __cplusplus
extern "C" {
#endif

	MethodConvStruct methodConverter[] =
		{ { "sum", &sumWrapper },
		  { "prod", &prodWrapper },
		  { "sumpower", &sumpowerWrapper },
		  {	"databel_impute_prob_2_databel_mach_dose", &databel_impute_prob_2_databel_mach_doseWrapper },
		  {	"databel_impute_prob_2_databel_mach_prob", &databel_impute_prob_2_databel_mach_probWrapper }
		};

	bool getDataNew(AbstractMatrix *inData, double *outData, unsigned int datasize,
			unsigned long int index, unsigned int margin) {
		if (margin == 2) { // column-wise
			try {
				inData->readVariableAs(index, outData);
			} catch (int errcode) {
				return false;
			}
		} else { // row-wise
			double dTmp;
			for (unsigned long int j = 0; j < datasize; j++) {
				inData->readElementAs(j, index, dTmp);
				outData[j] = dTmp;
			}
		}
	}

	void getDataOld(char const *inData, double *outData, unsigned int datasize,
			unsigned int index, unsigned int margin) {
		int i, j, iTmp;
		char str;
		int msk[4] = { 192, 48, 12, 3 };
		int ofs[4] = { 6, 4, 2, 0 };
		int nbytes; // the length of a row
		if ((datasize % 4) == 0) {
			nbytes = datasize / 4;
		} else {
			nbytes = ceil(1. * datasize / 4.);
		}

		if (margin == 1) { // row-wise
			int offset = index * nbytes;
			for (i = offset; i < offset + nbytes; i++) {
				str = inData[i];
				for (j = 0; j < 4; j++) {
					iTmp = str & msk[j];
					iTmp >>= ofs[j];
					outData[i] = static_cast<double> (iTmp);
				}
			}
		} else { // column-wise
			int insloc = 0;
			j = index % 4;
			for (i = index; i < datasize * nbytes; i += nbytes, insloc++) {
				str = inData[i];
				iTmp = str & msk[j];
				iTmp >>= ofs[j];
				outData[insloc] = static_cast<double> (iTmp);
			}
		}
	}


	SEXP iterator(SEXP data, SEXP nrids, SEXP nrobs, SEXP method, SEXP outputtype,
			SEXP margin, SEXP nrarg, ...) {

		// Check and get the data supplied
		unsigned long int nids, nobs;
		bool newtype = true;
		AbstractMatrix *pDataNew;
		char const *pDataOld;

		if (TYPEOF(data) == EXTPTRSXP) {
			pDataNew = getAbstractMatrixFromSEXP(data);
			if (pDataNew == NULL) {
				error_R("Pointer to data is NULL\n");
				return R_NilValue;
			}
			nids = pDataNew->getNumVariables();
			nobs = pDataNew->getNumObservations();
		} else if (TYPEOF(data) == STRSXP) {
			newtype = false;
			pDataOld = CHAR(data);
			nids = INTEGER(nrids)[0];
			nobs = INTEGER(nrobs)[0];
		} else {
			error_R("Incorrect data type\n");
			return R_NilValue;
		}

		// Find out and check the function supplied
		char const *methodName = CHAR(STRING_ELT(method, 0));
		myfunctiontype *pMethod = NULL;
		for (unsigned int i = 0; i < sizeof(methodConverter); i++) {
			if (strcmp(methodConverter[i].methodName, methodName) == 0) {
				pMethod = methodConverter[i].functionPtr;
				break;
			}
		}
		if (pMethod == NULL) {
			error_R("No (valid) function supplied\n");
			return R_NilValue;
		}

		// Find out the desired output type (file) supplied
		bool fv = true;
		char const *outputName = CHAR(STRING_ELT(outputtype, 0));
		if (strcmp(outputName, "R") == 0) {
			fv = false;
		}

		// Get the margin supplied
		int mar = INTEGER(margin)[0];
		if (mar < 1 || mar > 2) {
			error_R("No (valid) margin supplied\n");
			return R_NilValue;
		}

		// Get the nr. of additional arguments supplied
		unsigned int narg = INTEGER(nrarg)[0];

		// Get the additional parameters supplied, if any, and cast them to doubles
		unsigned int argListSize = narg > 0 ? narg : 1;
		double * argList = new (std::nothrow) double [argListSize];
		if (argList == NULL) {error_R("can not allocate RAM for argList\n");return R_NilValue;}
		va_list ap;
		va_start(ap, nrarg); // nrarg is last known parameter
		for (unsigned register int i = 0; i < narg; i++) {
			SEXP tmpPointer = va_arg(ap, SEXP);
			argList[i] = REAL(tmpPointer)[0];
		}
		va_end(ap);

		// The actual data handling part:

		unsigned long int ncol, nrow;
		if (mar == 1) { // row-wise
			ncol = nobs;
			nrow = nids;
		} else { // column-wise (default)
			ncol = nids;
			nrow = nobs;
		}

		unsigned long int nrow_new, ncol_multi;

		// Get the dimensions of the output the function of our choosing will be giving
		pMethod(0, nrow, 0, ncol_multi, nrow_new, narg, argList);
		//cout << "ncol_multi=" << ncol_multi << endl;
		//cout << "nrow_new=" << nrow_new << endl;
		// Allocate vector
		// Start output SEXP for passing to R
		// Even when the output is put into a filevector, we still return an (empty) SEXP
		SEXP out;
		// Declare output filevector (whether we'll be using it or not)
		FileVector* tmpFV = NULL;
		FilteredMatrix* outFV = NULL;
		//AbstractMatrix * outFV;
		if (!fv) {
			// Initialize output matrix once real number of rows is known
			// ASSUMPTION: nrow_new remains constant over calls to function wrapper
			PROTECT(out = allocVector(REALSXP, (R_len_t)(ncol * ncol_multi
					* nrow_new)));
		} else {
			// To avoid null pointer error, make output SEXP to return (although it will be empty)
			PROTECT(out = allocVector(REALSXP, (R_len_t) 1));
			try {
				initializeEmptyFile(outputName, ncol * ncol_multi,
						nrow_new, FLOAT, false);
			} catch (int errcode) {
				error_R("Failed in iterator - call - initializeEmptyFile");
				delete [] argList;
				UNPROTECT(1);
				return R_NilValue;
			}
			try {
	   			tmpFV = new FileVector(outputName,64,false);
	   			outFV = new FilteredMatrix(*tmpFV);
				//outFV = new FileVector(outputName, 64, false);
			} catch (int errcode) {
				error_R("Cannot initialize output file\n");
				delete [] argList;
				UNPROTECT(1);
				return R_NilValue;
			}
		}

		double * internal_data = new (std::nothrow) double [nrow];
		if (internal_data == NULL) {
			error_R("can not allocate RAM for internal_data\n");
			delete [] argList;
			UNPROTECT(1);
			return R_NilValue;
		}
		double * out_data = new (std::nothrow) double [nrow_new * ncol_multi];
		if (out_data == NULL) {
			error_R("can not allocate RAM for out_data\n");
			delete [] argList;
			delete [] internal_data;
			UNPROTECT(1);
			return R_NilValue;
		}

		// Read in data and apply function (row- or column-wise)
		for (unsigned long int i = 0; i < ncol; i++) {

			// Get row or column
			if (newtype) {
				getDataNew(pDataNew, internal_data, nrow, i, mar);
			} else {
				getDataOld(pDataOld, internal_data, nrow, i, mar);
			}

			// Apply function of choosing
			pMethod(internal_data, nrow, out_data, ncol_multi, nrow_new, narg,
					argList);

			// Write analyzed data to R vector or filevector
			for (unsigned long int j = 0; j < ncol_multi; j++) {
				if (!fv) {
					for (unsigned long int k = 0; k < nrow_new; k++) {
						// Add to output SEXP
						REAL(out)[(i * ncol_multi + j) * nrow_new + k]
						          = out_data[k];
					}
				} else {
					outFV->writeVariableAs(i * ncol_multi + j, &out_data[j*nrow_new]);
				}
			}
		}

		if (fv) {
			//tmpFV.deInitialize();
			delete tmpFV;
			delete outFV;
		}

		delete [] argList;
		delete [] internal_data;
		delete [] out_data;

		UNPROTECT(1);
		return out;
	}


	/**
	// OLD STUFF BELOW HERE:
	// iterator and other staff
	SEXP databel_impute_prob_2_databel_mach_dose(SEXP imputedata, SEXP OutFileName, SEXP CacheSizeMb)
	{
		CHECK_PTR(imputedata);
		//Rprintf("CHECK PASSED\n");
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(imputedata);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		std::string outfilename = CHAR(STRING_ELT(OutFileName,0));
		unsigned long int cachesizeMb = (unsigned long int) INTEGER(CacheSizeMb)[0];
		if (cachesizeMb<0) {
			error_R("negative cache size\n");
			return R_NilValue;
		}

		unsigned int nvars, nobss;

		try {
			nvars = (unsigned int) p->getNumVariables();
		} catch (int errcode) {
			error_R("ERROR: can not getNumVariables\n");
			return R_NilValue;
		}
		try {
			nobss = (unsigned int) p->getNumObservations();
		} catch (int errcode) {
			error_R("ERROR: can not getNumObservations\n");
			return R_NilValue;
		}

		if ((nobss % 3) != 0)
		{
			error_R("ERROR: nobss not divisable by 3\n");
			return R_NilValue;
		}

		unsigned int new_nvars = nvars, new_nobss = (unsigned int) nobss/3;

		try {
			// last flag -- override
			initializeEmptyFile(outfilename, new_nvars, new_nobss, FLOAT, false);
		} catch (int errcode) {
			error_R("failed in databel_impute_prob_2_databel_mach_dose - call - initializeEmptyFile");
			return R_NilValue;
		}

		Rprintf("READY TO INI NEW\n");
		AbstractMatrix * outFV;
		try {
			outFV = new FileVector(outfilename,cachesizeMb);
		} catch (int errcode) {
			error_R("can not ini outfile\n");
			return R_NilValue;
		}

		float tmpout[new_nobss], tmpin[nobss];
		for (unsigned int var=0; var<nvars; var++)
		{
			p->readVariableAs(var, tmpin);
			//for (int k=0;k<nobss;k++) Rprintf("%f ",tmpin[k]); Rprintf("\n");
			unsigned int j = 0;
			for (unsigned int obs=0;obs<nobss;obs+=3)
			{
				tmpout[j++] = 2.*tmpin[obs+2]+tmpin[obs+1];
			}
			//for (int k=0;k<new_nobss;k++) Rprintf("%f ",tmpout[k]); Rprintf("\n");
			outFV->writeVariableAs(var,tmpout);
		}

		delete outFV;
		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;
	}


	SEXP databel_impute_prob_2_databel_mach_prob(SEXP imputedata, SEXP OutFileName, SEXP CacheSizeMb)
	{
		CHECK_PTR(imputedata);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(imputedata);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		std::string outfilename = CHAR(STRING_ELT(OutFileName,0));
		unsigned long int cachesizeMb = (unsigned long int) INTEGER(CacheSizeMb)[0];
		if (cachesizeMb<0) {
			error_R("negative cache size\n");
			return R_NilValue;
		}

		unsigned int nvars, nobss;

		try {
			nvars = (unsigned int) p->getNumVariables();
		} catch (int errcode) {
			error_R("ERROR: can not getNumVariables\n");
			return R_NilValue;
		}
		try {
			nobss = (unsigned int) p->getNumObservations();
		} catch (int errcode) {
			error_R("ERROR: can not getNumObservations\n");
			return R_NilValue;
		}

		if ((nobss % 3) != 0)
		{
			error_R("ERROR: nobss not divisable by 3\n");
			return R_NilValue;
		}

		unsigned int new_nvars = 2*nvars, new_nobss = (unsigned int) nobss/3;

		try {
			// last flag -- override
			initializeEmptyFile(outfilename, new_nvars, new_nobss, FLOAT, false);
		} catch (int errcode) {
			error_R("failed in databel_impute_prob_2_databel_mach_prob - call - initializeEmptyFile");
			return R_NilValue;
		}

		AbstractMatrix * outFV;
		try {
			outFV = new FileVector(outfilename,cachesizeMb);
		} catch (int errcode) {
			error_R("can not ini outfile\n");
			return R_NilValue;
		}

		float tmpout1[new_nobss], tmpout2[new_nobss], tmpin[nobss];
		unsigned int coutvar = 0;
		for (unsigned int var=0; var<nvars; var++)
		{
			p->readVariableAs(var, tmpin);
			//for (int k=0;k<nobss;k++) Rprintf("%f ",tmpin[k]); Rprintf("\n");
			unsigned int j = 0;
			for (unsigned int obs=0;obs<nobss;obs+=3)
			{
				tmpout1[j] = tmpin[obs+1];
				tmpout2[j++] = tmpin[obs+2];
			}
			//for (int k=0;k<new_nobss;k++) Rprintf("%f ",tmpout2[k]); Rprintf("\n");
			//for (int k=0;k<new_nobss;k++) Rprintf("%f ",tmpout1[k]); Rprintf("\n");
			outFV->writeVariableAs(coutvar++,tmpout2);
			outFV->writeVariableAs(coutvar++,tmpout1);
		}

		delete outFV;
		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;
	}


	 **/

#ifdef __cplusplus
}
#endif
