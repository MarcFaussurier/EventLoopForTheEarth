//
// Created by marcfsr on 02/02/2019.
//

#ifndef NALP_THREADGROUP_H
#define NALP_THREADGROUP_H

#include <algorithm>

#include "../util/AssociativeArray.h"

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

        static AssociativeArray<float> GetThreadsPerGroup(int nbThreads) {
            const float total = A_percent + B_percent + C_percent + D_percent;
            cout << "In GetThreadsPerGroup with nbThreads=" << nbThreads << endl;
            auto output = new AssociativeArray<float>();
            output->AddItem("A", (A_percent) * nbThreads / total);
            cout << "A=" << (A_percent) * nbThreads / total << endl;
            output->AddItem("B", (B_percent) * nbThreads / total);
            cout << "B=" << (B_percent) * nbThreads / total << endl;
            output->AddItem("C", (C_percent) * nbThreads / total);
            cout << "C=" << (C_percent) * nbThreads / total << endl;
            output->AddItem("D", (D_percent) * nbThreads / total);
            cout << "D=" << (D_percent) * nbThreads / total << endl;
            return *output;
        }

        static char MsToChar(int ms) {
            if (ms >= D) {
                return 'D';
            }

            if (ms >= C) {
                return 'C';
            }

            if (ms >= B) {
                return 'B';
            }

            return 'A';
        }

        static int charToMs(char C) {
           switch(C) {
               case 'A':
                   return A;
               case 'B':
                   return B;
               case 'C':
                   return C;
               default:
                   return D;
           }
        }
    };
}

#endif //NALP_THREADGROUP_H
