#include "Rstaff.h"
#include "iterator_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

	// For testing purposes, tried to move some functions+wrappers to GenABEL: failed
	// Product function + wrapper
	double prod(double *mydata, unsigned int size) {
		double prodtotal = mydata[0];
		for (register unsigned int i = 1; i < size; i++) {
			prodtotal *= mydata[i];
		}
		return prodtotal;
	}
	void prodWrapper(double *indata, unsigned long int indataSize, double *outdata,
			unsigned long int &outdataNcol, unsigned long int &outdataNrow,
			unsigned int narg, double *argList) {
		if (indata) {
			outdata[0] = prod(indata, indataSize);
		}
		outdataNcol = 1;
		outdataNrow = 1;
	}

	// Sum function + wrapper
	double sum(double *mydata, unsigned int size) {
		double sumtotal = 0.;
		for (register unsigned int i = 0; i < size; i++) {
			sumtotal += mydata[i];
		}
		return sumtotal;
	}
	void sumWrapper(double *indata, unsigned long int indataSize, double *outdata,
			unsigned long int &outdataNcol, unsigned long int &outdataNrow,
			unsigned int narg, double *argList) {
		if (indata) {
			outdata[0] = sum(indata, indataSize);
		}
		outdataNcol = 1;
		outdataNrow = 1;
	}

	// Sum of powers function + wrapper
	double sumpower(double *mydata, unsigned int size, int power) {
		double sumpowertotal = 0.;
		for (register unsigned int i = 0; i < size; i++) {
			sumpowertotal += pow(mydata[i], power);
		}
		return sumpowertotal;
	}
	void sumpowerWrapper(double *indata, unsigned long int indataSize,
			double *outdata, unsigned long int &outdataNcol,
			unsigned long int &outdataNrow, unsigned int narg, double *argList) {
		if (indata) {
			int power = static_cast<int> (argList[0]);
			outdata[0] = sumpower(indata, indataSize, power);
		}
		outdataNcol = 1;
		outdataNrow = 1;
	}

	// databel_impute_prob_2_databel_mach_dose function + wrapper
	void databel_impute_prob_2_databel_mach_dose(double *mydata, unsigned int size,
			double *outdata) {
		unsigned int j = 0;
		double zero = 0;
		for (unsigned int obs = 0; obs < size; obs += 3) {
			double edose = 2. * mydata[obs + 2] + mydata[obs + 1];
			if ((edose + mydata[obs]) < 0.1) {
				outdata[j] = 0/zero;
			} else {
				outdata[j] = edose;
			}
			j++;
		}
	}
	void databel_impute_prob_2_databel_mach_doseWrapper(double *indata,
			unsigned long int indataSize, double *outdata,
			unsigned long int &outdataNcol, unsigned long int &outdataNrow,
			unsigned int narg, double *argList) {
		if (indata) {
			//int power = static_cast<int> (argList[0]);
			databel_impute_prob_2_databel_mach_dose(indata, indataSize, outdata);
		}
		outdataNcol = 1;
		outdataNrow = indataSize / 3;
	}

	// databel_impute_prob_2_databel_mach_prob function + wrapper
	void databel_impute_prob_2_databel_mach_prob(double *mydata, unsigned int size,
			double *outdata) {
		double zero = 0;
		unsigned int j = 0;
		for (unsigned int obs = 0; obs < size; obs += 3) {
			if ((mydata[obs]+mydata[obs + 1]+mydata[obs + 2]) < 0.1) {
				outdata[j] = 0/zero;
				outdata[(unsigned int) size / 3 + j] = 0/zero;
			} else {
				outdata[j] = mydata[obs + 2];
				outdata[(unsigned int) size / 3 + j] = mydata[obs + 1]; // the two columns are put behind eachother
				//cout << "j=" << j << "; (unsigned int) size/3 + j =" << (unsigned int) size/3 + j << endl;
				//cout << "; value[j] =" << outdata[j] << "; value[...]=" << outdata[(unsigned int) size/3 + j] << endl;
			}
			j++;
		}
		//cout << "size=" << size << endl;
		//cout << "end-j=" << j << endl;
	}
	void databel_impute_prob_2_databel_mach_probWrapper(double *indata,
			unsigned long int indataSize, double *outdata,
			unsigned long int &outdataNcol, unsigned long int &outdataNrow,
			unsigned int narg, double *argList) {
		if (indata) {
			//int power = static_cast<int> (argList[0]);
			databel_impute_prob_2_databel_mach_prob(indata, indataSize, outdata);
		}
		outdataNcol = 2;
		outdataNrow = indataSize / 3;
	}

	/*
void qtscore_glob(char *gdata, double *pheno, int *Type, int *Nids, int *Nsnps,
		int *Nstra, int *stra, double *chi2) {
	int nsnps = (*Nsnps);
	int nstra = (*Nstra);
	int nids = (*Nids);
	int type = (*Type);
	int gt[nids];
	int i, j, cstr, igt, i1 = 1;
	int nbytes;
	int dgt;
	double Ttotg, mx, bb, totg[nstra], x2[nstra], sumx[nstra];
	double Tsg0, Tsg1, Tsg2, sg0[nstra], sg1[nstra], sg2[nstra], xg0[nstra],
			xg1[nstra], xg2[nstra];
	double u, v, u0, u1, u2, m0, m1, m2, v00, v02, v11, v12, v22, det;
	mx = -999.99;
	if ((nids % 4) == 0)
		nbytes = nids / 4;
	else
		nbytes = ceil(1. * nids / 4.);
	//	char chgt[nbytes];

	// TODO: remove this loop, nsnps is data size
	for (igt = 0; igt < nsnps; igt++) {
		get_snps_many(gdata + nbytes * igt, Nids, &i1, gt);
		for (j = 0; j < nstra; j++) {
			totg[j] = 0.;
			x2[j] = 0.;
			sumx[j] = 0.;
			sg0[j] = 0.;
			sg1[j] = 0.;
			sg2[j] = 0.;
			xg0[j] = 0.;
			xg1[j] = 0.;
			xg2[j] = 0.;
		}
		for (i = 0; i < nids; i++) {
			if (gt[i] != 0) {
				cstr = stra[i];
				dgt = gt[i] - 1;
				totg[cstr] += 1.0;
				if (dgt == 0) {
					sg0[cstr] += 1.0;
					xg0[cstr] += pheno[i];
				} else if (dgt == 1) {
					sg1[cstr] += 1.0;
					xg1[cstr] += pheno[i];
				} else if (dgt == 2) {
					sg2[cstr] += 1.0;
					xg2[cstr] += pheno[i];
				}
				x2[cstr] += pheno[i] * pheno[i];
				sumx[cstr] += pheno[i];
			}
		}
		Ttotg = Tsg0 = Tsg1 = Tsg2 = 0.;
		for (j = 0; j < nstra; j++) {
			Ttotg += totg[j];
			Tsg0 += sg0[j];
			Tsg1 += sg1[j];
			Tsg2 += sg2[j];
		}
		chi2[igt + 6 * nsnps] = Ttotg;
		if (Ttotg == 0) {
			chi2[igt] = -999.99;
			chi2[igt + nsnps] = -999.99;
			chi2[igt + 2 * nsnps] = -999.99;
			chi2[igt + 3 * nsnps] = -999.99;
			chi2[igt + 4 * nsnps] = -999.99;
			chi2[igt + 5 * nsnps] = -999.99;
			chi2[igt + 7 * nsnps] = -999.99;
			chi2[igt + 8 * nsnps] = -999.99;
			chi2[igt + 9 * nsnps] = -999.99;
		} else {
			u0 = u1 = u2 = m0 = m1 = m2 = v00 = v02 = v11 = v12 = v22 = 0.;
			for (j = 0; j < nstra; j++)
				if (totg[j] > 0) {
					mx = sumx[j] / totg[j];
					//				if (type == 0)
					bb = (x2[j] / totg[j]) - mx * mx;
					//				else
					//					bb = mx*(1.-mx);
					u0 += (xg0[j] - sg0[j] * mx);
					m0 += xg0[j];
					u1 += (xg1[j] - sg1[j] * mx);
					m1 += xg1[j];
					u2 += (xg2[j] - sg2[j] * mx);
					m2 += xg2[j];
					v00 += bb * (sg0[j] - sg0[j] * sg0[j] / totg[j]);
					v11 += bb * (sg1[j] - sg1[j] * sg1[j] / totg[j]);
					v12 += bb * (0.0 - sg1[j] * sg2[j] / totg[j]);
					v02 += bb * (0.0 - sg0[j] * sg2[j] / totg[j]);
					v22 += bb * (sg2[j] - sg2[j] * sg2[j] / totg[j]);
				}
			if (Tsg0 > 0)
				m0 = m0 / Tsg0;
			else
				m0 = 1.e-16;
			if (Tsg1 > 0)
				m1 = m1 / Tsg1;
			else
				m1 = 1.e-16;
			if (Tsg2 > 0)
				m2 = m2 / Tsg2;
			else
				m2 = 1.e-16;
			u = u1 + 2. * u2;
			v = v11 + 4. * v12 + 4. * v22;
			if (v < 1.e-16) {
				chi2[igt] = -999.99;
				chi2[igt + 3 * nsnps] = -999.99;
			} else {
				chi2[igt] = u * u / v;
				if (type) {
					double p1 = mx + u / (Tsg1 + 4. * Tsg2 - Ttotg * ((Tsg1
							+ 2. * Tsg2) / Ttotg)
	 * ((Tsg1 + 2. * Tsg2) / Ttotg));
					chi2[igt + 3 * nsnps] = (1. - mx) * p1 / ((1. - p1) * mx);
				} else {
					//			  	chi2[igt+3*nsnps]=(Tsg0*(m0-mx)+Tsg1*(m1-mx)+Tsg2*(m2-mx))/Ttotg;
					chi2[igt + 3 * nsnps] = u / (Tsg1 + 4. * Tsg2 - Ttotg
	 * ((Tsg1 + 2. * Tsg2) / Ttotg)
	 * ((Tsg1 + 2. * Tsg2) / Ttotg));
				}
			}
			det = v11 * v22 - v12 * v12;
			//			double rho2 = v12*v12/(v11*v22);
			//			if (v00 <= 0. || v11<=0. || v22<=0. || rho2<1.e-16 || abs(det)<1.e-16) {
			chi2[igt + nsnps] = -999.99;
			chi2[igt + 2 * nsnps] = 1.e-16;
			chi2[igt + 4 * nsnps] = -999.99;
			chi2[igt + 5 * nsnps] = -999.99;
			chi2[igt + 7 * nsnps] = -999.99;
			chi2[igt + 8 * nsnps] = -999.99;
			chi2[igt + 9 * nsnps] = -999.99;
			//			} else {
			if (v00 > 0.) {
				chi2[igt + 7 * nsnps] = u0 / sqrt(v00);
				chi2[igt + nsnps] = u0 * u0 / v00;
			}
			if (v22 > 0.) {
				chi2[igt + 8 * nsnps] = u2 / sqrt(v22);
				chi2[igt + nsnps] += u2 * u2 / v22;
			}
			if (v00 * v22 > 0.) {
				chi2[igt + 9 * nsnps] = v02 / sqrt(v00 * v22);
				chi2[igt + nsnps] += -2. * u0 * u2 * v02 / (v00 * v22);
				chi2[igt + nsnps] = chi2[igt + nsnps] / (1. - v02 * v02 / (v00
	 * v22));
			}
			//				if (v11 > 0. && v22 > 0. && v12 > 0. && rho2<1.)
			//				if (!(v00 <= 0. || v11<=0. || v22<=0. || rho2*rho2<1.e-16 || abs(det)<1.e-16))
			//				HERE IS SOMETHING WRONG -- DO THE SAME AS IN QTSCORE CORRECTION!!!
			//				if (!(v12 <= 0. || v11<=0. || v22<=0. || (rho2*rho2-1.)<1.e-16 || abs(det)<1.e-16))
			//					chi2[igt+nsnps] = (u1*u1/v11 + u2*u2/v22 - 2.*rho2*u1*u2/v12)/(1.-rho2*rho2);
			//				else
			//					chi2[igt+nsnps] = chi2[igt];
			//(u1*u1*v22+u2*u2*v11-2.0*u1*u2*v12)/det;
			if (Tsg1 > 0) {
				if (type) {
					chi2[igt + 4 * nsnps] = (1. - m0) * m1 / ((1. - m1) * m0);
				} else {
					//				 	chi2[igt+4*nsnps]=(u1/Tsg1);
					chi2[igt + 4 * nsnps] = m1 - m0;
				}
			}
			if (Tsg2 > 0) {
				if (type) {
					chi2[igt + 5 * nsnps] = (1. - m0) * m2 / ((1. - m2) * m0);
				} else {
					//			  		chi2[igt+5*nsnps]=u2/Tsg2;
					chi2[igt + 5 * nsnps] = m2 - m0;
				}
			}
			if (Tsg1 > 0 && Tsg2 > 0)
				chi2[igt + 2 * nsnps] = 2.;
			else if (Tsg1 > 0 || Tsg2 > 0)
				chi2[igt + 2 * nsnps] = 1.;
			//			}
		}
	}
}
void qtscore_globWrapper(double *indata, unsigned long int indataSize,
		double *outdata, unsigned long int &outdataNcol,
		unsigned long int &outdataNrow, unsigned int narg, double *argList) {
	if (indata) {
		int power = static_cast<int> (argList[0]);
		// TODO: call function correctly
		qtscore_glob(char *gdata, double *pheno, int *Type, int *Nids, int *Nsnps, int *Nstra, int *stra, double *chi2);
	}
	outdataNcol = 2;
	outdataNrow = indataSize / 3;
}
	 */

#ifdef __cplusplus
}
#endif
