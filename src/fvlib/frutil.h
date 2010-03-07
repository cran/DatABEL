#ifndef __FRUTIL__
#define __FRUTIL__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "frerror.h"
#include "const.h"

using namespace std;

class fixedchar
{
public:
    fixedchar(){};
    fixedchar(string s){strcpy(name,s.c_str());};
	char name[NAMELENGTH];
};

class fr_type 
{
public:
	unsigned short int type;
	// should change that to long!!!
	unsigned int nelements;
	unsigned int nobservations;
	unsigned int nvariables;
	unsigned int bytes_per_record;
	unsigned int bits_per_record;
	unsigned int namelength;
	unsigned int reserved[RESERVEDSPACE];
	
	fr_type() {
		type=0;nelements=nobservations=bits_per_record=bytes_per_record=nvariables=0;
		namelength = NAMELENGTH;
		for (int i=0;i<RESERVEDSPACE;i++) reserved[i]=0;
	}
	~fr_type() {}
	void print() {
		std::cout << "type = " << type << "; nelements = " << nelements << "; nobservations = " 
		          << nobservations << "; nvariables = " << nvariables << "; bits_per_record = " 
		          << bits_per_record << "; bytes_per_record = " << bytes_per_record << "\n";
	}
};

template <class DT>
void make_fake_file(char * fake_filename, unsigned long int nvariables, unsigned long int nobservations, unsigned short int cast_type, DT start = 0, DT increment = 0)
{
	std::ofstream fake_file(fake_filename, std::ios::binary | std::ios::out);
	if (!fake_file) {
		error("can not create fake file %s \n",fake_filename);
	}
	DT out = start;
	fr_type tmp;
	tmp.type = cast_type;
	tmp.nvariables = nvariables; 
	tmp.nobservations = nobservations; 
	tmp.nelements = nvariables*nobservations;
	tmp.bits_per_record = sizeof(DT)*8;
	tmp.bytes_per_record = sizeof(DT);
	fake_file.write((char*)&tmp,sizeof(tmp));
	fixedchar obsname;
	for (unsigned long int i=0;i<nobservations;i++) {
		sprintf(obsname.name,"%lu",i+1);
		fake_file.write((char*)&obsname.name,sizeof(obsname.name));
	}
	for (unsigned long int i=0;i<nvariables;i++) {
		sprintf(obsname.name,"%lu",i+1);
		fake_file.write((char*)&obsname.name,sizeof(obsname.name));
	}

	DT * tmpdat = new DT [tmp.nobservations];
	for (unsigned long int i=0;i< (unsigned long int ) tmp.nvariables;i++) {
		for (unsigned long int j=0; j< (unsigned long int ) tmp.nobservations;j++) tmpdat[j] = (DT) increment * ( i * 
																											(unsigned long int) tmp.nobservations + j );
		fake_file.write((char*)tmpdat,sizeof(DT)*tmp.nobservations);
	}
	fake_file.close();
}

fr_type get_file_type(char * filename);

void initialize_empty_file(string filename, unsigned long int nvariables, unsigned long int nobservations, unsigned short int type, bool override);

string extract_base_file_name(string filename);
bool file_exists(string fileName);
bool headerOrDataExists(string fileName);
unsigned short calcDataSize(unsigned short int type);
void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");

#endif
