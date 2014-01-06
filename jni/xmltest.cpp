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
#include <sys/time.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <pthread.h>

using namespace std;
using std::vector;


//全局变量  
bool resumeDownload = false;        //是否需要下载的标记位  
double downloadFileLenth = 0;         //需要下载的总大小, 远程文件的大小 

/* 得到远程文件的大小, 要下载的文件大小 */  
double getDownloadFileLenth(const string URL){  
    downloadFileLenth = 0;  
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

long int getLocalFileLenth(const char* filePath)
{
	if (!resumeDownload){  
        return 0;  
    }  
    FILE *fp=fopen(filePath,"r");
    
    long int fileLength=fseek(fp,0L,SEEK_END);
    
   
    fclose(fp);
    return fileLength;
}

unsigned long get_file_size(const char *filename)

{

struct stat buf;

if(stat(filename, &buf)<0)

{

return 0;

}

return (unsigned long)buf.st_size;

}
//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//
vector< map<string, string> > parseXMLString()
{

	//static const char* xml = XML.c_str();
	TiXmlElement* pCurrentElement = NULL;
	vector< map<string, string> > vOperations;
	
	
	

	TiXmlDocument doc("/data/c.xml");
	doc.LoadFile();
	//doc.Parse( xml );


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
	
	
	return vOperations;
}

static int writeDataCallback(void *data, int size, int nmemb, std::string &content)
{
	long sizes = size * nmemb;
	string temp((char*)data,sizes);
	content += temp;
	return sizes;
}

size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return fwrite(ptr, size, nmemb, stream);
}



bool GetUrl(const string URL, long timeout)
{
	CURL *curl; 
    CURLcode res;
	string content;
	
	FILE *fp = NULL;
	
	//cout<<get_file_size("/data/c.xml")<<endl;
	const char* url = URL.c_str();
    curl_global_init(CURL_GLOBAL_ALL);
 	
    curl = curl_easy_init();
    if (curl) {     

		fp = fopen("/data/baidu.jpg","wb+");
        curl_easy_setopt(curl, CURLOPT_URL, url); 
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, get_file_size("/data/baidu.jpg"));
	    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        res = curl_easy_perform(curl); 

        if (res == CURLE_OK) {    
			cout<<"curl success"<<endl;
			resumeDownload = false; 	        
        }
		else
		{
			resumeDownload = true;
            cout<<"curl error: "<<res<<endl; 
		}

        fclose(fp);	
        curl_easy_cleanup(curl);   
			
    }   
	
	
    curl_global_cleanup();
	
	
	// cout<<"open file"<<endl;
	// fwrite(content.c_str(), 1, downloadFileLenth, fp);
	// cout<<"write file"<<endl;	
	// 
	return true;
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


// /* downloadControl函数用来控制整个下载过程的节奏, 控制下载的次数, 每次等待的时间等 */
// public void downloadControler(const string URL){
// 	downloadFileLenth = getDownloadFileLenth(URL);			//下载前得到要下载的文件大小赋值给全局变量
// 	int times = 605;						//600次*50ms=5分钟, 以此确保5分钟内的重试次数, 而5次是正常下载的中断次数, 意思即是5次内能正常完成下载.
// 	int count = 0;
// 	int timeout = 30;
// 	DSTATUS dstatus = DS_FAILED;
// 	while (count++ < times){
// 		status = scomoDownload(timeout);
// 		if (dstatus == DS_FINISHED){
// 			break;
// 		}
// 		Thread.sleep(500);				//每次下载中间间隔500毫秒
// 	}
// 	resumeDownload = false;				//不管下载成功或失败, 完成while循环后将标志回位
// 	if (dstaus == DS_FINISHED) {
// 		updateApp();					//执行软件安装的操作…
// 	}
// 	SAFE_DELETE(localFile);				//流程最后要确保本地文件删除
// }

int main()
{
	int timeout = 30;
	int times = 605;
	int count = 0;
	bool status = false;
	const string URL = "http://img3.douban.com/view/photo/photo/public/p1705523241.jpg";
	downloadFileLenth = getDownloadFileLenth(URL);
	cout<< "filelenth: "<<downloadFileLenth<<endl;
	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv, &tz);
	long beginTime = tv.tv_usec;
	//for(int i=0;i<100;i++)
	{
	while(count++ < times)
	{
		cout<<"time: "<<count<<endl;
		status = GetUrl(URL, 30);
		if(status == true)
			break;
		sleep(500);
	}

	resumeDownload = false;
	if(status == true)
	{
		//parseXMLString();
		//remove("/data/c.xml");
	}
	}
	gettimeofday (&tv , &tz);
	printf("time cost: %d\n",tv.tv_usec-beginTime);
	// vector< map<string, string> > OP = parseXMLString(GetUrl(URL, 30));
	// for(vector< map<string, string> >::iterator iter = OP.begin(); iter != OP.end(); ++iter)
	// {
	// cout<< (*iter)["cartag"] << endl;
		
	// }

	// map<int, string> MAP = generateSQL(OP);
	// for(map<int, string>::iterator iter = MAP.begin(); iter != MAP.end(); ++iter)
	// {
	// 	cout<< (*iter).first<< "  "<< (*iter).second<<endl;
	// }
	// openDB(MAP);
	
	return 0;
}
