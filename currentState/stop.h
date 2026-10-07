#ifndef STOP_H
#define STOP_H

#include "order.h"

class stop: public order{

    double trigger;

    public:

        stop(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex,double tri);
        ~stop();

        double getTrigger() const;

        bool operator<(stop& other);
        bool operator>(stop& other);


};

#endif