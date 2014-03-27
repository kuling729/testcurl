#include "DataBaseController.h"

using std::string;


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

int getDBdataString(void* data, int n_columns, char** column_values,
	char** column_names)
{
	int i;
	string all; 
	for(i = 0; i<n_columns; ++i)
	{
		// printf("%s", column_values[i]);
		all = all+column_values[i];
		
	}
	cout<<all<<endl;
	return 0;
}

DataBaseController::DataBaseController()
{
	const char* const dbname = "test.db";
	const char* CREATE_TABLE = "create table cartag_operation(id integer primary key autoincrement, devId int, cartag varchar(20), operation int);";
	const char* QUERY_ALL = "select * from cartag_operation limit 5;";
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
		pErrMsg = NULL;
    }
    
}

DataBaseController::~DataBaseController()
{
	cout<<"destroy DataBaseController "<<endl;
	sqlite3_free(pErrMsg);
	sqlite3_close(db);
	db = 0;
	pErrMsg = 0;
}

//异或校验
void XORValid(const char* buffer, int len)
{
    char checksum = 0, cr = 13, ln = 10;
    char ch1,c1; //校验位的高四位和低四位
    for (int i = 0;  i<len;  i++)
    {
        checksum = checksum ^ buffer[i+1]; //进行异或交验取值
    }

    ch1 = (checksum  >> 4) & 0x0F;  //取高位数；
    c1 = checksum & 0x0F;  //取低位数；

    if (ch1 < 10) //低于10的数
        ch1 = ch1  +  '0';
    else
        ch1 = (ch1 - 10 ) +  'A'; //不低于10的16进制数，如：A、B、C、D、E、F

    if (c1 < 10)
        c1 = c1  +  '0';
    else
        c1 = (c1 - 10 )+  'A';

    printf("%c, %c\n",ch1, c1);
}

void calcCheckout(const string SQLs)
{
	char checksum = 0, cr = 13, ln = 10;
	char Hch, Lch;
	string::size_type len = SQLs.size();
	for(string::size_type i = 0; i < len; ++i)
	{
		checksum = checksum ^ SQLs[i+1];
	}

	if (Hch < 10) 
        Hch = Hch  +  '0';
    else
        Hch = (Hch - 10 ) +  'A'; 

    if (Lch < 10)
        Lch = Lch  +  '0';
    else
        Lch = (Lch - 10 )+  'A';

    cout<< "H: "<< Hch<< " L: "<<Lch <<endl;
}

string makeSQL(vector< map<string, string> > OP)
{
	
	string sql("insert into cartag_operation values");
	
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
	string str1("insert into cartag_operation values(null,");
	map<int, string> MAP;
	int i=0;
	for(vector< map<string, string> >::iterator iter = OP.begin(); iter != OP.end(); iter++)
	{
		string sql = str1+(*iter)["devid"]+",'"+(*iter)["cartag"]+"',"+(*iter)["operation"]+");";
		MAP[i++] = sql;
	}
	
	cout<< "finish generateSQL"<< endl;
	
	return MAP;
}




int DataBaseController::syncDB(vector< map<string, string> > OP)
{
	int ret = 0;
	const char* QUERY_ALL = "SELECT * FROM cartag_operation ORDER BY id DESC limit 5;";
	const char* test = "粤A1234";
	unsigned long cost;
	//map<int, string> SQLs = generateSQL(OP);
	string SQLs = makeSQL(OP);
	sqlite3_exec(db, QUERY_ALL, getDBdataString, 0, &pErrMsg);

	struct timeval start;
	struct timeval end;
	gettimeofday (&start, NULL);

	sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	//for(map<int, string>::iterator iter = SQLs.begin(); iter != SQLs.end(); ++iter)
	{
		sqlite3_exec(db, SQLs.c_str(), 0, 0, &pErrMsg);
	}
	ret = sqlite3_exec(db, "COMMIT;", 0, 0, &pErrMsg);
	if(SQLITE_OK != ret)
	{

	}

	gettimeofday (&end , NULL);
	cost = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	cout<< "sync db cost:"<<cost<<" us"<<endl;

	sqlite3_exec(db, QUERY_ALL, print_result_cb, 0, &pErrMsg);
	//sqlite3_exec(db, "DELETE FROM cartag_operation;", 0, 0, &pErrMsg);
	sqlite3_exec(db, "SELECT count(*) FROM cartag_operation;", print_result_cb, 0, &pErrMsg);
	sqlite3_free(pErrMsg);
	pErrMsg = NULL;
	return 0;
	
}