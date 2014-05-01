#include "Randinium.hpp"

Randinium::Randinium(Rand_t seed):
    last_x_(seed),
    a_(1601),
    m_(10000),
    b_(3456)
{}

Randinium::~Randinium()
{}

Rand_t Randinium::Rand()
{
    last_x_ = (a_*last_x_ + b_) % m_;
    return last_x_;
}

double Randinium::NormRand()
{
    return (double) Rand()/m_;
}

