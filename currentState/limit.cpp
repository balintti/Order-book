#include "limit.h"

limit::limit(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex, double p): order(i,ts,ti,ty,q,ex), price(p){}

limit::~limit(){}

double limit::getPrice() const{
    return price;
}

bool limit::operator<(limit& other){
    return price < other.getPrice();
}

bool limit::operator>(limit& other){
    return price > other.getPrice();
}
