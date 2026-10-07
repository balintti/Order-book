#ifndef STOPM_H
#define STOPM_H

#include "stop.h"

class stopM: public stop {

    
    public:

        stopM(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex,double tri);
        ~stopM();

};

#endif