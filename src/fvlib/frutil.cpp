#include <sys/stat.h>
#include <string>
#include <vector>

#include "frutil.h"
#include "const.h"
#include "filevector.h"

fr_type get_file_type(char * filename) {
	fr_type out;
	ifstream myfile(filename, ios::binary | ios::in);
	if (!myfile) {
		error("can not open file for reading\n");
	}
	myfile.read((char*)&out,sizeof(out));
	return(out);
}

string extract_base_file_name(string filename)
{
    int idxExtPos = filename.find(FILEVECTOR_INDEX_FILE_SUFFIX);
    int dataExtPos = filename.find(FILEVECTOR_DATA_FILE_SUFFIX);
    if (idxExtPos == filename.size() - FILEVECTOR_INDEX_FILE_SUFFIX.size() )
    {
        return filename.substr(0, idxExtPos);
	}
	else if (dataExtPos == filename.size() - FILEVECTOR_DATA_FILE_SUFFIX.size() )
	{
	    return filename.substr(0, dataExtPos );
	}
	else
	{
		return filename;
	}
}

unsigned short calcDataSize(unsigned short int type){
    unsigned short desize;
	switch (type) {
	  case UNSIGNED_SHORT_INT:
		desize = sizeof(unsigned short int);
		break;
	  case SHORT_INT:
		desize = sizeof(short int);
		break;
	  case UNSIGNED_INT:
		desize = sizeof(unsigned int);
		break;
	  case INT:
		desize = sizeof(int);
		break;
	  case FLOAT:
		desize = sizeof(float);
		break;
	  case DOUBLE:
		desize = sizeof(double);
		break;
	  default:
		error("file contains data of unknown type\n");
	}
	return desize;
}

void initialize_empty_file(string filename, unsigned long nvariables, unsigned long nobservations, unsigned short type, bool override)
{
    cout << "Initizlizing empty file '" << filename << "', type " << type << "." << endl;
    string index_filename = filename + FILEVECTOR_INDEX_FILE_SUFFIX;
    string data_filename = filename + FILEVECTOR_DATA_FILE_SUFFIX;

	fr_type metadata;
	unsigned long int desize = calcDataSize(type);
	metadata.type = type;
	metadata.nvariables = nvariables;
	metadata.nobservations = nobservations;
	metadata.nelements = nvariables*nobservations;
	metadata.bytes_per_record = desize;
	metadata.bits_per_record = desize*8;

	bool bHeaderOrDataExists = headerOrDataExists( filename );

	if (override && bHeaderOrDataExists) {
	    unlink(index_filename.c_str());
	    unlink(data_filename.c_str());
	}

	if (!override && bHeaderOrDataExists) {
	    error("File %s already exists.", filename.c_str());
	}

	ofstream idx_file(index_filename.c_str(), ios::binary | ios::out);
	ofstream data_file(data_filename.c_str(), ios::binary | ios::out);

	idx_file.write((char*)&metadata,sizeof(metadata));

	fixedchar obsname;
	for (unsigned long int i=0;i<nobservations;i++) {
		sprintf(obsname.name,"%lu",i+1);
		idx_file.write((char*)&obsname.name,sizeof(obsname.name));
	}
	for (unsigned long int i=0;i<nvariables;i++) {
		sprintf(obsname.name,"%lu",i+1);
		idx_file.write((char*)&obsname.name,sizeof(obsname.name));
	}

	unsigned long int estimated_data_size = (unsigned long int) metadata.bytes_per_record *
										(unsigned long int) metadata.nvariables *
										(unsigned long int) metadata.nobservations;

	data_file.seekp(estimated_data_size -1);
	data_file.put('E');
	data_file.close();
	idx_file.close();
    cout << "File '" << filename << "' initialized."<< endl;
}

bool headerOrDataExists(string fileName) {
    return file_exists(fileName + FILEVECTOR_INDEX_FILE_SUFFIX) || file_exists(fileName + FILEVECTOR_INDEX_FILE_SUFFIX);
}

bool file_exists(string fileName)
{
    struct stat buf;
    int i = stat ( fileName.c_str(), &buf );
     /* File found */
    if ( i == 0 )
    {
        return true;
    }
    return false;
}

void tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

    