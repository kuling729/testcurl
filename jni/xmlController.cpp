
#include "xmlController.h"


vector< map<string, string> > xmlController::parseFile(const string xmlFile)
{

	
	TiXmlElement* pCurrentElement = NULL;
	vector< map<string, string> > vOperations;

	TiXmlDocument doc(xmlFile.c_str());
	bool isOK = doc.LoadFile(); 
	if(!isOK)
    {
        printf("Error is : %s\n",doc.Error());
        exit(1);
    }
    cout<<"parseFile"<<endl;
	

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
	
	// for(vector< map<string, string> >::iterator iter = vOperations.begin(); iter != vOperations.end(); iter++)
	// {
	// cout<< (*iter)["cartag"] << endl;
		
	// }
	
	if ( pCurrentElement )  
    {  
        delete pCurrentElement;  
        pCurrentElement = NULL;  
    }
	cout<<"finish parseFile"<<endl;
	return vOperations;
}

xmlController::xmlController()
{
	
	cout<<"create xmlcontroler"<<endl;
}

xmlController::~xmlController()
{
	cout<<"destroy xmlcontroler"<<endl;
}

