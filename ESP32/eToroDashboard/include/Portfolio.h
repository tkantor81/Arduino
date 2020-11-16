#include <list>
#include "Stock.h"

using namespace std;

class Portfolio
{
public:
    Portfolio();

    list<Stock>& GetStocks();

private:
    const string portfolio = R"(
    {
        "portfolio": [
            {
                "company": "Tencent",
                "symbol": "0700.HK",
                "invested": 131.99,
                "units": 2,
                "exchange": "SEHK"
            },
            {
                "company": "Xiaomi",
                "symbol": "1810.HK",
                "invested": 377.59,
                "units": 172,
                "exchange": "SEHK"
            },
            {
                "company": "AMD",
                "symbol": "AMD",
                "invested": 589.83,
                "units": 7.57,
                "exchange": "NASDAQ"
            },
            {
                "company": "Nio",
                "symbol": "NIO",
                "invested": 409.83,
                "units": 20.38,
                "exchange": "NYSE"
            },
            {
                "company": "Zynga",
                "symbol": "ZNGA",
                "invested": 384.93,
                "units": 47,
                "exchange": "NASDAQ"
            },
            {
                "company": "Aston Martin",
                "symbol": "AML.L",
                "invested": 573.09,
                "units": 877,
                "exchange": "LSE"
            },
            {
                "company": "Cineworld",
                "symbol": "CINE.L",
                "invested": 428.00,
                "units": 622.53,
                "exchange": "LSE"
            }
        ]
    })";

    list<Stock> stocks;
};