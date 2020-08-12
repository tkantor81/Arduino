#ifndef APIPROVIDER_H
#define APIPROVIDER_H

#include <string>
#include <HTTPClient.h>
#include <ArduinoJson.h>

using namespace std;

class APIProvider
{
public:
    virtual float GetRealTimePrice(string symbol) = 0;
    virtual float GetEndOfDayPrice(string symbol) = 0;

protected:
    HTTPClient httpClient;

    StaticJsonDocument<500> Call(string url);
};

#endif