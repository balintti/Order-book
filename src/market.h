#ifndef MARKET_H
#define MARKET_H

#include "order.h"

class market: public order{

    public:

        market(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex);
        ~market();

};

#endif