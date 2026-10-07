#include "orderBook.h"

using std::map;
using std::vector;
using std::priority_queue;
using std::string;
using std::cout;

orderBook::orderBook(string tick): ticker(tick), Logger(tick + "Stream.txt", tick){}

orderBook::~orderBook(){
    for (auto& [price, orderV] : BuyerSide){
        for (int i = 0; i < orderV.size(); i++){
            delete orderV[i];
        }
    }
    for (auto& [price, orderV] : SellerSide){
        for (int i = 0; i < orderV.size(); i++){
            delete orderV[i];
        }
    }
}

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

// void orderBook::newLimitOrder(limit& newOrder){
//     if (newOrder.getType() == "buy"){
//         // check seller side
//         if (newOrder.getPrice() >= SellerTopPrice.top()){
//             // execute trade
//             executeTrade(newOrder);
//         }
//         // check if all the stocks are traded
//         if (newOrder.getQuantity() == 0) return;

//         // no or not enough match -> add to buyer queue
//         addToSide(newOrder);

//     }
//     else if (newOrder.getType() == "sell"){
//         // check buyer side
//         if (newOrder.getPrice() <= BuyerTopPrice.top()){
//             // execute trade
//             executeTrade(newOrder);
//         }
//         // check if all the stocks are traded
//         if (newOrder.getQuantity() == 0) return;

//         // no or not enough match -> add to seller queue
//         addToSide(newOrder);
//     }
// }

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
}

void orderBook::reallocVector(const order& lastOrder){
    vector<order*> newV;
    vector<order*>* target;

    if (lastOrder.getType() == "sell"){
        if (BuyerSide.size() == 0) return;
        target = &BuyerSide[BuyerTopPrice.top()];
    }
    else {
        if (SellerSide.size() == 0) return;
        target = &SellerSide[SellerTopPrice.top()];
    }   
    
    vector<order*>& oldV = *target;

    int validC = 0;
    for (int i = 0; i < oldV.size(); i++) {
        if (oldV[i] != nullptr) validC++;
    }

    newV.reserve(validC);
    
    for (int i = 0; i < oldV.size(); i++){
        if (oldV[i] != nullptr){
            newV.push_back(oldV[i]);
        }
    }
    oldV.swap(newV); 
}

