/*
 * random.h
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#ifndef CORE_RANDOM_H_
#define CORE_RANDOM_H_

class Random {
public:
	Random(unsigned int seed = 5489UL) {
        mti = N+1; /* mti==N+1 means mt[N] is not initialized */
        Seed(seed);
    }

    void Seed(unsigned int seed) const;
    float RandomFloat() const;
    unsigned int RandomUInt() const;

private:
    static const int N = 624;
    mutable unsigned long mt[N]; /* the array for the state vector  */
    mutable int mti;
};



#endif /* CORE_RANDOM_H_ */
