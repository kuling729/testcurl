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



#include "tinyxml/tinyxml.h"
#include "curl/curl.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sqlite3.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;
using std::vector;


//全局变量  
bool resumeDownload = false;        //是否需要下载的标记位  
double downloadFileLenth = 0;         //需要下载的总大小, 远程文件的大小 

/* 得到远程文件的大小, 要下载的文件大小 */  
double getDownloadFileLenth(const string URL){  
    double downloadFileLenth = 0;  
	const char* url = URL.c_str();
    CURL *handle = curl_easy_init();  
    curl_easy_setopt(handle, CURLOPT_URL, url);  
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头  
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body  
    if (curl_easy_perform(handle) == CURLE_OK) {  
        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);  
    } else {  
        downloadFileLenth = -1;  
    }  
    return downloadFileLenth;  
} 


//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//
int parseXMLString(string XML)
{

	static const char* xml = XML.c_str();
	TiXmlElement* pCurrentElement = NULL;
	vector< map<string, string> > vOperations;
	
	
	

	TiXmlDocument doc;
	doc.Parse( xml );


	pCurrentElement = doc.FirstChildElement( "Lists" )->FirstChildElement("OP");
	while(pCurrentElement)
	{	
		string content;
		map<string, string> OneOperation;
		content = pCurrentElement->FirstChildElement("devid")->GetText();
		OneOperation["devid"] = content;
		content = pCurrentElement->FirstChildElement("cartag")->GetText();
		OneOperation["cartag"] = content;
		content = pCurrentElement->FirstChildElement("operation")->GetText();
		OneOperation["operation"] = content;
		
		vOperations.push_back(OneOperation);
		pCurrentElement = pCurrentElement->NextSiblingElement();
	}
	
	for(vector< map<string, string> >::iterator iter = vOperations.begin(); iter != vOperations.end(); iter++)
	{
	cout<< (*iter)["cartag"] << endl;
		
	}
	
	if ( pCurrentElement )  
    {  
        delete pCurrentElement;  
        pCurrentElement = NULL;  
    }
	
	
	return 1;
}

static int Post_Response(void *data, int size, int nmemb, std::string &content)
{
	long sizes = size * nmemb;
	string temp((char*)data,sizes);
	content += temp;
	return sizes;
}

string GetUrl(const string URL)
{
	CURL *curl; 
    CURLcode res;
	string content;
	
	double downloadFileLenth = 0;
	int timeout = 30;
	
	const char* url = URL.c_str();
    curl_global_init(CURL_GLOBAL_ALL);
 
    curl = curl_easy_init();
    if (curl) {     
		
        curl_easy_setopt(curl, CURLOPT_URL, url); 
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);  
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Post_Response);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
        res = curl_easy_perform(curl); 
        if (res == CURLE_OK) {    
			
			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);  
			        
        }
		else
		{
			downloadFileLenth = -1;
            printf("curl error: %d\n", res); 
		}
         
        curl_easy_cleanup(curl);   
			
    }   
	cout<< "this is from string:"<<downloadFileLenth<< endl;
	cout<< content<<endl;
    curl_global_cleanup();
	
	// FILE *fp = NULL;
	// fp = fopen("/mnt/sdcard/c.xml","wb+");
	// cout<<"open file"<<endl;
	// fwrite(content.c_str(), 1, downloadFileLenth, fp);
	// cout<<"write file"<<endl;	
	// fclose(fp);	
	return content;
}

static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)
{
    int i;
	printf("callback, argc = %d\n",argc);
    for ( i=0; i < argc; i++ )
    {
        printf( "%s = %s\n", *szColName, argv == 0 ? "NUL" : *argv );
    }
    return 0;
}

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

string makeSQL()
{

	return "hello";
}

int openDB()
{
	sqlite3 * db = 0;
	char * pErrMsg = 0;

	int ret = 0;
	const char* const dbname = "test.db";
	const char* CHECK_TABLE_EXIST = "select count(*) from sqlite_master where table='OperationDB';";
	const char* CREATE_TABLE = "create table OperationDB(id integer primary key autoincrement, cartag varchar(20) ,operation int,devid int);";
	const char* INSERT_DATA = "insert into OperationDB values(null, 'fish',29,'2009-7-16');";
	const char* QUERY_ALL = "select * from OperationDB where cartag='fish' limit 5;";
	ret = sqlite3_open(dbname, &db);
	cout<<"open db sucess"<<endl;
	if ( ret != SQLITE_OK )
	{	
		cout << "open error:"<<sqlite3_errmsg(db)<<endl;
        return -1;
    }

	//sqlite3_exec(db, CHECK_TABLE_EXIST, 0, 0, &pErrMsg);
	sqlite3_exec(db, CREATE_TABLE, 0, 0, &pErrMsg);
	
	if ( ret != SQLITE_OK )
     {
         //fprintf(stderr, "SQL error: %s\n", pErrMsg);
         printf("SQL error: %s\n", pErrMsg);
         sqlite3_free(pErrMsg);
     }

	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv, &tz);
	long beginTime = tv.tv_usec;
	
	sqlite3_exec(db, "BEGIN;", 0, 0, &pErrMsg);
	for(int i = 0;i<500;i++)
	{
		sqlite3_exec(db, INSERT_DATA, 0, 0, &pErrMsg);
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
	const string URL = "http://192.168.0.41:8888/testServlet/services/second.do?method=getUpdate";
	downloadFileLenth = getDownloadFileLenth(URL);
	cout<< "filelenth: "<<downloadFileLenth<<endl;
	GetUrl(URL);
	openDB();
	//parseXMLString(GetUrl(URL));
	
	//
	// We start with the 'demoStart' todo list. Process it. And
	// should hopefully end up with the todo list as illustrated.
	//
	return 0;
}
