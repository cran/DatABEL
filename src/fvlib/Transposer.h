#ifndef ___TRANSPOSE___
#define ___TRANSPOSE___

#include "filevector.h"

/*
this class transposes filevector files without reading them fully into memory.
data copying is working as explained below:
 some square_size value is used for partial copy.
 the square window traverses source data, ( cycle with i/j )
 the piece of data is read into memory,
 then it's being transposed
 and written to the destination file ( j/i )
*/

class Transposer
{
public :
Transposer(){ square_size =10000;};
Transposer(int opt_square_size){ square_size=opt_square_size;};

void process(string filename, string destFileName, bool forceOverwrite);
void process(string filename);

void write_var_obs_names(filevector * src_fv, filevector * dest_fv);

void copy_data(string src_data_file_name,string dest_data_file_name, unsigned long int src_nvars,
unsigned long int src_nobss, unsigned int data_size);

void read_part(ifstream * src_stream, char * data_part, unsigned long int obs_start , unsigned long int obs_length,
unsigned long int var_start, unsigned long int var_length , unsigned int  data_size,unsigned long int src_obs_length );

void write_part(ofstream * src_stream, char * data_part_transposed, unsigned long int obs_start , unsigned long int obs_length,
unsigned long int var_start, unsigned long int var_length , unsigned int  data_size, unsigned long int dest_obs_length );

void transpose_part(void * data_part, void * data_part_transposed,
unsigned long int obs_length,unsigned long int var_length, unsigned int data_size );

private:
//number of elements in square side. square_size^2 elements will be processed at one time:read and write
unsigned int square_size;

};

#endif