#ifndef __DataBaseController__
#define __DataBaseController__
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sqlite3.h>
using namespace std;

class DataBaseController{

	public:
		DataBaseController();
		~DataBaseController();
		int openDB();
		int closeDB();
		int syncDB(vector< map<string, string> > OP);
	private:
		map<int, string> generateSQL(vector< map<string, string> > OP);
		sqlite3 * db;
		char * pErrMsg;
		
};

#endif