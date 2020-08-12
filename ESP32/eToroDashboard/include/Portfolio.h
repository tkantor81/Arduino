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
                "symbol": "AF.XPAR",
                "invested": 345.9,
                "units": 74,
                "exchange": "Euronext Paris"
            },
            {
                "company": "Tencent",
                "symbol": "0700.XHKG",
                "invested": 413.79,
                "units": 6,
                "exchange": "SEHK"
            },
            {
                "company": "Xiaomi",
                "symbol": "1810.XHKG",
                "invested": 241.66,
                "units": 123,
                "exchange": "SEHK"
            },
            {
                "company": "Intel",
                "symbol": "INTC",
                "invested": 247.65,
                "units": 5,
                "exchange": "NASDAQ"
            }
        ]
    })";

    list<Stock> stocks;
};