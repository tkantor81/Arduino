#include <string>
#include "APIProvider.h"
#include "NotImplemented.h"

using namespace std;

class Marketstack: public APIProvider 
{
public:
    float GetRealTimePrice(string symbol);
    float GetEndOfDayPrice(string symbol);

private:
    const string TOKEN = "cbf584558164785c57bbb25224f2e47d";
};