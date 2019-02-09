//
// Created by marcfsr on 02/02/2019.
//

#ifndef NALP_THREADGROUP_H
#define NALP_THREADGROUP_H

#include <algorithm>

#include "../util/AssociativeArray.hpp"

namespace ipolitic {
    class ThreadGroup {
    public:
        static constexpr const int      A = 0,
                                        B = 50,
                                        C = 200,
                                        D = 1000;

        static constexpr const float    A_percent = 50,
                                        B_percent = 20,
                                        C_percent = 20,
                                        D_percent = 10;

        static AssociativeArray<float> GetThreadsPerGroup(int nbThreads);

        static char MsToChar(float ms);

        static float charToMs(char C);
    };
}

#endif //NALP_THREADGROUP_H
