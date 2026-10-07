// Order.h
#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>


class order {
    int id;
    std::string timeStamp;
    std::string ticker; // stock id
    std::string type; // buy / sell
    std::string execution;
    int quantity;

public:
    order(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex);
    ~order();

    // getters
    int getId() const;
    std::string getTimeStamp() const;
    std::string getTicker() const;
    std::string getType() const;
    std::string getExecution() const;
    int getQuantity() const;

    // setters
    int setQuantity(int newQ);
};

#endif