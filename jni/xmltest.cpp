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
	return content;
}

int main()
{
	const string URL = "http://192.168.0.50:8888/testServlet/services/second.do?method=getUpdate";
	downloadFileLenth = getDownloadFileLenth(URL);
	cout<< "filelenth: "<<downloadFileLenth<<endl;
	
	parseXMLString(GetUrl(URL));
	
	//
	// We start with the 'demoStart' todo list. Process it. And
	// should hopefully end up with the todo list as illustrated.
	//
	return 0;
}
