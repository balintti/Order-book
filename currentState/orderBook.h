#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>

#include "limit.h"
#include "market.h"
#include "order.h"
#include "stop.h"
#include "stopL.h"
#include "stopM.h"



class orderBook{

    std::string ticker;

    std::map<double,std::vector<order*>> BuyerSide;  
    std::map<double,std::vector<order*>> SellerSide;

    std::priority_queue<double> BuyerTopPrice;
    std::priority_queue<double, std::vector<double>, std::greater<double>> SellerTopPrice;

    std::map<double,std::vector<stop*>> triggerFall;
    std::map<double,std::vector<stop*>> triggerRise;

    std::priority_queue<double> highestFall;
    std::priority_queue<double, std::vector<double>, std::greater<double>> lowestRise;

    public:

        orderBook(std::string tick);
        ~orderBook();

        // getters
        double getTopBuyer();
        double getTopSeller();
        std::string getTicker();
        double getHighFall();
        double getLowRise();


        // order operations

        void newLimitOrder(limit &newOrder);
        void executeTrade(order &newOrder);
        void addToSide(limit &newOrder);
        void checkTriggers();



};


#endif