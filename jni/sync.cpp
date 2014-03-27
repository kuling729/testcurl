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

int main()
{

	downloadController dc;
	xmlController xmlc;
	//dc.getDownloadFileLenth();
	DataBaseController dbc;
	int times = 605;
	
	bool status = false;
	const string filename = "/data/c.xml";

	struct timeval start;
	struct timeval end;

	//for(int i=0;i<10;i++)
	{
	gettimeofday (&start, NULL);
	
	int count = 0;
	while(count++ < times)
	{
		cout<< "download time: "<< count<< endl;
		status = dc.downloadToFile(filename);
		if(status == true)
			break;
		sleep(1);
	}

	dc.resumeDownload = false;
	if(status == true)
	{
		vector< map<string, string> > vOperations = xmlc.parseFile(filename);
		dbc.syncDB(vOperations);
		remove(filename.c_str());
		cout<< "delete file"<< endl;
	}
	
	gettimeofday (&end , NULL);
	unsigned long cost = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	cout<< "cost:"<< cost<< " us"<< endl;

	}
	return 0;
}
