#ifndef __ITERATOR_FUNCTIONS_H__
#define __ITERATOR_FUNCTIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

	double prod(double *, unsigned int );
	void prodWrapper(double *, unsigned long int , double *,
					unsigned long int &, unsigned long int &,
					unsigned int , double *);
	double sum(double *, unsigned int );
	void sumWrapper(double *, unsigned long int , double *,
					unsigned long int &, unsigned long int &,
					unsigned int , double *);
	double sumpower(double *, unsigned int , int );
	void sumpowerWrapper(double *, unsigned long int ,
					double *, unsigned long int &,
					unsigned long int &, unsigned int , double *);
	void databel_impute_prob_2_databel_mach_dose(double *, unsigned int ,
				double *, int );
	void databel_impute_prob_2_databel_mach_doseWrapper(double *,
				unsigned long int , double *,
				unsigned long int &, unsigned long int &,
				unsigned int , double *);
	void databel_impute_prob_2_databel_mach_prob(double *, unsigned int ,
				double *);
	void databel_impute_prob_2_databel_mach_probWrapper(double *,
				unsigned long int , double *,
				unsigned long int &, unsigned long int &,
				unsigned int , double *);
	// TODO: add declaration for qtscore_glob and qtscore_globWrapper, once arguments correct

#ifdef __cplusplus
}
#endif

#endif
