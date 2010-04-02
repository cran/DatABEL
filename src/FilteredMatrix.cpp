#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <memory.h>
#include <set>

using namespace std;

#include "FilteredMatrix.h"
#include "frutil.h"

unsigned long FilteredMatrix::getCacheSizeInMb() {
	return getNestedMatrix().getCacheSizeInMb();
}

// calculates filtering map for multiply filtering
void FilteredMatrix::fillUpIndexMap(vector<unsigned long> &v, IndexMap &m, IndexMap &result ){
    set<unsigned long> s;
    IndexMap::iterator i;
    for(i=m.begin();i!=m.end();i++) {
        deepDbg << "fillUpIndexMap: s.insert("<<i->second<<")"<< endl;
        s.insert(i->second);
    }
    unsigned long k;
    for (k=0;k<v.size();k++){
        s.insert(v[k]);
        deepDbg << "fillUpIndexMap: s.insert("<<v[k]<<") (from v)"<< endl;
    }

    k=0;
    set<unsigned long>::iterator j;
    for (j=s.begin();j!=s.end();j++) {
        result[k] = *j;
        deepDbg << "fillUpIndexMap: result.k["<<k<<"] = " << *j << endl;
        k++;
    }
}


void FilteredMatrix::setCacheSizeInMb( unsigned long cachesizeMb ) {
    getNestedMatrix().setCacheSizeInMb(cachesizeMb);
}

void FilteredMatrix::setUpdateNamesOnWrite(bool bUpdate) {
    getNestedMatrix().setUpdateNamesOnWrite(bUpdate);
}

void FilteredMatrix::writeVariableName(unsigned long varIdx, FixedChar name) {
    getNestedMatrix().writeVariableName(filteredToRealRowIdx[varIdx], name);
}

void FilteredMatrix::writeObservationName(unsigned long obsIdx, FixedChar name) {
    getNestedMatrix().writeObservationName(filteredToRealColIdx[obsIdx], name);
}

FixedChar FilteredMatrix::readVariableName(unsigned long varIdx) {
    return getNestedMatrix().readVariableName(filteredToRealRowIdx[varIdx]);
}

FixedChar FilteredMatrix::readObservationName(unsigned long obsIdx) {
    return getNestedMatrix().readObservationName(filteredToRealColIdx[obsIdx]);
}

void FilteredMatrix::readVariable(unsigned long varIdx, void * outvec) {
    unsigned long i;
    for(i=0;i<getNumObservations();i++){
        readElement(varIdx, i, (char*)outvec + i * getElementSize());
    }
}

void FilteredMatrix::readObservation(unsigned long obsIdx, void * outvec) {
    unsigned long i;
    for(i=0;i<getNumVariables();i++){
        readElement( i, obsIdx, (char*)outvec + i * getElementSize());
    }
}

void FilteredMatrix::writeObservation(unsigned long obsIdx, void * invec) {
    unsigned long i;
    for(i=0;i<getNumObservations();i++){
        writeElement( i, obsIdx, (char*)invec + i * getElementSize());
    }
}

void FilteredMatrix::writeVariable(unsigned long varIdx, void *datavec) {
    unsigned long i;
    deepDbg << "FilteredMatrix.writeVariable(" << varIdx << ")" << endl;
    for(i=0;i<getNumObservations();i++){
        writeElement(varIdx, i, (char*)datavec + i * getElementSize());
    }
}

void FilteredMatrix::readElement(unsigned long varIdx, unsigned long obsIdx, void * out) {
    deepDbg << "FilteredMatrix::readElement(" << varIdx << "," << obsIdx<<")";
    getNestedMatrix().readElement(filteredToRealRowIdx[varIdx], filteredToRealColIdx[obsIdx], out);
}

void FilteredMatrix::writeElement(unsigned long varIdx, unsigned long obsIdx, void* data) {
    deepDbg << "FilteredMatrix.writeElement (" << varIdx << "," << obsIdx << ")" << endl;
    getNestedMatrix().writeElement(filteredToRealRowIdx[varIdx], filteredToRealColIdx[obsIdx], data);
}

unsigned long FilteredMatrix::getNumVariables() {
   return filteredToRealRowIdx.size();
}

unsigned long FilteredMatrix::getNumObservations() {
   return filteredToRealColIdx.size();
}

void FilteredMatrix::saveAs(string newFilename) {
    vector<unsigned long> recodedColIndexes;
    vector<unsigned long> recodedRowIndexes;

    map<unsigned long, unsigned long>::iterator i;

    for(i=filteredToRealColIdx.begin(); i!=filteredToRealColIdx.end(); i++) {
        recodedColIndexes.push_back(i->second);
        recodedRowIndexes.push_back(i->second);
    }

    getNestedMatrix().saveAs(newFilename, recodedRowIndexes.size(), recodedColIndexes.size(), &recodedRowIndexes[0], &recodedColIndexes[0]);
}

void FilteredMatrix::saveVariablesAs( string newFilename, unsigned long nvars, unsigned long * varIndexes) {
    vector<unsigned long> recodedIndexes;
    filterIdxList(varIndexes, nvars, recodedIndexes, filteredToRealRowIdx);
    getNestedMatrix().saveVariablesAs( newFilename, nvars, &recodedIndexes[0]);
}

void FilteredMatrix::saveObservationsAs( string newFilename, unsigned long nobss, unsigned long * obsIndexes) {
    vector<unsigned long> recodedIndexes;
    filterIdxList(obsIndexes, nobss, recodedIndexes, filteredToRealColIdx);
    getNestedMatrix().saveObservationsAs(newFilename, nobss, &recodedIndexes[0]);
}

void FilteredMatrix::saveAs(string newFilename, unsigned long nvars, unsigned long nobss, unsigned long *varIndexes, unsigned long *obsIndexes) {
    vector<unsigned long> recodedColIndexes;
    vector<unsigned long> recodedRowIndexes;
    filterIdxList(varIndexes, nobss, recodedColIndexes, filteredToRealColIdx);
    filterIdxList(varIndexes, nvars, recodedRowIndexes, filteredToRealRowIdx);
    getNestedMatrix().saveAs(newFilename, nvars, nobss, &recodedRowIndexes[0], &recodedColIndexes[0]);
}

void FilteredMatrix::saveAsText(string newFilename, unsigned long nvars, unsigned long nobss, unsigned long * varIndexes, unsigned long * obsIndexes) {
    vector<unsigned long> recodedColIndexes;
    vector<unsigned long> recodedRowIndexes;
    filterIdxList(obsIndexes, nobss, recodedColIndexes, filteredToRealColIdx);
    filterIdxList(varIndexes, nvars, recodedRowIndexes, filteredToRealRowIdx);
    getNestedMatrix().saveAsText(newFilename, nvars, nobss, &recodedRowIndexes[0], &recodedColIndexes[0]);
}

short unsigned FilteredMatrix::getElementSize() {
    return getNestedMatrix().getElementSize();
}

short unsigned FilteredMatrix::getElementType() {
    return getNestedMatrix().getElementType();
}

void FilteredMatrix::addVariable(void * invec, string varname) {
    errorLog << "FilteredMatrix doesn't support addVariable." << endl << errorExit;
}

void FilteredMatrix::cacheAllNames(bool doCache) {
    getNestedMatrix().cacheAllNames(doCache);
}





