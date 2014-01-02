#include "curl/curl.h"
#include "tinyxml/tinyxml.h"
#include <stdio.h>

int example_3()
{
	static const char* xml =
		"<?xml version=\"1.0\"?>"
		"<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
		"<PLAY>"
		"<TITLE>A Midsummer Night's Dream</TITLE>"
		"</PLAY>";

	TiXmlDocument doc;
	doc.Parse( xml );

	TiXmlElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
	const char* title = titleElement->GetText();
	printf( "Name of play (1): %s\n", title );

	TiXmlText* textNode = titleElement->FirstChild()->ToText();
	title = textNode->Value();
	printf( "Name of play (2): %s\n", title );

	return 1;
}

int GetUrl()
{
	CURL *curl; 
    CURLcode res;
 
    curl_global_init(CURL_GLOBAL_ALL);
 
    curl = curl_easy_init();
    if (curl) {     
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/"); 
        res = curl_easy_perform(curl); 
        if (0!=res) {      
            printf("curl error: %d\n", res);         
        }
         
        curl_easy_cleanup(curl);     
    }   
 
    curl_global_cleanup();
	return 0;
}
int main() { 

	example_3();
    GetUrl();
    return 0;
}