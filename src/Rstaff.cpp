#include "Rstaff.h"

// most be included after c++ headers!
#include <stdio.h>
#include <Rdefines.h>

#ifdef __cplusplus
extern "C" {
#endif

	static SEXP type_tag;

	/* macro to check if ptr valid */
#define CHECK_PTR(s) do { \
		if (TYPEOF(s) != EXTPTRSXP || \
				R_ExternalPtrTag(s) != type_tag) \
				error("External pointer not valid!"); \
} while (0)

	/* Install the type tag */
	SEXP DatABELBaseCPP_init(void)
	{
		type_tag = install("AbstractMatrix");
		return R_NilValue;
	}

#ifdef __cplusplus
}
#endif

extern "C" {
	//        .Fortran("dqrls",
	//                  qr = x, n = n, p = p,
	//                  y = tra, ny = ny,
	//                  tol = as.double(tol),
	//                  coefficients = mat.or.vec(p, ny),
	//                  residuals = y, effects = y, rank = integer(1L),
	//                  pivot = 1L:p, qraux = double(p), work = double(2*p),
	//                  PACKAGE="base")$coefficients[2]

	void dqrls_(
			double*, int*, int*,
			double*, int*,
			double*,
			double*,
			double*, double*, int*,
			int*, double*, double*
	);
}

extern "C" {
	//    .Fortran("ch2inv", x = x, nr, size, v = matrix(0, nrow = size,
	//        ncol = size), info = integer(1L), DUP = FALSE, PACKAGE = "base")
	void ch2inv_(
			double*,int*,int*,double*,int*
	);
}


