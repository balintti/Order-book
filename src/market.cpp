#include "market.h"

market::market(int i, std::string ts, std::string ti, std::string ty, int q, std::string ex): order(i,ts,ti,ty,q,ex){}
market::~market(){}