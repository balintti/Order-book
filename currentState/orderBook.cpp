#include "orderBook.h"

using std::map;
using std::vector;
using std::priority_queue;
using std::string;
using std::cout;

orderBook::orderBook(string tick): ticker(tick){}

orderBook::~orderBook(){}

// getters--------------------------------------

double orderBook::getTopBuyer(){
    return BuyerTopPrice.top();
}

double orderBook::getTopSeller(){
    return SellerTopPrice.top();
}

double orderBook::getHighFall(){
    return highestFall.top();
}

double orderBook::getLowRise(){
    return lowestRise.top();
}

string orderBook::getTicker(){
    return ticker;
}

// limit order operations

void orderBook::newLimitOrder(limit& newOrder){
    if (newOrder.getType() == "buy"){
        // check seller side
        if (newOrder.getPrice() >= SellerTopPrice.top()){
            // execute trade
            executeTrade(newOrder);
        }
        // check if all the stocks are traded
        if (newOrder.getQuantity() == 0) return;

        // no or not enough match -> add to buyer queue
        addToSide(newOrder);

    }
    else if (newOrder.getType() == "sell"){
        // check buyer side
        if (newOrder.getPrice() <= BuyerTopPrice.top()){
            // execute trade
            executeTrade(newOrder);
        }
        // check if all the stocks are traded
        if (newOrder.getQuantity() == 0) return;

        // no or not enough match -> add to seller queue
        addToSide(newOrder);
    }
}

void orderBook::addToSide(limit &newOrder){
    if (newOrder.getType() == "buy"){
        // add to queue if new price
        if(!BuyerSide.count(newOrder.getPrice())) BuyerTopPrice.push(newOrder.getPrice());
        // add if exists or aouto construction
        BuyerSide[newOrder.getPrice()].push_back(&newOrder);       
    }
    else{
        // add to queue if new price
        if(!SellerSide.count(newOrder.getPrice())) SellerTopPrice.push(newOrder.getPrice());
        // add if exists or aouto construction
        SellerSide[newOrder.getPrice()].push_back(&newOrder);     
    }
    checkTriggers();
}