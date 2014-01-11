#ifndef __xmlController__
#define __xmlController__

#include <iostream>
#include "tinyxml/tinyxml.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class xmlController{
public:
	xmlController();
	~xmlController();
	vector< map<string, string> > parseFile(std::string xml);
	bool parseString();
private:
	
};

#endif