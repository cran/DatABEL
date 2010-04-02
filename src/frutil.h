#ifndef __FRUTIL__
#define __FRUTIL__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "frerror.h"
#include "const.h"
#include "Logger.h"
#include "CastUtils.h"

using namespace std;

class FixedChar
{
public:
    FixedChar(){
        memset(name,0xab,NAMELENGTH);
    };
    FixedChar(string s){strcpy(name,s.c_str());};
	char name[NAMELENGTH];
};

class FileHeader
{
public:
	unsigned short int type;
	// should change that to long!!!
	unsigned int nelements;
	unsigned int numObservations;
	unsigned int numVariables;
	unsigned int bytesPerRecord;
	unsigned int bitsPerRecord;
	unsigned int namelength;
	unsigned int reserved[RESERVEDSPACE];
	
    FileHeader () {
		type=0;
		nelements=0;
		numObservations=0;
		bitsPerRecord=0;
		bytesPerRecord=0;
		numVariables=0;
		namelength = NAMELENGTH;
		for (int i=0;i<RESERVEDSPACE;i++) reserved[i]=0;
	}
	~FileHeader() {}

	void print() {
		dbg << "type = " << type << "("<< dataTypeToString(type) << ")"<< endl;
		dbg << "nelements = " << nelements << endl;
		dbg << "numObservations = " << numObservations << endl;
		dbg << "numVariables = " << numVariables << ";" << endl;
		dbg << "bytesPerRecord = " << bytesPerRecord << ";" << endl;
		dbg << "bitsPerRecord = " << bitsPerRecord << ";" << endl;
	}
};

template <class DT>
void make_fake_file(char * fake_filename, unsigned long numVariables, unsigned long nobservations, unsigned short int cast_type, DT start = 0, DT increment = 0)
{
	ofstream fake_file(fake_filename, ios::binary | ios::out);
	if (!fake_file) {
		errorLog << "can not create fake file %s " << endl << fake_filename << errorExit;
	}
	DT out = start;
	FileHeader tmp;
	tmp.type = cast_type;
	tmp.numVariables = numVariables;
	tmp.numObservations = nobservations;
	tmp.nelements = numVariables*nobservations;
	tmp.bitsPerRecord = sizeof(DT)*8;
	tmp.bytesPerRecord = sizeof(DT);
	fake_file.write((char*)&tmp,sizeof(tmp));
	FixedChar obsname;
	for (unsigned long i=0;i<nobservations;i++) {
		sprintf(obsname.name,"%lu",i+1);
		fake_file.write((char*)&obsname.name,sizeof(obsname.name));
	}
	for (unsigned long i=0;i<numVariables;i++) {
		sprintf(obsname.name,"%lu",i+1);
		fake_file.write((char*)&obsname.name,sizeof(obsname.name));
	}

	DT * tmpdat = new DT [tmp.numObservations];
	for (unsigned long i=0;i< (unsigned long ) tmp.numVariables;i++) {
		for (unsigned long j=0; j< (unsigned long ) tmp.numObservations;j++) tmpdat[j] = (DT) increment * ( i *
																											(unsigned long) tmp.numObservations + j );
		fake_file.write((char*)tmpdat,sizeof(DT)*tmp.numObservations);
	}
	fake_file.close();
}

FileHeader get_file_type(char * filename);

void initializeEmptyFile(string filename, unsigned long numVariables, unsigned long nobservations, unsigned short int type, bool override);

string extract_base_file_name(string filename);
bool file_exists(string fileName);
bool headerOrDataExists(string fileName);
unsigned short calcDataSize(unsigned short int type);
void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");

#endif
