#ifndef STOP_L
#define STOP_L

#include "stop.h"

class stopL: public stop {

    double price;

    public:

        stopL(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex,double tri, double pr);
        ~stopL();

};

#endif