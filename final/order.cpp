#include "Order.h"

using std::string;

order::order(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex): id(i), timeStamp(ts), ticker(ti), type(ty), quantity(q), execution(ex){}

order::~order(){}


// getters
int order::getId() const {
    return id;
}

string order::getTimeStamp() const {
    return timeStamp;
}

string order::getTicker() const {
    return ticker;
}

string order::getType() const {
    return type;
}

string order::getExecution() const {
    return execution;
}

int order::getQuantity() const {
    return quantity;
}

// seters
void order::setQuantity(int newQ){
    if (newQ >= 0){
        quantity = newQ;
    }
    else{
        std::cout << "--ERROR--\nInternal error negative quantit" << std::endl;
        throw 1;
    }
}
