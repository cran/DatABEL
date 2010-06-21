#ifndef __CastUtils__
#define __CastUtils__

// these copy-pastes are for template support
void setNan(unsigned short int &i);
void setNan(short int &i);
void setNan(unsigned int &i);
void setNan(int &i);
void setNan(float &i);
void setNan(double &i);

bool checkNan(unsigned short int i);
bool checkNan(short int i);
bool checkNan(unsigned int i);
bool checkNan(int i);
bool checkNan(float i);
bool checkNan(double i);

string dataTypeToString(int type);
int getDataType(unsigned short int);
int getDataType(short int);
int getDataType(unsigned int);
int getDataType(int);
int getDataType(float);
int getDataType(double);

bool checkNan(void *data, int dataType);
void setNan(void *data, int dataType);

extern unsigned short int UNSIGNED_SHORT_INT_NAN;
extern short int SHORT_INT_NAN;
extern unsigned int UNSIGNED_INT_NAN;
extern int INT_NAN;

#define COPY_AND_COMPARE(dest,src) dest=src; //if(dest!=src || checkNan(dest) != checkNan(src)) {errorLog << "Loss of precision / loss of data during conversion from " << dataTypeToString(getDataType(src)) << " to " << dataTypeToString(getDataType(dest)) << "." << endl;}
template <class DT> void performCast(DT &dest, void*src, int srcType) {
    if (checkNan(src,srcType)){
        setNan(dest);
        return;
    }
    switch (srcType) {
        case UNSIGNED_SHORT_INT:
    	    COPY_AND_COMPARE(dest, (DT)*((unsigned short int*) src));
    		break;
    	case SHORT_INT:
    		COPY_AND_COMPARE(dest, (DT)*((short int*) src));
    		break;
    	case UNSIGNED_INT:
    		COPY_AND_COMPARE(dest, (DT)*((unsigned int*) src));
    		break;
    	case INT:
    		COPY_AND_COMPARE(dest, (DT)*((int*) src));
    		break;
    	case FLOAT:
    		COPY_AND_COMPARE(dest, (DT)*((float*) src));
    		break;
    	case DOUBLE:
    		COPY_AND_COMPARE(dest, (DT)*((double*) src));
    		break;
    	default:
    		errorLog << "file contains data of unknown type" << endl << errorExit;
   	}
}


template <class DT> void performCast(void*dest, DT&src, int destType) {
    if (checkNan(src)){
        setNan(dest,destType);
        return;
    }
    switch (destType) {
        case UNSIGNED_SHORT_INT:
    	    COPY_AND_COMPARE(*((unsigned short int*)dest), src);
    	    break;
    	case SHORT_INT:
    	    COPY_AND_COMPARE(*((short int*)dest), src);
    	    break;
    	case UNSIGNED_INT:
            COPY_AND_COMPARE(*((unsigned int*)dest), src);
        	break;
    	case INT:
    	    COPY_AND_COMPARE(*((int*)dest), src);
    	    break;
    	case FLOAT:
    	    COPY_AND_COMPARE(*((float*)dest), src);
    	    break;
    	case DOUBLE:
        	COPY_AND_COMPARE(*((double*)dest), src);
        	break;
    	default:
    	    errorLog << "file contains data of unknown type" << endl << errorExit;
    }
}

void parseStringToArbType(string s, int destType, void *destData, string nanString);
unsigned short int dataTypeFromString(string type);
string bufToString(short int dataType, char *data);

#endif
