#include <iostream>
#include <sqlite3.h>
using namespace std;

class DataBaseController{

	public:
		DataBaseController();
		~DataBaseController();
		int openDB();
		int closeDB();
		int syncDB();
	private:
		sqlite3 * db = 0;
		char * pErrMsg = 0;
		
};

DataBaseController::DataBaseController()
{
	const char* const dbname = "test.db";
	int ret = 0;
	ret = sqlite3_open(dbname, &db);
	cout<<"open db sucess"<<endl;
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        return -1;
    }
}

int DataBaseController::closeDB()
{
	
}
