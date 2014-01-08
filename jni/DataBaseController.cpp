#include "DataBaseController.h"

DataBaseController::DataBaseController()
{
	const char* const dbname = "test.db";
	int ret = 0;
	ret = sqlite3_open(dbname, &db);
	cout<<"open db sucess"<<endl;
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        
    }
}

DataBaseController::~DataBaseController()
{
	db = 0;
	pErrMsg = 0;
}

int DataBaseController::closeDB()
{
	
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
	cout<< sql<< endl;
	}
	
	
	return MAP;
}