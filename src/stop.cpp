#include "stop.h"

stop::stop(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex,double tri): order(i,ts,ti,ty,q,ex), trigger(tri){}

stop::~stop(){}

double stop::getTrigger() const {
    return trigger;
}

bool stop::operator<(stop& other){
    return trigger < other.getTrigger();
}

bool stop::operator>(stop& other){
    return trigger > other.getTrigger();
}

