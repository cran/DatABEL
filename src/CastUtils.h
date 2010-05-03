#ifndef __CastUtils__
#define __CastUtils__

template <class DT> void performCast(DT &dest, void*src, int srcType) {
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
    		errorLog << "file contains data of unknown type" << endl << errorExit;
   	}
}

template <class DT> void performCast(void*dest, DT&src, int destType) {
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
    	    errorLog << "file contains data of unknown type" << endl << errorExit;
    	    errorLog << "file contains data of unknown type" << endl << errorExit;
    }
}

void parseStringToArbType(string s, int destType, void *destData);
unsigned short int dataTypeFromString(string type);
string dataTypeToString(int type);
string bufToString(short int dataType, char *data);

#endif
