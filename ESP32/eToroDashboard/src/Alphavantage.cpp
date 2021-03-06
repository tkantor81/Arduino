#include "Alphavantage.h"

using namespace std;

float Alphavantage::GetRealTimePrice(string symbol)
{
    throw NotImplemented();
}

float Alphavantage::GetEndOfDayPrice(string symbol)
{
    try
    {
        StaticJsonDocument<500> response = Call("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + TOKEN);
        string price = response["Global Quote"]["05. price"].as<string>();
        if (price == "null")
        {
            throw runtime_error("API requests limit exceeded or data unavailable");
        }
        return stof(price);
    }
    catch (const std::exception& e)
    {
        throw;
    }
}