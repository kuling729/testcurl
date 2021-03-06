#include "downloadController.h"

using namespace std;

downloadController::downloadController()
{
	cout<<"create downloadController"<<endl;
	fileURL = "http://192.168.0.35:8888/testServlet/services/second.do?method=test";
	
	resumeDownload = false;        //是否需要下载的标记位  
	downloadFileLenth = 0;         //需要下载的总大小, 远程文件的大小 
	
	timeout = 30;
}

downloadController::~downloadController()
{
	cout<<"destroy downloadController"<<endl;
}


size_t my_write_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return fwrite(ptr, size, nmemb, stream);
}




bool downloadController::downloadToFile(const string filename)
{
	CURL *curl; 
    CURLcode res;
	string content;
	int retcode = 0;
	FILE *fp = NULL;
	const char* url = fileURL.c_str();
	const int FILE_EXIST = 200;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {     

		fp = fopen(filename.c_str(),"ab+");
        curl_easy_setopt(curl, CURLOPT_URL, url); 
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); 
		//curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, );
		//curl_easy_setopt(curl, CURLOPT_HEADERDATA, ); 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_file);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	    curl_easy_setopt(curl, CURLOPT_RESUME_FROM, getLocalFileLenth(filename));
	    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
       	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        res = curl_easy_perform(curl); 
        fclose(fp);	
        if (CURLE_OK == res) {   
        	res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode); 
        	if(CURLE_OK == res && FILE_EXIST == retcode)
        	{
				cout<<"download success"<<endl;
				resumeDownload = false; 
			}
			else
			{
				cout<<"file not exist"<<endl;
			}	        
        }
		else
		{
			resumeDownload = true;
			cout<<"download failed"<<endl;
            cout<<"curl error: "<<res<<endl;
		}
        curl_easy_cleanup(curl);   			
    }   
    curl_global_cleanup();
	return !resumeDownload;
}

/* 得到远程文件的大小, 要下载的文件大小 */  
double downloadController::getDownloadFileLenth(){  
    downloadFileLenth = 0;  
	const char* url = fileURL.c_str();
    CURL *handle = curl_easy_init();  
    curl_easy_setopt(handle, CURLOPT_URL, url);  
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头  
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body  
    if (curl_easy_perform(handle) == CURLE_OK) {  
        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);  
    } else {  
        downloadFileLenth = -1;  
    }  
    cout<< downloadFileLenth<<endl;
    return downloadFileLenth;  
} 

long downloadController::getLocalFileLenth(const string filename)
{

	struct stat buf;

	if(stat(filename.c_str(), &buf)<0)
	{
		return 0;
	}
	return (long)buf.st_size;
}