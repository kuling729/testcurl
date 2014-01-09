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
#include <sqlite3.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;




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

//map<int, string> OP
int openDB()
{
	sqlite3 * db = 0;
	char * pErrMsg = 0;

	int ret = 0;
	const char* const dbname = "test.db";
	const char* CREATE_TABLE = "create table OperationDB(id integer primary key autoincrement, devid int, cartag varchar(20), operation int);";
	const char* INSERT_DATA = "insert into OperationDB values(null, 29,'fish',4);";
	const char* QUERY_ALL = "select count(*) from OperationDB;";
	ret = sqlite3_open(dbname, &db);
	
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        return -1;
    }
    cout<<"open db sucess"<<endl;
	// sqlite3_exec(db, CREATE_TABLE, 0, 0, &pErrMsg);
	
	// if ( ret != SQLITE_OK )
 //   	{
 //         printf("SQL error: %s\n", pErrMsg);
 //         sqlite3_free(pErrMsg);
 //    }

	// struct timeval tv;
	// struct timezone tz;
	// gettimeofday (&tv, &tz);
	// long beginTime = tv.tv_usec;
	

	// sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	
	// for(map<int, string>::iterator iter = OP.begin(); iter != OP.end(); ++iter)
	// {
	// 	sqlite3_exec(db, (*iter).second.c_str(), 0, 0, &pErrMsg);
	// }
	
	
	// sqlite3_exec(db, "COMMIT;", 0, 0, &pErrMsg);
	
	// gettimeofday (&tv , &tz);
	// printf("time cost: %d\n",tv.tv_usec-beginTime);

	// sqlite3_exec(db, QUERY_ALL, print_result_cb, 0, &pErrMsg);
	
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
		cout<<"download time: "<<count<<endl;
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
		
		DataBaseController dbc;
		
		//map<int, string> SQLs = dbc.generateSQL(vOperations);
		dbc.syncDB(vOperations);
		remove("/data/c.xml");
		cout<<"delete file"<<endl;
		

	}
	
	gettimeofday (&end , NULL);
	unsigned long cost = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	cout<< "cost:"<<cost<<" us"<<endl;

	

	
	 
	
	return 0;
}
