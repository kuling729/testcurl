/*
   Test program for TinyXML.
*/


#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif


#include "xmlController.h"
#include "downloadController.h"
#include "DataBaseController.h"
// #include "tinyxml/tinyxml.h"
// #include "curl/curl.h"
// #include <string>
// #include <iostream>
// #include <vector>
// #include <map>
#include <sqlite3.h>
#include <sys/time.h>
// #include <sys/stat.h> 
#include <unistd.h>
#include <pthread.h>

using namespace std;
using std::vector;











void print_row(int n_values, char** values)
{
	int i;
	for (i = 0; i < n_values; ++i) {
		if (i > 0) {
		printf("\t");
		}
	printf("%s", values[i]);

	}
	printf("\n");
}

int print_result_cb(void* data, int n_columns, char** column_values,
	char** column_names)
{
	static int column_names_printed = 0;
	int i;
	if (!column_names_printed) {
		print_row(n_columns, column_names);
		column_names_printed = 1;
	}
	print_row(n_columns, column_values);
	return 0;
}

map<int, string> generateSQL(vector< map<string, string> > OP)
{
	string str1("insert into OperationDB values(null,");
	map<int, string> MAP;
	int i=0;
	for(vector< map<string, string> >::iterator iter = OP.begin(); iter != OP.end(); iter++)
	{
	string sql = str1+(*iter)["devid"]+",'"+(*iter)["cartag"]+"',"+(*iter)["operation"]+");";
	MAP[i++] = sql;
	cout<< sql<< endl;
	}
	
	
	return MAP;
}

int openDB(map<int, string> OP)
{
	sqlite3 * db = 0;
	char * pErrMsg = 0;

	int ret = 0;
	const char* const dbname = "test.db";
	const char* CREATE_TABLE = "create table OperationDB(id integer primary key autoincrement, devid int, cartag varchar(20), operation int);";
	const char* INSERT_DATA = "insert into OperationDB values(null, 29,'fish',4);";
	const char* QUERY_ALL = "select * from OperationDB;";
	ret = sqlite3_open(dbname, &db);
	cout<<"open db sucess"<<endl;
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        return -1;
    }

	sqlite3_exec(db, CREATE_TABLE, 0, 0, &pErrMsg);
	
	if ( ret != SQLITE_OK )
   	{
         printf("SQL error: %s\n", pErrMsg);
         sqlite3_free(pErrMsg);
    }

	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv, &tz);
	long beginTime = tv.tv_usec;
	

	sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	
	for(map<int, string>::iterator iter = OP.begin(); iter != OP.end(); ++iter)
	{
		sqlite3_exec(db, (*iter).second.c_str(), 0, 0, &pErrMsg);
	}
	
	
	sqlite3_exec(db, "COMMIT;", 0, 0, &pErrMsg);
	
	gettimeofday (&tv , &tz);
	printf("time cost: %d\n",tv.tv_usec-beginTime);

	sqlite3_exec(db, QUERY_ALL, print_result_cb, 0, &pErrMsg);
	
	sqlite3_close(db);
	db = 0;
	return 0;
}

int main()
{

	downloadController dc;
	dc.getDownloadFileLenth();
	
	int times = 605;
	int count = 0;
	bool status = false;
	const string filename = "/data/c.xml";

	struct timeval start;
	struct timeval end;
	gettimeofday (&start, NULL);
	
	
	while(count++ < times)
	{
		cout<<"time: "<<count<<endl;
		status = dc.downloadToFile(filename);
		if(status == true)
			break;
		sleep(500);
	}

	dc.resumeDownload = false;
	if(status == true)
	{
		xmlController c("/data/c.xml");
		vector< map<string, string> > vOperations = c.parseFile();
		remove("/data/c.xml");

		DataBaseController dbc;
		
		map<int, string> MAP = dbc.generateSQL(vOperations);
		for(map<int, string>::iterator iter = MAP.begin(); iter != MAP.end(); ++iter)
		{
			cout<< (*iter).first<< "  "<< (*iter).second<<endl;
		}
		
	}
	
	gettimeofday (&end , NULL);
	unsigned long cost = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	cout<< "cost:"<<cost<<endl;

	

	// map<int, string> MAP = generateSQL(OP);
	// for(map<int, string>::iterator iter = MAP.begin(); iter != MAP.end(); ++iter)
	// {
	// 	cout<< (*iter).first<< "  "<< (*iter).second<<endl;
	// }
	// openDB(MAP);
	
	return 0;
}
