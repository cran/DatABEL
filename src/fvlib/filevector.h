#ifndef __FRVECTOR__
#define __FRVECTOR__

#include <new>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

#include "frutil.h"
#include "frerror.h"

#include "AbstractMatrix.h"

using namespace std;

#define DB_CREATE 1
#define DB_EXCL 2
#define DB_RDONLY 4

class filevector: public AbstractMatrix
{
public:
	string data_filename;
	string index_filename;
	fstream data_file;
	fstream index_file;
	fr_type data_type;
	// row and column names
	fixedchar * variable_names;
	fixedchar * observation_names;
	// size of header (descriptives + var/obs names)
	unsigned long int header_size;
	// cache size (Mb) requested by user
	unsigned long int cache_size_Mb;
	// cache size internal; these ones are exact and used internaly
	unsigned long int cache_size_nvars;
	unsigned long int cache_size_bytes;
	unsigned long int max_buffer_size_bytes;
	// which variables are now in cache
	unsigned long int in_cache_from;
	unsigned long int in_cache_to;
	char * cached_data;
	char * char_buffer;

	// prototypes
	filevector();
	~filevector();

	bool readOnly;
	bool updateNamesOnWrite;	

	filevector(string filename_toload, unsigned long int cachesizeMb)
	{
		readOnly = false;
		updateNamesOnWrite = false;
		char_buffer = 0;
		initialize(filename_toload, cachesizeMb);
	}

	filevector(string filename_toload, unsigned long int cachesizeMb, bool iReadOnly) : readOnly(iReadOnly)
    {
		updateNamesOnWrite = false;
		char_buffer = 0;
		initialize(filename_toload, cachesizeMb);
    }

	filevector(char *filename_toload, unsigned long int cachesizeMb)
	{
		updateNamesOnWrite = false;
		readOnly = false;
		string filename(filename_toload);
		char_buffer = 0;
		initialize(filename_toload, cachesizeMb);
	}

	filevector(char *filename_toload, unsigned long int cachesizeMb, bool iReadOnly) : readOnly(iReadOnly)
	{
		updateNamesOnWrite = false;
		string filename(filename_toload);
		char_buffer = 0;
		initialize(filename_toload, cachesizeMb);
	}

	// these ones are the actual used to initialize and free up
	void initialize(string filename_toload, unsigned long int cachesizeMb);
	// this one updates cache
	void update_cache(unsigned long int from_var);
	// gives element number from nvar & nobs
	unsigned long int nrnc_to_nelem(unsigned long int nvar, unsigned long int nobs);

	// getting and setting var/col names
	void write_variable_name(unsigned long int nvar, fixedchar name);
	void write_observation_name(unsigned long int nobs, fixedchar name);

	virtual unsigned int get_nvariables();
	virtual unsigned int get_nobservations();

	fixedchar read_variable_name(unsigned long int nvar);
	fixedchar read_observation_name(unsigned long int nobs);

	// USER FUNCTIONS
	// can read single variable
	void read_variable(unsigned long int nvar, void * outvec);

	void add_variable(void * invec, string varname);

	// should only be used for reading single random elements!
	void read_element(unsigned long int nvar, unsigned long int nobs, void * data);
	// write single variable
	void write_variable(unsigned long int nvar, void * datavec);
	//	void add_variable(DT * invec, fixedchar varname);
	// write single element
	void write_element(unsigned long int nvar, unsigned long int nobs, void * data);

	void read_observation(unsigned long int nobs, void * outvec);
	void write_observation(unsigned long int nobs, void * outvec);

	void save( string new_file_name );
	void save_vars( string new_file_name, unsigned long int nvars, unsigned long int * varindexes);
	void save_obs( string new_file_name, unsigned long int nobss, unsigned long int * obsindexes);
	void save(string new_file_name, unsigned long int nvars, unsigned long int nobss, unsigned long int * varindexes, unsigned long int * obsindexes);
	void save_as_text(string new_file_name, unsigned long int nvars, unsigned long int nobss, unsigned long int * varindexes, unsigned long int * obsindexes);

	unsigned long int get_cachesizeMb();
	void set_cachesizeMb( unsigned long int cachesizeMb );

	virtual short unsigned getDataSize();
	virtual short unsigned getDataType();
	void saveIndexFile();

	virtual void setUpdateNamesOnWrite(bool bUpdate);

	// FOR FUTURE:
	// very slow one!
	//	DT * read_observation(unsigned long int nobs);
	// should only be used for reading single random elements!
	//	DT read_element(unsigned long int nelment);
private :
	void copy_variable(char * to, char * from, int n, unsigned long int * indexes );
	void free_resources();

};

//global variables
const string FILEVECTOR_DATA_FILE_SUFFIX=".fvd";
const string FILEVECTOR_INDEX_FILE_SUFFIX=".fvi";

#endif

