#ifndef __FilteredMatrix__
#define __FilteredMatrix__

#include <map>

using namespace std;

#include "AbstractMatrix.h"
/*
* "Cols" are observations and "Rows" are variables  
*/
class FilteredMatrix : public AbstractMatrix {
    AbstractMatrix *nestedMatrix;

    vector<unsigned long> filteredToRealColIdx;
    vector<unsigned long> filteredToRealRowIdx;

//    void fillUpIndexMap(vector<unsigned long> &v, IndexMap &m, IndexMap &result );

    // takes iIndexes for input, filters using filterMap and outputs to oIndexes
    void filterIdxList(unsigned long *iIndexes, unsigned long numIndexes,
    vector<unsigned long> &oIndexes, vector<unsigned long> &filter) {
        oIndexes.reserve(numIndexes);

        unsigned long i;
        for(i=0; i<numIndexes; i++) {
            oIndexes.push_back(filter[iIndexes[i]]);
        }
    }
    
public:
    // makes this matrix don't filter any cells
    void setNoFiltering(){
        unsigned long i;
        filteredToRealRowIdx.reserve(nestedMatrix->getNumVariables());
        for(i=0;i<nestedMatrix->getNumVariables();i++) {
            filteredToRealRowIdx.push_back(i);
        }

        filteredToRealColIdx.reserve(nestedMatrix->getNumObservations());
        for(i=0;i<nestedMatrix->getNumObservations();i++) {
            filteredToRealColIdx.push_back(i);
        }
    }
    // set filter for Filterematrix
    void setFilteredArea(vector<unsigned long> &rowMask, vector<unsigned long> &colMask){
        fmDbg << "setFilteredArea()" << endl;
        this->filteredToRealRowIdx = vector<unsigned long>(rowMask);
        this->filteredToRealColIdx = vector<unsigned long>(colMask);
    }

    FilteredMatrix(AbstractMatrix &matrix) : nestedMatrix(&matrix) {
        dbg << "Constructing FilteredMatrix from AbstractMatrix, ptr = " << (long)this << endl;
        setNoFiltering();
    }

    unsigned long getNumVariables();
    unsigned long getNumObservations();
    void cacheAllNames(bool);
    AbstractMatrix* getNestedMatrix() {return nestedMatrix;}

	void saveAs(string newFilename);
	void saveVariablesAs(string newFilename, unsigned long nvars, unsigned long * varindexes);
   	void saveObservationsAs(string newFilename, unsigned long nobss, unsigned long * obsindexes);
    void saveAs(string newFilename, unsigned long nvars, unsigned long nobss, unsigned long * varindexes, unsigned long * obsindexes);
    void saveAsText(string newFilename, unsigned long nvars, unsigned long nobss, unsigned long * varindexes, unsigned long * obsindexes);
    void readObservation(unsigned long obsIdx, void * outvec);
    void writeObservation(unsigned long obsIdx, void * invec);
    void writeVariableName(unsigned long varIdx, FixedChar newname);  // todo loooong future -- control that name is unique
    void writeObservationName(unsigned long obsIdx, FixedChar newname);  //todo loooong future -- control that name is unique!
    unsigned long getCacheSizeInMb();
    void setCacheSizeInMb( unsigned long cachesizeMb );
    FixedChar readObservationName(unsigned long obsIdx);
    FixedChar readVariableName(unsigned long varIdx);
    void setUpdateNamesOnWrite(bool bUpdate);
	short unsigned getElementSize();
	short unsigned getElementType();
	void readVariable(unsigned long varIdx, void * outvec);
	void readElement(unsigned long varIdx, unsigned long obsIdx, void * elem);
	void writeVariable(unsigned long varIdx, void * datavec);
	void writeElement(unsigned long varIdx, unsigned long obsIdx, void * data);
    virtual AbstractMatrix* castToAbstractMatrix();
    virtual bool setReadOnly(bool readOnly);
    
private:
    void addVariable(void * invec, string varname);
};

#endif