extern "C" {

	SEXP get_nvars_R(SEXP s) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		SEXP out;
		PROTECT(out = allocVector(INTSXP, 1));
		unsigned int nvars = 0;

		try {
			nvars = (unsigned int) p->get_nvariables();
		} catch (int errcode) {
			nvars = 0;
		}

		if (nvars<=0) {
			out = R_NilValue;
		} else {
			INTEGER(out)[0] = nvars;
		}
		UNPROTECT(1);
		return out;
	}

	SEXP get_nobs_R(SEXP s) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		SEXP out;
		PROTECT(out = allocVector(INTSXP, 1));
		unsigned int nobss = 0;

		try {
			nobss = (unsigned int) p->get_nobservations();
		} catch (int errcode) {
			nobss = 0;
		}

		if (nobss<=0) {
			out = R_NilValue;
		} else {
			INTEGER(out)[0] = nobss;
		}
		UNPROTECT(1);
		return out;
	}

	SEXP get_all_varnames_R(SEXP s) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		R_len_t nvars = (R_len_t) 0;

		try {
			nvars = p->get_nvariables();
		} catch (int errcode) {
			return R_NilValue;
		}

		fixedchar tmp;
		SEXP ret;
		PROTECT(ret = allocVector(STRSXP, (R_len_t) nvars));

		try {
			for (unsigned long int i = 0; i< nvars; i++) {
				tmp = p->read_variable_name(i);
				SET_STRING_ELT(ret, i, mkChar(tmp.name));
			}
		} catch (int errcode) {
			error_R("something went terribly wrong in get_all_varnames_R\n");
			UNPROTECT(1);
			return ret;
		}
		UNPROTECT(1);
		return ret;
	}

	// !!!
	SEXP set_all_varnames_R(SEXP s, SEXP names) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		R_len_t nvars = (R_len_t) 0;

		try {
			nvars = p->get_nvariables();
		} catch (int errcode) {
			error_R("can not p->get_nvariables()\n");
			return R_NilValue;
		}

		// check that length of SEXP names is the same!!!

		for (unsigned long int i = 0; i < nvars; i++) {
			std::string varname = CHAR(STRING_ELT(names,i));
			try {
				p->write_variable_name(i,fixedchar(varname));
			} catch (int errcode) {
				error_R("can not set variable name for variable %ul\n",i);
				return R_NilValue;
			}
		}

		SEXP ret;PROTECT(ret = allocVector(LGLSXP, 1));LOGICAL(ret)[0] = TRUE;UNPROTECT(1);
		return ret;

	}

	SEXP get_all_obsnames_R(SEXP s) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		R_len_t nobss = (R_len_t) 0;

		try {
			nobss = p->get_nobservations();
		} catch (int errcode) {
			return R_NilValue;
		}

		fixedchar tmp;
		SEXP ret;
		PROTECT(ret = allocVector(STRSXP, (R_len_t) nobss));

		try {
			for (unsigned long int i = 0; i< nobss; i++) {
				tmp = p->read_observation_name(i);
				SET_STRING_ELT(ret, i, mkChar(tmp.name));
			}
		} catch (int errcode) {
			error_R("something went terribly wrong in get_all_obsnames_R\n");
			UNPROTECT(1);
			return ret;
		}
		UNPROTECT(1);
		return ret;
	}

	// !!!
	SEXP set_all_obsnames_R(SEXP s, SEXP names) {
		CHECK_PTR(s);

		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		R_len_t nobss = (R_len_t) 0;

		try {
			nobss = p->get_nobservations();
		} catch (int errcode) {
			error_R("can not p->get_nobservations()\n");
			return R_NilValue;
		}

		// check that length of SEXP names is the same!!!

		for (unsigned long int i = 0; i < nobss; i++) {
			std::string obsname = CHAR(STRING_ELT(names,i));
			try {
				p->write_observation_name(i,fixedchar(obsname));
			} catch (int errcode) {
				error_R("can not set observation name for observation %ul\n",i);
				return R_NilValue;
			}
		}

		SEXP ret;PROTECT(ret = allocVector(LGLSXP, 1));LOGICAL(ret)[0] = TRUE;UNPROTECT(1);
		return ret;

	}


	static void AbstractMatrixRFinalizer(SEXP x) {
		CHECK_PTR(x);
		if (x == R_NilValue) return;
		AbstractMatrix* p = (AbstractMatrix *) EXTPTR_PTR(x);
		if (p == NULL) return;
		//		p->free_resources();
		Rprintf("finalizing AbstractMatrix: %p\n", p);
		delete p;
	}

	// !!!
	SEXP disconnect_R(SEXP s) {
		AbstractMatrixRFinalizer(s);
		R_ClearExternalPtr(s);
		return R_NilValue;
	}

	SEXP externalptr_is_null(SEXP s) {
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = FALSE;
		if (p == NULL) LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;
	}

	SEXP open_float_FileMatrix_R(SEXP fname, SEXP cacheMb) {
		unsigned long int cachesizeMb = (unsigned long int) INTEGER(cacheMb)[0];
		std::string filename = CHAR(STRING_ELT(fname,0));
		if (cachesizeMb<0) {
			error_R("negative cache size");
			return R_NilValue;
		}

		AbstractMatrix* p = NULL;

		try {
			p = new filevector(filename,cachesizeMb);
		} catch (int errcode) {
			return R_NilValue;
		}

		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		SEXP val = R_MakeExternalPtr(p, type_tag, R_NilValue);
		R_RegisterCFinalizerEx(val, AbstractMatrixRFinalizer, (Rboolean) TRUE);
		return val;
	}

	SEXP read_variable_float_FileMatrix_R(SEXP nvar, SEXP s) {
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		unsigned long int nvariable = (unsigned long int) INTEGER(nvar)[0];
		unsigned int nobs = 0;
		try {
			nobs = p->get_nobservations();
		} catch (int errcode) {
			return R_NilValue;
		}
		float * internal_data = new (std::nothrow) float [nobs];

		try {
			p->read_variable_as(nvariable, internal_data);
		} catch (int errcode) {
			return R_NilValue;
		}

		SEXP out;
		PROTECT(out = allocVector(REALSXP, (R_len_t) p->get_nobservations()));
		for (unsigned long int i=0;i< nobs; i++) REAL(out)[i] = (double) internal_data[i];
		UNPROTECT(1);

		delete [] internal_data;

		return out;
	}

	SEXP write_variable_double_FileMatrix_R(SEXP nvar, SEXP data, SEXP s) {
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		unsigned long int nvariable = (unsigned long int) INTEGER(nvar)[0];
		// here generally should be very careful -- what type of data is IN?

		unsigned int nvars = 0;
		unsigned int nobss = 0;

		try {
			nvars = p->get_nvariables();
		} catch (int errocode) {
			return R_NilValue;
		}

		if (nvariable <0 || nvariable >= nvars) {
			error_R("nvar (%lu) out of range!\n",nvariable);
			return R_NilValue;
		}

		try {
			nobss = p->get_nobservations();
		} catch (int errcode) {
			return R_NilValue;
		}

		//		float * internal_data = new (std::nothrow) float [nobss];
		double internal_data[nobss];
		if (internal_data == NULL) {
			error_R("internal_data pointer is NULL\n");
			return R_NilValue;
		}

		for (unsigned long int i=0;i< nobss;i++) {
			internal_data[i] = (double) REAL(data)[i];
		}

		//		Rprintf("\n%lu, %lu\n",nvariable,nobss);
		//		for (unsigned long int i=0;i< nobss;i++) {
		//			Rprintf("%f ",internal_data[i]);
		//		}
		try {
			p->write_variable_as(nvariable, internal_data);
		} catch (int errcode) {
			error_R("can not write variable %ul\n",nvariable);
		}

		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;
	}

	// !!!
	SEXP set_cachesizeMb_R(SEXP s, SEXP SizeMB)
	{
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}
		unsigned long int sizeMb = (unsigned long int) INTEGER(SizeMB)[0];
		try {
			p->set_cachesizeMb( sizeMb );
		} catch (int errcode) {
			error_R("cannot reset cache size\n");
			return R_NilValue;
		}

		SEXP ret;PROTECT(ret = allocVector(LGLSXP, 1));LOGICAL(ret)[0] = TRUE;UNPROTECT(1);
		return ret;

	}

	SEXP get_cachesizeMb_R(SEXP s)
	{
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		unsigned long int sizeMb = 0;

		try {
			sizeMb = p->get_cachesizeMb();
		} catch (int errcode) {
			return R_NilValue;
		}

		SEXP out;
		PROTECT(out = allocVector(INTSXP, 1));
		INTEGER(out)[0] = (int) sizeMb;
		UNPROTECT(1);
		return(out);
	}

	// !!!
	SEXP text2fvf_R(SEXP Fnames, SEXP IntPars)
	{

		std::string program_name = "text2fvf_R";
		std::string infilename = CHAR(STRING_ELT(Fnames,0));
		std::string outfilename = CHAR(STRING_ELT(Fnames,1));
		std::string rownamesfilename = CHAR(STRING_ELT(Fnames,2));
		std::string colnamesfilename = CHAR(STRING_ELT(Fnames,3));
		unsigned long rownames = (unsigned long) INTEGER(IntPars)[0];
		unsigned long colnames = (unsigned long) INTEGER(IntPars)[1];
		unsigned long skiprows = (unsigned long) INTEGER(IntPars)[2];
		unsigned long skipcols = (unsigned long) INTEGER(IntPars)[3];
		int transpose = (int) INTEGER(IntPars)[4];
		int Rmatrix = (int) INTEGER(IntPars)[5];
		unsigned short Type = (unsigned short ) INTEGER(IntPars)[6];

		try {
			text2fvf(program_name,
					infilename, outfilename,
					rownamesfilename, colnamesfilename,
					rownames, colnames,
					skiprows, skipcols,
					transpose, Rmatrix, Type, false);
		} catch (int x) {
			error_R("failed in text2fvf_R\n");
			return R_NilValue;
		}

		//		Rprintf("well-finished in text2_float_fvf_R!\n");
		SEXP ret;PROTECT(ret = allocVector(LGLSXP, 1));LOGICAL(ret)[0] = TRUE;UNPROTECT(1);
		return ret;

	}

	SEXP ini_empty_FileMatrix_R(SEXP fname, SEXP nvars, SEXP nobs, SEXP Type)
	{
		// internal format data types
		//#define UNSIGNED_SHORT_INT 1
		//#define SHORT_INT          2
		//#define UNSIGNED_INT       3
		//#define INT                4
		//#define FLOAT              5
		//#define DOUBLE             6

		unsigned long int nvariables = (unsigned long int) INTEGER(nvars)[0];
		unsigned long int nobservations = (unsigned long int) INTEGER(nobs)[0];
		std::string filename = CHAR(STRING_ELT(fname,0));
		unsigned short int type = (unsigned short int) INTEGER(Type)[0];

		if (type <=0 || type > 6) {
			error_R("unknow type %u\n",type);
			return R_NilValue;
		}
		try {
			// last flag -- override
			initialize_empty_file(filename, nvariables, nobservations, type, false);
		} catch (int errcode) {
			error_R("failed in ini_empty_FileMatrix_R");
			return R_NilValue;
		}

		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;

	}

	//virtual void save(string new_file_name, unsigned long int nvars, unsigned long int nobss, unsigned long int * varindexes, unsigned long int * obsindexes)
	SEXP save_R(SEXP New_file_name, SEXP IntPars, SEXP s)
	{
		CHECK_PTR(s);
		AbstractMatrix * p = (AbstractMatrix*)R_ExternalPtrAddr(s);
		if (p == NULL) {
			error_R("pointer is NULL\n");
			return R_NilValue;
		}

		std::string new_file_name = CHAR(STRING_ELT(New_file_name,0));
		unsigned long int nvars = (unsigned long int) INTEGER(IntPars)[0];
		unsigned long int nobss = (unsigned long int) INTEGER(IntPars)[1];
		unsigned long int varindexes[nvars];
		unsigned long int obsindexes[nobss];

		for (unsigned long int i = 0; i < nvars; i++)
			varindexes[i] = (unsigned long int) INTEGER(IntPars)[i+2];
		for (unsigned long int i = 0; i < nobss; i++)
			obsindexes[i] = (unsigned long int) INTEGER(IntPars)[i+2+nvars];

		try {
			p->save(new_file_name,nvars,nobss,varindexes,obsindexes);
		} catch (int errcode) {
			error_R("can not save data to file %s\n",new_file_name.c_str());
			return R_NilValue;
		}

		SEXP ret;
		PROTECT(ret = allocVector(LGLSXP, 1));
		LOGICAL(ret)[0] = TRUE;
		UNPROTECT(1);
		return ret;
	}

	//      subroutine dqrls(x,n,p,y,ny,tol,b,rsd,qty,k,jpvt,qraux,work)
	//      integer n,p,ny,k,jpvt(p)
	//      double precision x(n,p),y(n,ny),tol,b(p,ny),rsd(n,ny),
	//     .                 qty(n,ny),qraux(p),work(p)

	void CPP_dqrls(
			double * x, int * n, int * p,
			double * y, int * ny,
			double * tol,
			double * b,
			double * rsd, double * qty, int * k,
			int * jpvt, double * qraux, double * work
	)
	{
		dqrls_(x,n,p,y,ny,tol,b,rsd,qty,k,jpvt,qraux,work);
	}
	/**
	void apply_CPP_dqrls(
	     int * conn,
	     double * output,
	     double * x, int * n, int * p, 
	     double * y, int * ny, 
	     double * tol,
	     double * b,
	     double * rsd, double * qty, int * k,
	     int * jpvt, double * qraux, double * work
	    ) 
	{
		double rss, resvar;
		int info;
		fr_type tmp = floatFileMatrix[0].data_type;
		double * saveX = new (std::nothrow) double [(*n)*(*p)];
		if (!saveX) error("cannot get mem for 'saveX'\n");
		double * v = new (std::nothrow) double [(*p)*(*p)];
		if (!v) error("cannot get mem for 'v'\n");
		double * se = new (std::nothrow) double [(*p)];
		if (!se) error("cannot get mem for 'se'\n");
		unsigned long int offset = (*n)*((*p)-1);
		for (int obs=0;obs<((*n)*(*p));obs++) saveX[obs]=x[obs];
		for (int var=0;var<(int)tmp.nvariables;var++) {

			for (unsigned int obs=0;obs<tmp.nobservations;obs++) x[obs]=saveX[obs];

			read_variable_float_FileMatrix(&var,
					(x+offset),conn);

			dqrls_(x,n,p,y,ny,tol,b,rsd,qty,k,jpvt,qraux,work);
			ch2inv_(x,n,p,v,&info);

			rss = 0;
			for (unsigned int i=0;i<tmp.nobservations;i++) rss+=rsd[i]*rsd[i];
			resvar = rss/((double)(n-p));
			for (int i=0;i<(*p);i++) se[i] = v[i*(*p)+i]*resvar;

			output[var] = b[1];
		}
		delete [] saveX;
		delete [] v;
		delete [] se;
	}
	 **/

} // end extern
