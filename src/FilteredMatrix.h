#ifndef __FilteredMatrix__
#define __FilteredMatrix__

#include <map>

using namespace std;

#include "AbstractMatrix.h"

typedef map<unsigned long, unsigned long> IndexMap;

class FilteredMatrix : public AbstractMatrix {
    AbstractMatrix *nestedMatrix;
    FilteredMatrix *nestedFilteredMatrix;
    bool nestedMatrixIsFiltered;
    
    IndexMap filteredToRealColIdx;
    IndexMap filteredToRealRowIdx;

    void fillUpIndexMap(vector<unsigned long> &v, IndexMap &m, IndexMap &result );

    void filterIdxList(unsigned long *iIndexes, unsigned long numIndexes, vector<unsigned long> &oIndexes, map<unsigned long, unsigned long> &filterMap) {
        oIndexes.reserve(filteredToRealColIdx.size());

        unsigned long i;
        for(i=0; i<numIndexes; i++) {
            oIndexes.push_back(filterMap[iIndexes[i]]);
        }
    }
    
public:
    IndexMap &getFilteredToRealColMap() {
        return filteredToRealColIdx;
    }
    IndexMap &getFilteredToRealRowMap() {
        return filteredToRealRowIdx;
    }

    // makes this matrix don't filter any cells
    void setNoFiltering(){
        unsigned long i;
        filteredToRealRowIdx = IndexMap();
        for(i=0;i<nestedMatrix->getNumVariables();i++) {
            filteredToRealRowIdx[i]=i;
        }

        filteredToRealColIdx = IndexMap();
        for(i=0;i<nestedMatrix->getNumObservations();i++) {
            filteredToRealColIdx[i]=i;
        }
    }

    void setFilters(vector<unsigned long> &rowMask, vector<unsigned long> &colMask){
        filteredToRealRowIdx = IndexMap();
        filteredToRealColIdx = IndexMap();
        if (nestedMatrixIsFiltered) {
            fillUpIndexMap(rowMask,nestedFilteredMatrix->getFilteredToRealRowMap(),filteredToRealRowIdx);
            fillUpIndexMap(colMask,nestedFilteredMatrix->getFilteredToRealColMap(),filteredToRealColIdx);
        } else {
            unsigned long realColIdx;
            for (realColIdx=0; realColIdx<colMask.size(); realColIdx++) {
                filteredToRealColIdx[realColIdx] = colMask[realColIdx];
            }

            unsigned long realRowIdx;
            for (realRowIdx=0; realRowIdx<rowMask.size(); realRowIdx++) {
                filteredToRealRowIdx[realRowIdx] = rowMask[realRowIdx];
            }
        }
    }

    FilteredMatrix(AbstractMatrix &matrix) : nestedMatrix(&matrix), nestedMatrixIsFiltered (false) { }

    FilteredMatrix(FilteredMatrix& fm ): nestedFilteredMatrix(&fm), nestedMatrixIsFiltered (true) { }

    AbstractMatrix &getNestedMatrix() {
        return nestedMatrixIsFiltered?nestedFilteredMatrix->getNestedMatrix():*nestedMatrix;
    }

    unsigned long getNumVariables();
    unsigned long getNumObservations();
    void cacheAllNames(bool);

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

private:
    void addVariable(void * invec, string varname);
};

#endif