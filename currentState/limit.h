#ifndef LIMIT_H
#define LIMIT_H
#include "order.h"

class limit: public order{

    double price;

    public:

        limit(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex, double p);

        ~limit();

        double getPrice() const;

        bool operator<(limit& other);
        bool operator>(limit& other);
};

#endif