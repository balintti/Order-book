#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

#include "order.h"
#include "limit.h"
#include "market.h"

class logger {

    static std::string mainFName;
    std::string ticker;
    std::string fName;
    
public:
    
    logger(std::string fN, std::string tick);

    void depleat(double price, char side);
    void logTrade(double price, const order &newOrder, const limit &oldOrder);
    

};

#endif