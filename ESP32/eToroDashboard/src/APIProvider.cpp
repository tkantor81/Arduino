#include "APIProvider.h"

using namespace std;

StaticJsonDocument<500> APIProvider::Call(string url)
{
    // allocate the JSON document
    StaticJsonDocument<500> jsonDoc;

    try
    {
        httpClient.begin(url.c_str());
        int httpCode = httpClient.GET();
        
        if (httpCode > 0) // check for the returning code
        {
            String response = httpClient.getString();

            // deserialize the JSON document
            DeserializationError jsonError = deserializeJson(jsonDoc, response);
            // test if parsing succeeds
            if (jsonError)
            {
                throw runtime_error("deserializeJson() failed" + string(jsonError.c_str()));
            }
        }
        else
        {
            throw runtime_error("HTTP request error: " + httpCode);
        }
    }
    catch (const std::exception& e)
    {
        httpClient.end(); // free the resources
        throw e;
    }

    httpClient.end(); // free the resources
    return jsonDoc;
}