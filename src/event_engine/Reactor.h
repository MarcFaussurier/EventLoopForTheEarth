//
// Created by marc on 29/12/18.
//

#ifndef NALP_REACTOR_H
#define NALP_REACTOR_H

#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <unistd.h>

#include "action.h"
#include "../util/AssociativeArray.hpp"
#include "Profiler.h"

using namespace std;

namespace ipolitic {
    class Reactor {
        AssociativeArray<vec_action_stats> * actionStats;
    public:
        char group = 'A';
        Profiler * pro;
        mutex baction_mutex;
        Reactor(AssociativeArray<vec_action_stats> * actionStats);
        thread rThread;
        vector<action> actions;
        bool shouldStop = false;
        void reactorThread();

        float getWaitTime();

        void insertAction(action action, bool stopAfter = false);

        void run();
    };
}


#endif //NALP_REACTOR_H
