#ifndef __RANDINIUM_HPP__
#define __RANDINIUM_HPP__

typedef unsigned long long Rand_t;

class Randinium {
    private:
        Rand_t last_x_;
        Rand_t a_;
        Rand_t m_;
        Rand_t b_;

    public:
        Randinium(Rand_t seed=0);
        ~Randinium();

        Rand_t Rand();
        double NormRand();
};

#endif // __RANDINIUM_HPP__
