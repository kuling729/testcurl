#include "DataBaseController.h"


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

DataBaseController::DataBaseController()
{
	const char* const dbname = "test.db";
	const char* CREATE_TABLE = "create table OperationDB(id integer primary key autoincrement, devid int, cartag varchar(20), operation int);";
	const char* QUERY_ALL = "select count(*) from OperationDB;";
	int ret = 0;
	ret = sqlite3_open(dbname, &db);
	cout<<"create DataBaseController"<<endl;
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        
    }
    sqlite3_exec(db, CREATE_TABLE, 0, 0, &pErrMsg);
	
	if ( ret != SQLITE_OK )
   	{
         printf("SQL error: %s\n", pErrMsg);
         sqlite3_free(pErrMsg);
    }
    
}

DataBaseController::~DataBaseController()
{
	cout<<"destroy DataBaseController "<<endl;
	sqlite3_close(db);
	db = 0;
	pErrMsg = 0;
}


string makeSQL(vector< map<string, string> > OP)
{
	
	string sql("insert into OperationDB values");
	
	
	
	for(vector< map<string, string> >::iterator iter = OP.begin(); iter != OP.end(); iter++)
	{
	 sql += "(null,"+(*iter)["devid"]+",'"+(*iter)["cartag"]+"',"+(*iter)["operation"]+"),";
	
	}
	sql.replace(sql.size()-1,1,";");
	//cout<<sql<<endl;
	return sql;
}

map<int, string> DataBaseController::generateSQL(vector< map<string, string> > OP)
{
	string str1("insert into OperationDB values(null,");
	map<int, string> MAP;
	int i=0;
	for(vector< map<string, string> >::iterator iter = OP.begin(); iter != OP.end(); iter++)
	{
	string sql = str1+(*iter)["devid"]+",'"+(*iter)["cartag"]+"',"+(*iter)["operation"]+");";
	MAP[i++] = sql;
	//cout<< sql<< endl;
	}
	
	cout<< "finish generateSQL"<< endl;
	
	return MAP;
}

int DataBaseController::syncDB(vector< map<string, string> > OP)
{
	const char* QUERY_ALL = "select count(*) from OperationDB;";
	unsigned long cost;
	map<int, string> SQLs = generateSQL(OP);
	sqlite3_exec(db, QUERY_ALL, print_result_cb, 0, &pErrMsg);

	struct timeval start;
	struct timeval end;
	gettimeofday (&start, NULL);

	sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	
	for(map<int, string>::iterator iter = SQLs.begin(); iter != SQLs.end(); ++iter)
	{
		sqlite3_exec(db, (*iter).second.c_str(), 0, 0, &pErrMsg);
	}
	
	
	sqlite3_exec(db, "COMMIT;", 0, 0, &pErrMsg);

	gettimeofday (&end , NULL);
	cost = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	cout<< "sync db 1 cost:"<<cost<<" us"<<endl;


	string sqls = makeSQL(OP);
	gettimeofday (&start, NULL);
	sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	sqlite3_exec(db, sqls.c_str(), 0, 0, &pErrMsg);
	sqlite3_exec(db, "COMMIT;", 0, 0, &pErrMsg);
	gettimeofday (&end , NULL);
	cost = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	cout<< "sync db 2 cost:"<<cost<<" us"<<endl;

	sqlite3_exec(db, QUERY_ALL, print_result_cb, 0, &pErrMsg);

	return 0;
	
}