#ifndef __AbstractMatrix__
#define __AbstractMatrix__

#include <string>

using namespace std;

#include "frutil.h"

class AbstractMatrix
{
public:
    virtual ~AbstractMatrix(){};

    template <class DT> void performCast(DT &dest, void*src, int srcType)
    {
	    switch (srcType) {
    	  case UNSIGNED_SHORT_INT:
    		dest = (DT)*((unsigned short int*) src);
    		break;
    	  case SHORT_INT:
    		dest = (DT)*((short int*) src);
    		break;
    	  case UNSIGNED_INT:
    		dest = (DT)*((unsigned int*) src);
    		break;
    	  case INT:
    		dest = (DT)*((int*) src);
    		break;
    	  case FLOAT:
    		dest = (DT)*((float*) src);
    		break;
    	  case DOUBLE:
    		dest = (DT)*((double*) src);
    		break;
    	  default:
    		error("file contains data of unknown type\n");
    	}
    }

    template <class DT> void performCast(void*dest, DT&src, int destType)
    {
        switch (destType) {
    	    case UNSIGNED_SHORT_INT:
    		    *((unsigned short int*)dest) = src;
    		    break;
    	    case SHORT_INT:
    		    *((short int*)dest) = src;
    		    break;
    	    case UNSIGNED_INT:
        		*((unsigned int*)dest) = src;
        		break;
    	    case INT:
    		    *((int*)dest) = src;
    		    break;
    	    case FLOAT:
    		    *((float*)dest) = src;
    		    break;
    	    case DOUBLE:
        		*((double*)dest) = src;
        		break;
    	      default:
    		    error("file contains data of unknown type\n");
    	}
    }

    template <class DT>
    void write_variable_as(unsigned long int nvar, DT * outvec)
    {
        char* tmp = new (nothrow) char [get_nobservations()*getDataSize()];
        if(!tmp)
            error("write_variable_as allocation error");
        for(int i = 0; i< get_nobservations();i++){
            performCast(&tmp[i*getDataSize()],outvec[i],getDataType());
        }
        write_variable(nvar, tmp);
        delete[] tmp;
    }

    template <class DT>
    void add_variable_as(DT * outvec, string varname)
    {
        char* tmp = new (nothrow) char [get_nobservations()*getDataSize()];
        if(!tmp)
            error("add_variable_as allocation error");
        for(int i = 0; i< get_nobservations();i++){
            performCast(&tmp[i*getDataSize()],outvec[i],getDataType());
        }
        add_variable (tmp, varname);
        delete[] tmp;
    }

    template<class DT>
    void read_variable_as(unsigned long int nvar, DT * outvec)
    {
       char * tmp = new (nothrow) char[get_nobservations()*getDataSize()];
       read_variable(nvar, tmp);
       for(int i = 0; i< get_nobservations();i++) {
            performCast(outvec[i],&tmp[i*getDataSize()],getDataType());
       }
       delete[] tmp;
    }

    void read_element_as(unsigned long varNumber, unsigned long obsNumber, float& element){
        char *ret= new char [getDataSize()];
        read_element(varNumber, obsNumber, ret);
        performCast(element, ret, getDataType());
        delete [] ret;
    }

    template <class DT>
    void write_element_as(unsigned long varNumber, unsigned long obsNumber, DT& element){
       char *ret= new char [getDataSize()];
       performCast(ret, element, getDataType());
       write_element(varNumber, obsNumber, ret);
       delete [] ret;
    }

    virtual unsigned int get_nvariables() = 0;
    virtual unsigned int get_nobservations() = 0;

	virtual void save( string new_file_name ) = 0;
  	/*
    * Save specified vars to new file
	*/
	virtual void save_vars( string new_file_name, unsigned long int nvars, unsigned long int * varindexes) = 0;
	/*
    * Save specified observations to new file
  	*/
   	virtual void save_obs( string new_file_name, unsigned long int nobss, unsigned long int * obsindexes) = 0;

    virtual void save(string new_file_name, unsigned long int nvars, unsigned long int nobss, unsigned long int * varindexes, unsigned long int * obsindexes) = 0;
    virtual void save_as_text(string new_file_name, unsigned long int nvars, unsigned long int nobss, unsigned long int * varindexes, unsigned long int * obsindexes) = 0;

    //virtual void add_variable(void * invec, fixedchar varname) = 0; // adds variable at the end = write_variable with nvar=NVARS?
    // loooong future -- control that name is unique!
    virtual void read_observation(unsigned long int nobs, void * outvec) = 0;
    virtual void write_observation(unsigned long int nobs, void * invec) = 0;

    virtual void write_variable_name(unsigned long int nvar, fixedchar newname) = 0;  // todo loooong future -- control that name is unique
    virtual void write_observation_name(unsigned long int nobs, fixedchar newname)= 0;  //todo loooong future -- control that name is unique!

// HIGH -- here I see the possibility to make these functions faster then "random" access functions

    // changing cache size on the fly
    virtual unsigned long int get_cachesizeMb() = 0;
    virtual void set_cachesizeMb( unsigned long int cachesizeMb ) = 0;

    virtual fixedchar read_observation_name(unsigned long int nobs) = 0;
    virtual fixedchar read_variable_name(unsigned long int nvar) = 0;

    virtual void setUpdateNamesOnWrite(bool bUpdate) = 0;

private:
    // can read single variable
	virtual void read_variable(unsigned long int nvar, void * outvec) = 0;

    // should only be used for reading single random elements!
	virtual void read_element(unsigned long int nvar, unsigned long int nobs, void * elem) = 0;

	// write single variable
	virtual void write_variable(unsigned long int nvar, void * datavec) = 0;

    // HIGH -- here I see the possibility to make these functions faster then "random" access functions
    // adds variable at the end = write_variable with nvar=NVARS?
	// todo loooong future -- control that name is unique!
    virtual void add_variable(void * invec, string varname) = 0;
    // todo later
    //    virtual void add_observation(void * invec, string obsname) = 0;

    // write single element
    // CURRENTLY CACHE IS NOT UPDATED!
	virtual void write_element(unsigned long int nvar, unsigned long int nobs, void * data) = 0;


	virtual short unsigned getDataSize() = 0;
	virtual short unsigned getDataType() = 0;
};

#endif


