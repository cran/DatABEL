#include <map>
#include <string>
#include <algorithm>

using namespace std;

#include "frutil.h"
#include "CastUtils.h"

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

void parseStringToArbType(string s, int destType, void *destData) {

	map<int, string> fmt;

	fmt[UNSIGNED_SHORT_INT] = string("%hu");
	fmt[SHORT_INT] = string("%hd");
	fmt[UNSIGNED_INT] = string("%u");
	fmt[INT] = string("%d");
	fmt[FLOAT] = string("%f");
	fmt[DOUBLE] = string("%lf");

	string format = fmt[destType];

	if (isNan_any(s)){
		double zero = 0;
		string zeroStr = "0"; // nan for integers
		if (destType == DOUBLE ) {
			*(double*)destData = 0/zero;
		} else if (destType == FLOAT){
			*(float*)destData = 0/zero;
		} else {
			sscanf(zeroStr.c_str(), format.c_str(), destData );
		}
		return;
	}

	sscanf(s.c_str(), format.c_str(), destData);
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
