#ifndef STREAMREADER_H
#define STREAMREADER_H

#include <string>
#include <iostream>
#include <fstream>

#include "limit.h"
#include "market.h"
#include "order.h"
#include "stop.h"
#include "stopL.h"
#include "stopM.h"

class StreamReader {

    std::string streamInName;
    std::ifstream file; 
    int orderNumber = 0;

    public:

        StreamReader(std::string in);
        order* getNextOrder();
        int getOrderNumber();
};

#endif