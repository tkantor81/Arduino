#include "Finnhub.h"

using namespace std;

float Finnhub::GetRealTimePrice(string symbol)
{
    try
    {
        StaticJsonDocument<500> response = Call("https://finnhub.io/api/v1/quote?symbol=" + symbol + "&token=" + TOKEN);
        return response["c"].as<float>();
    }
    catch (const std::exception& e)
    {
        throw;
    }
}

float Finnhub::GetEndOfDayPrice(string symbol)
{
    throw NotImplemented();
}