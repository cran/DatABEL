#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

#include "filevector.h"
#include "Transposer.h"
#include "frerror.h"

#include "convert_util.h"

// old version (246) works with DatABEL

#define REPORT_EVERY 10000
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

void text2fvf_246(
		std::string program_name,
		std::string infilename, std::string outfilename,
		std::string rownamesfilename, std::string colnamesfilename,
		int rncol, int cnrow,
		int skiprows, int skipcols,
		int transpose, int Rmatrix,
		unsigned short int type,
		bool quiet
)
{

	if (!quiet) message("Options in effect:\n");
	if (!quiet) message("\t --infile    = %s\n",infilename.c_str());
	if (!quiet) message("\t --outfile   = %s\n",outfilename.c_str());
	if (!quiet) message("\t --skiprows  = ");
	if (skiprows) if (!quiet) message("%d\n",skiprows); else if (!quiet) message("OFF\n");
	if (!quiet) message("\t --skipcols  = ");
	if (skipcols) if (!quiet) message("%d\n",skipcols); else if (!quiet) message("OFF\n");
	if (!quiet) message("\t --rncol     = ");
	if (rncol) if (!quiet) message("%d\n",rncol); else if (!quiet) message("OFF\n");
	if (!quiet) message("\t --cnrow     = ");
	if (cnrow) if (!quiet) message("%d\n",cnrow); else if (!quiet) message("OFF\n");

	if (colnamesfilename != "") cnrow = -1;
	if (rownamesfilename != "") rncol = -1;
	if (!quiet) message("\t --cnrow  = ");
	if (cnrow) {
		if (colnamesfilename == "")
			if (!quiet) message("ON, using line %d of '%s'\n",cnrow,infilename.c_str());
			else
				if (!quiet) message("ON, using data from file '%s'\n",colnamesfilename.c_str());
	} else if (!quiet) message("OFF\n");

	if (!quiet) message("\t --rncol  = ");
	if (rncol) {
		if (rownamesfilename == "")
			if (!quiet) message("ON, using column %d of '%s'\n",rncol,infilename.c_str());
			else
				if (!quiet) message("ON, using data from file '%s'\n",rownamesfilename.c_str());
	} else if (!quiet) message("OFF\n");

	if (!quiet) message("\t --transpose = ");
	if (transpose) if (!quiet) message("ON\n"); else if (!quiet) message("OFF\n");
	if (!quiet) message("\t --Rmatrix   = ");
	if (Rmatrix) if (!quiet) message("ON\n"); else if (!quiet) message("OFF\n");
	if (!quiet) message("\n");


	// check that it is mentioned how many columns to skip when reading row names
	if (rownamesfilename=="" && rncol && !skipcols)
		error("\n\nPlease tell how many columns to skip when you provide the column containing row names!\n\n");
	if (skipcols && (rncol > skipcols))
		error("rncol > skipcols");
	if (colnamesfilename=="" && cnrow && !skiprows)
		error("\n\nPlease tell how many rows to skip when you provide the row containing column names!\n\n");
	if (skiprows && (cnrow > skiprows))
		error("cnrow > skiprows");


	std::ifstream infile(infilename.c_str());
	if (!infile) error("can not open file '%s' for reading\n\n",infilename.c_str());

	// column names specified in a separate file; check the number of columns
	unsigned long int words_in_colnamesfile = 0;
	if (cnrow & colnamesfilename != "")
	{
		std::ifstream colnamesfile(colnamesfilename.c_str());
		std::string tmpstr;
		if (!colnamesfile) error("can not open column names file '%s'\n\n",colnamesfilename.c_str());
		while (colnamesfile >> tmpstr) words_in_colnamesfile++;
		colnamesfile.close();
		if (!quiet) message("number of names in column-names file '%s' is %d\n\n",colnamesfilename.c_str(),words_in_colnamesfile);
	}

	// row names specified in a separate file; check the number of columns
	unsigned long int words_in_rownamesfile = 0;
	if (rncol & rownamesfilename != "")
	{
		std::ifstream rownamesfile(rownamesfilename.c_str());
		std::string tmpstr;
		if (!rownamesfile) error("can not open row names file '%s'\n\n",rownamesfilename.c_str());
		while (rownamesfile >> tmpstr) words_in_rownamesfile++;
		rownamesfile.close();
		if (!quiet) message("number of names in row-names file '%s' is %d\n\n", rownamesfilename.c_str(), words_in_rownamesfile );
	}

	// find out the number of rows and columns + some checks
	if (!quiet) message("Finding the number of columns and rows in the file + integrity checks ... Line count:\n");
	unsigned long int infile_linecount = 0, line_wordcount = 0;
	long int last_word_count = -1;
	std::string sline, sword;
	while (getline(infile,sline))
	{
		infile_linecount++;
		if ((infile_linecount % REPORT_EVERY) == 0) {if (!quiet) message("\b\b\b\b\b\b\b\b\b\b\b\b%d",infile_linecount);std::cout.flush();}
		std::istringstream stream_sline(sline);
		line_wordcount = 0;
		while (stream_sline >> sword) line_wordcount++;
		//		if (firstline_add_one) {line_wordcount++;firstline_add_one=0;}
		if (last_word_count >=0 && last_word_count != line_wordcount)
			if (!(Rmatrix && (line_wordcount-1) == last_word_count))
				error("file '%s', line %u, number of elements is %u (previous line(s): %u)\n\n",
						infilename.c_str(),infile_linecount,line_wordcount,last_word_count);
		last_word_count = line_wordcount;
	}
	if (!quiet) message("\b\b\b\b\b\b\b\b\b\b\b\b%d",infile_linecount);std::cout.flush();
	if (!quiet) message("\n");
	infile.close();
	if (!quiet) message("file '%s' line count = %d\n",infilename.c_str(),infile_linecount);
	if (!quiet) message("file '%s' column count = %d\n",infilename.c_str(),line_wordcount);
	if ( infile_linecount == 0 || line_wordcount ==0 ) error("file '%s' contains no lines/columns\n\n",infilename.c_str());

	if (cnrow & colnamesfilename != "")
		if ((line_wordcount - skipcols) != words_in_colnamesfile)
			error("number of column names (%lu) does not match number of data columns (%lu)\n\n",
					words_in_colnamesfile,(line_wordcount-skipcols));

	// read the data to memory

	unsigned long int ncols = line_wordcount - skipcols;
	unsigned long int nrows = infile_linecount - skiprows;

	unsigned long int num_data_elements = ncols * nrows;

	// if file is to be transposed we can read one line at a time and write it immediately
	if (transpose) num_data_elements = ncols;

	float * data = new (std::nothrow) float [num_data_elements];

	if (!data) error("can not get RAM for %u column and %u rows matrix\n\n",ncols,nrows);

	infile.open(infilename.c_str());
	if (!infile) error("can not open file '%s' for reading\n\n",infilename.c_str());

	// make empty filevector file
	unsigned long int out_nvars = 0, out_nobs = 0;
	if (transpose) {
		out_nvars = nrows;
		out_nobs = ncols;
	} else {
		out_nvars = ncols;
		out_nobs = nrows;
	}
	// HERE IS THE TYPE
	initialize_empty_file(outfilename, out_nvars, out_nobs, type, true);

	if (!quiet) message("number of variables in FVF-file '%s' will be %d\n",outfilename.c_str(),out_nvars);
	if (!quiet) message("number of observations in FVF-file '%s' will be %d\n\n",outfilename.c_str(),out_nobs);

	AbstractMatrix * outdata = new filevector(outfilename, (unsigned long int) 64); // this is not nice - fixed cache-size of 64 Mb

	fixedchar tmpname;

	// read column names
	if (cnrow)
		if (colnamesfilename=="") {
			int cur_line = 0;
			while (cur_line<skiprows)
			{
				cur_line++;
				getline(infile,sline);
				std::istringstream stream_sline(sline);
				if (cur_line == cnrow) {
					int toCol = ncols;
					if (!Rmatrix) toCol = ncols + skipcols;
					//					Rprintf("toCol=%u\n",toCol);
					for (unsigned long int i=0;i<toCol;i++) {
						stream_sline >> sword;
						if (Rmatrix || i>=skipcols) {
							int idx = i;
							if (!Rmatrix) idx = idx - skipcols;
							//							Rprintf("%u %u %s\n",i,idx,sword.c_str());
							strcpy(tmpname.name,sword.c_str());
							if (transpose)
								outdata->write_observation_name(idx,tmpname);
							else
								outdata->write_variable_name(idx,tmpname);
						}
					}
				}
			}
		} else {
			std::ifstream colnamesfile(colnamesfilename.c_str());
			if (!colnamesfile) error("can not open column names file '%s'\n\n",colnamesfilename.c_str());
			std::string tmpstr;
			for (unsigned long int i=0;i<ncols;i++) {
				colnamesfile >> tmpstr;
				strcpy(tmpname.name,tmpstr.c_str());
				if (transpose)
					outdata->write_observation_name(i,tmpname);
				else
					outdata->write_variable_name(i,tmpname);
			}
			colnamesfile.close();
		}

	// read row names file if present
	if (rncol && rownamesfilename != "")
	{
		std::ifstream rownamesfile(rownamesfilename.c_str());
		if (!rownamesfile) error("can not open row names file '%s'\n\n",rownamesfilename.c_str());
		std::string tmpstr;
		for (unsigned long int i=0;i<nrows;i++) {
			rownamesfile >> tmpstr;
			strcpy(tmpname.name,tmpstr.c_str());
			if (transpose)
				outdata->write_variable_name(i,tmpname);
			else
				outdata->write_observation_name(i,tmpname);
		}
		rownamesfile.close();
	}


	// read the data
	if (transpose)
		if (!quiet) message("reading data to RAM and writing to file '%s' ... Line count:\n",outfilename.c_str());
		else
			if (!quiet) message("reading data to RAM ... Line count:\n");
	unsigned long int j = 0, cline = 0;
	while (getline(infile,sline))
	{
		std::istringstream stream_sline(sline);
		unsigned long int current_word = 0;
		while (stream_sline >> sword) {
			current_word++;
			if (current_word == rncol) {
				strcpy(tmpname.name,sword.c_str());
				if (transpose)
					outdata->write_variable_name(cline,tmpname);
				else
					outdata->write_observation_name(cline,tmpname);
			} else if (current_word > skipcols) {
				sscanf(sword.c_str(),"%f",&data[j++]);
			}
		}
		if (transpose) {
			outdata->write_variable_as(cline,data);
			j=0;
		}
		cline++;
		if ((cline % REPORT_EVERY) == 0) {if (!quiet) message("\b\b\b\b\b\b\b\b\b\b\b\b%d",cline);std::cout.flush();}
	}
	if (!quiet) message("\b\b\b\b\b\b\b\b\b\b\b\b%d",cline);std::cout.flush();
	if (!quiet) message("\n\n");
	infile.close();

	// check that row names are the same length as data
	if (rncol & rownamesfilename != "")
		if ((cline) != words_in_rownamesfile)
			error("number of row names (%lu) does not match number of data rows (%lu)\n\n",
					words_in_rownamesfile,(cline-skiprows));


	// we have to write data from memory...
	if (!transpose) {

		if (!quiet) message("Writing data to FVF-file '%s' ... Writing variable:\n",outfilename.c_str());

		float * tmpdat = new (std::nothrow) float [nrows];

		if (!tmpdat) error("can not get RAM for tmpdat\n\n");
		unsigned long int current_var;
		for (current_var=0;current_var<out_nvars;current_var++)
		{
			if (transpose)
			{
				outdata->write_variable_as(current_var,(data+current_var*ncols));
			} else {
				for (unsigned long int j=0;j<nrows;j++) tmpdat[j] = data[j * ncols + current_var];
				outdata->write_variable_as(current_var,tmpdat);
			}
			if ((current_var+1 % REPORT_EVERY) == 0) {if (!quiet) message("\b\b\b\b\b\b\b\b\b\b\b\b%d",current_var+1);std::cout.flush();}
		}
		if (!quiet) {
			message("\b\b\b\b\b\b\b\b\b\b\b\b%d",(current_var));
			std::cout.flush();
			message("\n\n");
		}

		delete [] tmpdat;

	}
	// from-memory block finished


	// free up the RAM
	delete [] data;
	delete outdata;
	if (!quiet) message("\nFinished successfully text2fvf\n");

}



