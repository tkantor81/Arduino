#include "Portfolio.h"
#include <ArduinoJson.h>

using namespace std;

Portfolio::Portfolio()
{
    // parse portfolio to stocks
    // allocate the JSON document
    StaticJsonDocument<1000> jsonDoc;
    
    // deserialize the JSON document
    DeserializationError jsonError = deserializeJson(jsonDoc, portfolio);
    // test if parsing succeeds
    if (jsonError)
    {
        throw runtime_error("deserializeJson() failed" + string(jsonError.c_str()));
    }

    for (int i = 0; i < jsonDoc["portfolio"].size(); ++i)
    {
        Stock stock;
        stock.company = string(jsonDoc["portfolio"][i]["company"].as<char*>());
        stock.symbol = string(jsonDoc["portfolio"][i]["symbol"].as<char*>());
        stock.invested = jsonDoc["portfolio"][i]["invested"].as<float>();
        stock.units = jsonDoc["portfolio"][i]["units"].as<float>();
        stock.exchange = string(jsonDoc["portfolio"][i]["exchange"].as<char*>());
        stocks.push_back(stock);
    }
}

list<Stock>& Portfolio::GetStocks()
{
    return stocks;
}