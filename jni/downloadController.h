#ifndef __downloadController__
#define __downloadController__

#include <iostream>
#include "curl/curl.h"
#include <string>
#include <sys/stat.h> 

class downloadController{
public:
	downloadController();
	~downloadController();
	bool downloadToFile(const std::string filename);
	double getDownloadFileLenth(); 
	long getLocalFileLenth(const std::string filename);
	bool resumeDownload;
private:
	
	std::string fileURL;
	
	double downloadFileLenth;
	
	int timeout;
};


#endif