unsigned long calcNumLines(string fileName){
	ifstream file(fileName.c_str());
	string line;
	unsigned long numlines = 0;
	while(getline(file, line)){
		numlines++;
	}
	return numlines;
}

unsigned long calcNumWordsInFirstLine(string fileName){
	ifstream file(fileName.c_str());
	vector<string> words;
	string line;
	unsigned long numlines = 0;
	getline(file, line);
	tokenize(line, words);
	return words.size();
}

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

void text2fvf(string program_name, string infilename, string outfilename,
		string rownamesfilename, string colnamesfilename, int rncol, int cnrow,
		unsigned long skiprows, unsigned long skipcols, int bTranspose, int Rmatrix,
		unsigned short type, bool quiet) {

	/**
	rncol--;
	cnrow--;

	if (colnamesfilename != "") cnrow = 1;
	if (rownamesfilename != "") rncol = 1;
	**/

	if (Rmatrix) {
		skipcols = skiprows = 1;
		rncol = cnrow = 1;
	}

	if (!quiet) {
		message("Options in effect: \n");
		message("\t --infile    = %s\n",infilename.c_str());
		message("\t --outfile   = %s\n",outfilename.c_str());

		message("\t --skiprows  = ");
		if (skiprows) message("%d\n",skiprows); else message("OFF\n");

		message("\t --skipcols  = ");
		if (skipcols) message("%d\n",skipcols); else message("OFF\n");

		/**
		message("\t --cnrow     = ");
		if (cnrow) message("%d\n",cnrow); else message("OFF\n");

		message("\t --rncol     = ");
		if (rncol) message("%d\n",rncol); else message("OFF\n");
		**/

		message("\t --cnrow     = ");
		if (cnrow || colnamesfilename != "") {
			if (colnamesfilename == "")
				message("ON, using line %d of '%s'\n",cnrow,infilename.c_str());
			else
				message("ON, using data from file '%s'\n",colnamesfilename.c_str());
		} else {
			message("OFF\n");
		}

		message("\t --rncol     = ");

		if (rncol || rownamesfilename != "") {
			if (rownamesfilename == "")
				message("ON, using column %d of '%s'\n",rncol,infilename.c_str());
			else
				message("ON, using data from file '%s'\n",rownamesfilename.c_str());
		} else {
			message("OFF\n");
		}

		message("\t --transpose = ");
		messageOnOff(bTranspose);
		message("\n");
		message("\t --Rmatrix   = ");
		messageOnOff(Rmatrix);
		message("\n");
	}

	const string TMP_SUFFIX = "_fvtmp";
	string realOutFilename = outfilename;

	// if transpose if OFF, do transpose (sic) and write to temp file
	if (!bTranspose){
		outfilename = outfilename + TMP_SUFFIX;
	}

	// check that it is mentioned how many columns to skip when reading row names
	if (rownamesfilename=="" && (rncol>0) && !skipcols) {
		error("\n\nPlease tell how many columns to skip when you provide the column containing row names!\n\n");
	}
	if (skipcols && (rncol > skipcols)) {
		error("rncol > skipcols");
	}
	if (colnamesfilename=="" && (cnrow>0) && !skiprows) {
		error("\n\nPlease tell how many rows to skip when you provide the row containing column names!\n\n");
	}
	if (skiprows && (cnrow > skiprows)) {
		error("cnrow > skiprows");
	}

	ifstream infile(infilename.c_str());
	if (!infile) error("can not open file '%s' for reading\n\n",infilename.c_str());

	vector<string> extColNames;

	// column names specified in a separate file; check the number of columns
	unsigned long int words_in_colnamesfile = 0;

	// reading column names from file
	if (colnamesfilename != "")
	{
		cout << "Reading columns from "  << colnamesfilename << ": ";
		ifstream colnamesfile(colnamesfilename.c_str());
		string tmpstr;
		if (!colnamesfile) {
			error("can not open column names file '%s'\n\n", colnamesfilename.c_str());
		}
		while (colnamesfile >> tmpstr) {
			words_in_colnamesfile++;
			extColNames.push_back(tmpstr);
		}
		colnamesfile.close();
		if (!quiet) message("number of names in column-names file '%s' is %d\n\n",colnamesfilename.c_str(),words_in_colnamesfile);
	}


	vector<string> extRowNames;

	// row names specified in a separate file; check the number of columns
	unsigned long int words_in_rownamesfile = 0;

	// reading row names from file
	if (rownamesfilename != "") {
		ifstream rownamesfile(rownamesfilename.c_str());
		string tmpstr;
		if (!rownamesfile) {
			error("can not open row names file '%s'\n\n", rownamesfilename.c_str());
		}
		while (rownamesfile >> tmpstr) {
			words_in_rownamesfile++;
			extRowNames.push_back(tmpstr);
		}

		rownamesfile.close();

		if (!quiet)
			message("number of names in row-names file '%s' is %d\n\n", rownamesfilename.c_str(), words_in_rownamesfile );
	}

	ifstream srcFile(infilename.c_str());

	string firstLine;

	vector<string> firstLineWords;
	tokenize(firstLine, firstLineWords, " ");

	unsigned long numLines = calcNumLines(infilename);
	unsigned long numWords = calcNumWordsInFirstLine(infilename);

	cout << "Number of lines in source file is " << numLines << endl;
	cout << "Number of words in source file is " << numWords << endl;

	cout << "skiprows = " << skiprows << endl;
	cout << "cnrow = " << cnrow << endl;
	cout << "skipcols = " << skipcols << endl;
	cout << "rncol = " << rncol << endl;
	cout << "Rmatrix = " << Rmatrix << endl;
	cout << "numWords = " << numWords << endl;

	int numRows = numLines - skiprows;
	int numColumns=123;

	bool colNamesFilePresents = (colnamesfilename!="");
	bool rowNamesFilePresents = (rownamesfilename!="");

	/**
	if (Rmatrix) {
		if (colNamesFilePresents && rowNamesFilePresents){
			numColumns = numWords - skipcols;
		}
		if (colNamesFilePresents && !rowNamesFilePresents){
			numColumns = numWords - skipcols;
		}
		if (!colNamesFilePresents && rowNamesFilePresents){
			numColumns = numWords - skipcols;
		}
		if (!colNamesFilePresents && !rowNamesFilePresents){
			numColumns = numWords - skipcols + 1;//edited
		}
	} else {
		if (colNamesFilePresents&&rowNamesFilePresents){
			numColumns = numWords - skipcols; // edited
		}
		if (colNamesFilePresents&&!rowNamesFilePresents){
			numColumns = numWords - skipcols;//edited
		}
		if (!colNamesFilePresents&&rowNamesFilePresents){
			numColumns = numWords - skipcols;//edited
		}
		if (!colNamesFilePresents&&!rowNamesFilePresents){
			numColumns = numWords - skipcols;//edited
		}
	}
	**/

	numColumns = numWords - skipcols;
	if (Rmatrix && !colNamesFilePresents)
		numColumns = numWords - skipcols + 1;

	cout << "Creating file with numRows = " << numRows << endl;
	cout << "Creating file with numColumns = " << numColumns << endl;
	initialize_empty_file(outfilename, numRows, numColumns, type, true );
	filevector *out = new filevector(outfilename, 1);

	string line;
	unsigned long rowCnt = 1;
	unsigned long lineCnt = 1;

	while(getline(srcFile, line)) {
		vector<string> lineWords;
		tokenize(line, lineWords, " ");

		// is this a column name line?
		if (lineCnt == cnrow && !colNamesFilePresents)
		{
			unsigned long i;
			// ignoring R-matrix flag for some reason
			for(i=skipcols-Rmatrix; i<lineWords.size(); i++) {
				extColNames.push_back(lineWords[i]);
			}
			lineCnt++;
			continue;
		}

		if (lineCnt <= skiprows)
		{
			lineCnt++;
			continue;
		}

		unsigned long colCnt = 0;
		unsigned long i;
		for (i=0; i<lineWords.size(); i++) {
			if (i == (rncol-1) && !rowNamesFilePresents) {
				extRowNames.push_back(lineWords[i]);
				continue;
			}

			if (i < skipcols) {
				continue;
			}

			char buf[20] = "01234567";
			parseStringToArbType(lineWords[i], type, buf);
			out->write_element(rowCnt-1, colCnt, buf);
			colCnt++;
		}

		rowCnt++;
		lineCnt++;
	}

	char * tmpstr;
	if (!colNamesFilePresents && cnrow < 0) for (unsigned long int i=1;i<=numColumns;i++) {
		sprintf(tmpstr,"%l",i);
		extColNames.push_back(tmpstr);
	}
	if (!rowNamesFilePresents && cnrow < 0) for (unsigned long int i=1;i<=numRows;i++) {
		sprintf(tmpstr,"%l",i);
		extRowNames.push_back(tmpstr);
	}
	unsigned long i;
	for (i=0;i<extColNames.size();i++){
		out->write_observation_name(i,extColNames[i]);
	}

	for (i=0;i<extRowNames.size();i++){
		out->write_variable_name(i,extRowNames[i]);
	}

	delete out;

	if(!bTranspose)
	{
		cout << "Transposing " << outfilename << " => " << realOutFilename << "." << endl;
		Transposer tr;

		tr.process(outfilename, realOutFilename, true);
	}
	cout << "text2fvf finished." << endl;
}



