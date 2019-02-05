//
// Created by marcfsr on 02/02/2019.
//

#include "ThreadGroup.h"

namespace ipolitic {
    AssociativeArray<float> ThreadGroup::GetThreadsPerGroup(int nbThreads) {
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

    char ThreadGroup::MsToChar(int ms) {
        if (ms >= D) {
            return 'D';
        } else if (ms >= C) {
            return 'C';
        } else if (ms >= B) {
            return 'B';
        } else {
            return 'A';
        }
    }

    int ThreadGroup::charToMs(char C) {
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
}
