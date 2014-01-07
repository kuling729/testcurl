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
	xmlController(std::string xml);
	~xmlController();
	vector< map<string, string> > parseFile();
	bool parseString();
private:
	std::string xmlFile;
};

#endif