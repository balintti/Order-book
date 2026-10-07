#include <string>
#include <map>
#include <iostream>

#include "order.h"
#include "orderBook.h"
#include "streamReader.h"
#include "logger.h"

using std::string;
using std::map;
using std::cout;
using std::endl;

string logger::mainFName = "mainOutputStream.txt";

int main(){

    string streamIn = "streamIn.txt";
    StreamReader reader(streamIn);

    map<string, orderBook*> stockMarket;

    order* newOrder;
    while(1){
        
        try{
             newOrder = reader.getNextOrder();
        }
        catch (int e){
            if (e == 0){
                cout << "Type error in input stream";
            }
            else{
                cout << "Value error in input stream";
            }
            cout << " At line: " << reader.getOrderNumber() << endl;
            break;
        }

        if (newOrder == nullptr){
            cout << "End of input stream\n";
            break;
        }

        if ( stockMarket.count(newOrder->getTicker()) == 0){
            orderBook* newOrderBook = new orderBook(newOrder->getTicker());
            stockMarket[newOrder->getTicker()] = newOrderBook;
        }

        try{
            stockMarket[newOrder->getTicker()]->executeTrade(*newOrder);
        }
        catch (int e){
            if (e == 1){
                cout << "qouantity error\n";
                break;
            }
        }

        //realloc changed vector
        stockMarket[newOrder->getTicker()]->reallocVector(*newOrder);
        if (newOrder->getQuantity() == 0 || newOrder->getExecution() == "market"){
            delete newOrder;
        }

    }

    for (auto& [ticker, book] : stockMarket ){
        delete book;
    }

    return 0;
}

