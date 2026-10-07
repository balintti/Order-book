#include "logger.h"

using std::string;
using std::ofstream;
using std::cerr;

logger::logger(string fN, string tick): fName(fN), ticker(tick) {}

void logger::depleat(double price, char side){
    
    // stream of one stock

    ofstream file;
    file.open(fName, std::ios::app);

    if (file.is_open()) {
        file << "Deplete in stock: " << ticker << ", at side: " << side << ", at price: " << price << std::endl;  
        file.close(); 
    } else {
        std::cerr << "Failed to open the file: " << fName << std::endl;
    }

    //main stock market stream

    file.open(mainFName, std::ios::app);

    if (file.is_open()) {
        file << "Deplete in stock: " << ticker << ", at side: " << side << ", at price: " << price << std::endl;  
        file.close(); 
    } else {
        std::cerr << "Failed to open the file: " << fName << std::endl;
    }

}

void logger::logTrade(double price, const order &newOrder, const limit &oldOrder){

    // stream of one stock
    
    ofstream file;
    file.open(fName, std::ios::app);

    if (file.is_open()) {
        file <<newOrder.getTimeStamp()<<" Trade in: " << ticker << " between: " << newOrder.getId() << " and " << oldOrder.getId() << " at price: " << oldOrder.getPrice() << " for: " << ((newOrder.getQuantity() < oldOrder.getQuantity()) ? newOrder.getQuantity() : oldOrder.getQuantity()) << " stock" << std::endl;  
        file.close(); 
    } else {
        std::cerr << "Failed to open the file: " << fName << std::endl;
    }

    //main stock market stream
    file.open(mainFName, std::ios::app);
    

    if (file.is_open()) {
        file << newOrder.getTimeStamp()<<" Trade in: " << ticker << " between: " << newOrder.getId() << " and " << oldOrder.getId() << " at price: " << oldOrder.getPrice() << " for: " << ((newOrder.getQuantity() < oldOrder.getQuantity()) ? newOrder.getQuantity() : oldOrder.getQuantity()) << " stock" << std::endl;  
        file.close(); 
    } else {
        std::cerr << "Failed to open the file: " << fName << std::endl;
    }
}