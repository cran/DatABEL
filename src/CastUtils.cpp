#include <map>
#include <string>

using namespace std;

#include "frutil.h"
#include "CastUtils.h"

void parseStringToArbType(string s, int destType, void *destData) {
    map<int, string> fmt;

    fmt[UNSIGNED_SHORT_INT] = string("%hu");
    fmt[SHORT_INT] = string("%sd");
    fmt[UNSIGNED_INT] = string("%ud");
    fmt[INT] = string("%d");
    fmt[FLOAT] = string("%f");
    fmt[DOUBLE] = string("%lf");

    string format = fmt[destType];

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
