#include <map>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

#include "frutil.h"
#include "CastUtils.h"

unsigned short int UNSIGNED_SHORT_INT_NAN;
short int SHORT_INT_NAN;
unsigned int UNSIGNED_INT_NAN;
int INT_NAN;

int initConsts(){
  sscanf("32767","%hi",&SHORT_INT_NAN);
  sscanf("65535","%hu",&UNSIGNED_SHORT_INT_NAN);
  sscanf("2147483647","%i",&INT_NAN);
  sscanf("4294967295","%u",&UNSIGNED_INT_NAN);
}

int dummy = initConsts();


bool isNan_exact(string s){
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s=="nan";
}

bool isNan_any(string s){
	double out = -999.99;
	int res = sscanf(s.c_str(),"%lf",&out);
	if (res) {
		return false;
	} else {
		return true;
	}
}

void parseStringToArbType(string s, int destType, void *destData, string nanString) {

	map<int, string> fmt;

	fmt[UNSIGNED_SHORT_INT] = string("%hu");
	fmt[SHORT_INT] = string("%hd");
	fmt[UNSIGNED_INT] = string("%u");
	fmt[INT] = string("%d");
	fmt[FLOAT] = string("%f");
	fmt[DOUBLE] = string("%lf");

	string format = fmt[destType];

	int result = sscanf(s.c_str(), format.c_str(), destData);

	if (nanString == s || result !=1){
	    setNan(destData, destType);
		return;
	}
}

unsigned short int dataTypeFromString(string type){
	if (type == "UNSIGNED_SHORT_INT") return 1;
	if (type == "SHORT_INT") return 2;
	if (type == "UNSIGNED_INT") return 3;
	if (type == "INT") return 4;
	if (type == "FLOAT") return 5;
	if (type == "DOUBLE") return 6;
	return 0;
}

string dataTypeToString(int type){
	if (type == 1) return "UNSIGNED_SHORT_INT";
	if (type == 2) return "SHORT_INT";
	if (type == 3) return "UNSIGNED_INT";
	if (type == 4) return "INT";
	if (type == 5) return "FLOAT";
	if (type == 6) return "DOUBLE";
	return 0;
}

string bufToString(short int dataType, char *data){
	char ret[20];
	switch(dataType){
	case UNSIGNED_SHORT_INT:
		sprintf(ret, "%hu", *(unsigned short int*)data);
		break;
	case SHORT_INT:
		sprintf(ret, "%hd", *(short int*)data);
		break;
	case UNSIGNED_INT:
		sprintf(ret, "%u", *(unsigned int*)data);
		break;
	case INT:
		sprintf(ret, "%d", *(int*)data);
		break;
	case FLOAT:
		sprintf(ret, "%f", *(float*)data);
		break;
	case DOUBLE: // changed to "%f" from %lf [not ISO C++]
		sprintf(ret, "%f", *(double*)data);
		break;
	}

	return string(ret);
}

void setNan(unsigned short int &i){setNan(&i, UNSIGNED_SHORT_INT);}
void setNan(short int &i){setNan(&i, SHORT_INT);}
void setNan(unsigned int &i){setNan(&i, UNSIGNED_INT);}
void setNan(int &i){setNan(&i, INT);}
void setNan(float &i){setNan(&i, FLOAT);}
void setNan(double &i){setNan(&i, DOUBLE);}

bool checkNan(unsigned short int i){return checkNan(&i, UNSIGNED_SHORT_INT);}
bool checkNan(short int i){return checkNan(&i, SHORT_INT);}
bool checkNan(unsigned int i){return checkNan(&i, UNSIGNED_INT);}
bool checkNan(int i){return checkNan(&i, INT);}
bool checkNan(float i){return checkNan(&i, FLOAT);}
bool checkNan(double i){return checkNan(&i, DOUBLE);}

void setNan(void *data, int dataType){
    double dZero = 0.;
    float fZero = 0.;
    switch (dataType) {
        case UNSIGNED_SHORT_INT:
    	    (*(unsigned short int*) data) = UNSIGNED_SHORT_INT_NAN;
    		break;
    	case SHORT_INT:
    	    (*(short int*) data) = SHORT_INT_NAN;
    		break;
    	case UNSIGNED_INT:
    	    (*(unsigned int*) data) = UNSIGNED_INT_NAN;
    		break;
    	case INT:
    	    (*(int*) data) = INT_NAN;
    		break;
    	case FLOAT:
    	    (*(float*) data) = fZero/fZero;
    		break;
    	case DOUBLE:
    	    (*(double*) data) = dZero/dZero;
    		break;
    	default:
    		errorLog << "file contains data of unknown type" << endl << errorExit;
   }
}

bool checkNan(void *data, int dataType){
    switch (dataType) {
        case UNSIGNED_SHORT_INT:
    	    return (*(unsigned short int*) data) == UNSIGNED_SHORT_INT_NAN;
    	case SHORT_INT:
    	    return (*(short int*) data) == SHORT_INT_NAN;
    	case UNSIGNED_INT:
    	    return (*(unsigned int*) data) == UNSIGNED_INT_NAN;
    	case INT:
    	    return (*(int*) data) == INT_NAN;
    	case FLOAT:
    	    return isnan(*(float*) data);
    	case DOUBLE:
    	    return isnan(*(double*) data);
    	default:
    		errorLog << "file contains data of unknown type" << endl << errorExit;
   }
}

int getDataType(unsigned short int){return UNSIGNED_SHORT_INT;}
int getDataType(short int){return SHORT_INT;}
int getDataType(unsigned int){return UNSIGNED_INT;}
int getDataType(int){return INT;}
int getDataType(float){return FLOAT;}
int getDataType(double){return DOUBLE;}