void orderBook::executeTrade(order &newOrder){
    
    if (newOrder.getExecution() == "limit"){
        if(newOrder.getType() == "sell"){
            if (BuyerSide.size() == 0){
                if (newOrder.getQuantity() > 0){
                    addToSide(dynamic_cast<limit&>(newOrder));
                }
                return;
            }
            if (static_cast<limit&>(newOrder).getPrice() > BuyerTopPrice.top()){
                if (newOrder.getQuantity() > 0){
                    addToSide(static_cast<limit&>(newOrder));
                }
                return;
            }
        }
        else {
            if (SellerSide.size() == 0){
                if (newOrder.getQuantity() > 0){
                    addToSide(static_cast<limit&>(newOrder));
                }
                return;
            }
            if (dynamic_cast<limit&>(newOrder).getPrice() < SellerTopPrice.top()){
                if (newOrder.getQuantity() > 0){
                    addToSide(static_cast<limit&>(newOrder));
                }
                return;
            }
        }
    }

    if(newOrder.getType() == "sell"){
        if (BuyerSide.size() == 0){
            return;
        }
        int buyerIndex = getIndexOfTimePriorityAtPrice('b', BuyerTopPrice.top());
        
        if( buyerIndex == -1){ // no more buyer at given price --> delete price from queue

            Logger.depleat(BuyerTopPrice.top(), 'b'); // log depleat 
            BuyerSide.erase(BuyerTopPrice.top()); // delete old price holder
            BuyerTopPrice.pop();

            // recall this function for next price
            executeTrade(newOrder);
            return; // and end this execution
        }

        if(newOrder.getQuantity() < BuyerSide[BuyerTopPrice.top()][buyerIndex]->getQuantity()){
            // buyer has more. execute trade and end of execution

            Logger.logTrade(BuyerTopPrice.top(), newOrder, static_cast<limit&>(*BuyerSide[BuyerTopPrice.top()][buyerIndex]));
            
            BuyerSide[BuyerTopPrice.top()][buyerIndex]->setQuantity(BuyerSide[BuyerTopPrice.top()][buyerIndex]->getQuantity()-newOrder.getQuantity());
            newOrder.setQuantity(0);
            return;

        }
        else if (newOrder.getQuantity() > BuyerSide[BuyerTopPrice.top()][buyerIndex]->getQuantity()){
            // seller has more and want to sell it --> go deeper

            Logger.logTrade(BuyerTopPrice.top(),newOrder, static_cast<limit&>(*BuyerSide[BuyerTopPrice.top()][buyerIndex]));
        
            newOrder.setQuantity(newOrder.getQuantity()-BuyerSide[BuyerTopPrice.top()][buyerIndex]->getQuantity() );

            // delete depleated order
            delete BuyerSide[BuyerTopPrice.top()][buyerIndex];
            BuyerSide[BuyerTopPrice.top()][buyerIndex] = nullptr;

            if (getIndexOfTimePriorityAtPrice('b', BuyerTopPrice.top()) == -1){
                Logger.depleat(BuyerTopPrice.top(), 'b'); // log depleat 
                BuyerSide.erase(BuyerTopPrice.top()); // delete old price holder
                BuyerTopPrice.pop();
            }

            // recall execute
            executeTrade(newOrder);
            return;

        }
        else{ // ==
            // same quantiti perfect match
            Logger.logTrade(BuyerTopPrice.top(), newOrder, static_cast<limit&>(*BuyerSide[BuyerTopPrice.top()][buyerIndex]));
        
            newOrder.setQuantity(0);
            // delete depleated order
            delete BuyerSide[BuyerTopPrice.top()][buyerIndex];
            BuyerSide[BuyerTopPrice.top()][buyerIndex] = nullptr;

            if (getIndexOfTimePriorityAtPrice('b', BuyerTopPrice.top()) == -1){
                Logger.depleat(BuyerTopPrice.top(), 'b'); // log depleat 
                BuyerSide.erase(BuyerTopPrice.top()); // delete old price holder
                BuyerTopPrice.pop();
            }
            return;
        }
    }
    else{ // buy
        if (SellerSide.size() == 0){
            return;
        }
        int sellerIndex = getIndexOfTimePriorityAtPrice('s', SellerTopPrice.top());
        
        if( sellerIndex == -1){ // no more seller at given price --> delete price from queue

            Logger.depleat(SellerTopPrice.top(), 's'); // log depleat 
            SellerSide.erase(SellerTopPrice.top()); // delete old price holder
            SellerTopPrice.pop();

            // recall this function for next price
            executeTrade(newOrder);
            return; // and end this execution
        }

        if(newOrder.getQuantity() < SellerSide[SellerTopPrice.top()][sellerIndex]->getQuantity()){
            // seller has more. execute trade and end of execution

            Logger.logTrade(SellerTopPrice.top(),newOrder, static_cast<limit&>(*SellerSide[SellerTopPrice.top()][sellerIndex]));
            
            SellerSide[SellerTopPrice.top()][sellerIndex]->setQuantity(SellerSide[SellerTopPrice.top()][sellerIndex]->getQuantity()-newOrder.getQuantity());
            newOrder.setQuantity(0);
            return;

        }
        else if (newOrder.getQuantity() > SellerSide[SellerTopPrice.top()][sellerIndex]->getQuantity()){
            // buyer wants to buy more --> go deeper

            Logger.logTrade(SellerTopPrice.top(), newOrder, static_cast<limit&>(*SellerSide[SellerTopPrice.top()][sellerIndex]));
        
            newOrder.setQuantity(newOrder.getQuantity()-SellerSide[SellerTopPrice.top()][sellerIndex]->getQuantity() );

            // delete depleated order
            delete SellerSide[SellerTopPrice.top()][sellerIndex];
            SellerSide[SellerTopPrice.top()][sellerIndex] = nullptr;

            if (getIndexOfTimePriorityAtPrice('s', SellerTopPrice.top()) == -1){
                Logger.depleat(SellerTopPrice.top(), 's'); // log depleat 
                SellerSide.erase(SellerTopPrice.top()); // delete old price holder
                SellerTopPrice.pop();
            }

            // recall execute
            executeTrade(newOrder);
            return;

        }
        else{ // ==
            // same quantiti perfect match
            Logger.logTrade(SellerTopPrice.top(), newOrder, static_cast<limit&>(*SellerSide[SellerTopPrice.top()][sellerIndex]));
        
            newOrder.setQuantity(0);
            // delete depleated order
            delete SellerSide[SellerTopPrice.top()][sellerIndex];
            SellerSide[SellerTopPrice.top()][sellerIndex] = nullptr;

            if (getIndexOfTimePriorityAtPrice('s', SellerTopPrice.top()) == -1){
                Logger.depleat(SellerTopPrice.top(), 's'); // log depleat 
                SellerSide.erase(SellerTopPrice.top()); // delete old price holder
                SellerTopPrice.pop();
            }
            return;
        }
    }
    
}

int orderBook::getIndexOfTimePriorityAtPrice(char side, double price){
    if (side == 'b'){
        for (int i = 0; i < BuyerSide[price].size(); i++){
            if (BuyerSide[price][i] != nullptr) return i;
        }
        return -1; // if no more buyer at the given price
    }
    if (side == 's'){
        for (int i = 0; i < SellerSide[price].size(); i++){
            if (SellerSide[price][i] != nullptr) return i;
        }
        return -1; // if no more buyer at the given price
    }

    return -1;
}