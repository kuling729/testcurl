
#include "xmlController.h"


vector< map<string, string> > xmlController::parseFile()
{

	//static const char* xml = XML.c_str();
	TiXmlElement* pCurrentElement = NULL;
	vector< map<string, string> > vOperations;
	
	
	string fileXml = xmlFile;

	TiXmlDocument doc(fileXml.c_str());
	bool isOK = doc.LoadFile(); 
	if(!isOK)
    {
        printf("Error is : %s\n",doc.Error());
        exit(1);
    }
    cout<<"loadfile"<<endl;
	

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

xmlController::xmlController(string xml)
{
	xmlFile = xml;
}

xmlController::~xmlController()
{

}

