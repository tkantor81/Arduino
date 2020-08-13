#include <string>
#include "APIProvider.h"
#include "NotImplemented.h"

using namespace std;

class Alphavantage: public APIProvider 
{
public:
    float GetRealTimePrice(string symbol);
    float GetEndOfDayPrice(string symbol);

private:
    const string TOKEN = "B2JESWVO51YB46E2";
};