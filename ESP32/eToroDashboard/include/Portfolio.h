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
                "company": "Boeing",
                "symbol": "BA",
                "invested": 312.19,
                "units": 2,
                "exchange": "NYSE"
            },
            {
                "company": "Air France-KLM",
                "symbol": "AF.PAR",
                "invested": 206.16,
                "units": 42,
                "exchange": "Euronext Paris"
            },
            {
                "company": "Tencent",
                "symbol": "0700.HK",
                "invested": 413.79,
                "units": 6,
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
                "invested": 389.83,
                "units": 5,
                "exchange": "NASDAQ"
            },
            {
                "company": "Nio",
                "symbol": "NIO",
                "invested": 352.83,
                "units": 19,
                "exchange": "NYSE"
            }
        ]
    })";

    list<Stock> stocks;
};