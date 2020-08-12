#include "Marketstack.h"

using namespace std;

float Marketstack::GetRealTimePrice(string symbol)
{
    throw NotImplemented();
}

float Marketstack::GetEndOfDayPrice(string symbol)
{
    try
    {
        StaticJsonDocument<500> response = Call("http://api.marketstack.com/v1/eod/latest?access_key=" + TOKEN + "&symbols=" + symbol);
        return response["data"][0]["close"].as<float>();
    }
    catch (const std::exception& e)
    {
        throw;
    }
}