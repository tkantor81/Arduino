#include <string>
#include "APIProvider.h"
#include "NotImplemented.h"

using namespace std;

class Finnhub: public APIProvider 
{
public:
    float GetRealTimePrice(string symbol);
    float GetEndOfDayPrice(string symbol);

private:
    const string TOKEN = "bslv61nrh5rdb4ar944g";
};