#include "streamReader.h"

using std::string;
using std::getline;
using std::stringstream;
using std::cerr;


StreamReader::StreamReader(std::string in): streamInName(in), file(in){}

int StreamReader::getOrderNumber(){
    return orderNumber;
}

order* StreamReader::getNextOrder(){
    string line;
    if (getline(file, line)) {
        // Line data layout separeted by " "
        // id(int) timeStamp(string) ticker(string) type(string) executin(string) quantity(int) {price(double) only in limit}

        stringstream ss(line);

        
        string timeStamp;
        string ticker;
        string type;
        string exe;
        int quant;

        if (!(ss >> timeStamp >> ticker >> type >> exe >> quant)){
            cerr << "Invalid type in input stream\n";
            throw 0;
        }

        if (type != "buy" && type != "sell"){
            cerr << "Invalid type of stock only (sell/buy)\n";
            throw 1;
        }

        if (exe != "market" && exe != "limit" && exe != "stop"){
            cerr << "Invalid execution of stock only (market/limit/stop)\n";
            throw 2;
        }

        if (quant <= 0){
            cerr << "Invalid 0 or negative quantity\n";
            throw 3;
        }

        if (exe == "limit"){
            double price;
            if (!(ss >> price)){
                cerr << "Invalid type in input stream\n";
                throw 0;
            }

            if (price <= 0.0){
                cerr << "Invalid price, should be greater than 0\n";
                throw 4;
            }

            limit* newOrder = new limit(orderNumber, timeStamp, ticker, type, quant, exe, price);
            orderNumber++;
            return newOrder;   
        }

        order* newOrder = new market(orderNumber, timeStamp, ticker, type, quant, exe);
        orderNumber++;

        return newOrder;

    }
    
    return nullptr;  // end of stream
}

// ERROR codes:
// 0 --> invalid type in inputstream or segmentation
// 1 --> invalid type of stock only (sell/buy)
// 2 --> invalid execution of stock only (market/limit/stop)
// 3 --> invalid quantity should be greater than 0
// 4 --> invalid price should be greater than 0