// Order.h
#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <sstream>


class order {
    int id;
    std::string timeStamp;
    std::string ticker; // stock id
    std::string type; // buy / sell
    std::string execution; // market / limit / stop
    int quantity;

public:
    order(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex);
    virtual ~order();

    // getters
    int getId() const;
    std::string getTimeStamp() const;
    std::string getTicker() const;
    std::string getType() const;
    std::string getExecution() const;

    int getQuantity() const;

    // setters
    void setQuantity(int newQ);
};

#endif